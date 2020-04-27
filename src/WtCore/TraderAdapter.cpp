/*!
 * \file TraderAdapter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtExecuter.h"
#include "TraderAdapter.h"
#include "ActionPolicyMgr.h"
#include "WtHelper.h"
#include "ITrdNotifySink.h"
#include "RiskMonDefs.h"

#include <atomic>

#include "../WTSTools/WTSLogger.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/WTSError.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSTradeDef.hpp"
#include "../Share/WTSRiskDef.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/WTSSessionInfo.hpp"

#include "../Share/WTSContractInfo.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/decimal.h"



uint32_t makeLocalOrderID()
{
	static std::atomic<uint32_t> _auto_order_id{ 0 };
	if (_auto_order_id == 0)
	{
		uint32_t curYear = TimeUtils::getCurDate() / 10000 * 10000 + 101;
		_auto_order_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(curYear, 0)) / 1000 * 50);
	}

	return _auto_order_id.fetch_add(1);
}

TraderAdapter::TraderAdapter()
	: _id("")
	, _cfg(NULL)
	, _state(AS_NOTLOGIN)
	, _trader_api(NULL)
	, _orders(NULL)
	, _undone_qty(0)
	, _stat_map(NULL)
	, _risk_mon_enabled(false)
{
}


TraderAdapter::~TraderAdapter()
{
	if (_stat_map)
		_stat_map->release();
}

bool TraderAdapter::init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr)
{
	if (params == NULL)
		return false;

	_policy_mgr = policyMgr;
	_bd_mgr = bdMgr;
	_id = id;

	_order_pattern = StrUtil::printf("otp.%s", id);

	if (_cfg != NULL)
		return false;

	_cfg = params;
	_cfg->retain();

	//�������������ز���
	WTSVariant* cfgRisk = params->get("riskmon");
	if (cfgRisk)
	{
		if (cfgRisk->getBoolean("active"))
		{
			_risk_mon_enabled = true;

			WTSVariant* cfgPolicy = cfgRisk->get("policy");
			auto keys = cfgPolicy->memberNames();
			for (auto it = keys.begin(); it != keys.end(); it++)
			{
				const char* product = (*it).c_str();
				WTSVariant*	vProdItem = cfgPolicy->get(product);
				RiskParams& rParam = _risk_params_map[product];
				rParam._cancel_total_limits = vProdItem->getUInt32("cancel_total_limits");
				rParam._cancel_times_boundary = vProdItem->getUInt32("cancel_times_boundary");
				rParam._cancel_stat_timespan = vProdItem->getUInt32("cancel_stat_timespan");

				rParam._order_times_boundary = vProdItem->getUInt32("order_times_boundary");
				rParam._order_stat_timespan = vProdItem->getUInt32("order_stat_timespan");

				WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]����ͨ����ع��� %s �Ѽ���", _id.c_str(), product);
			}

			auto it = _risk_params_map.find("default");
			if (it == _risk_params_map.end())
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[%s]����ͨ��û��Ĭ�Ϸ�ع���, ���ܻ���һЩƷ�ֲ�����н���ͨ�����", _id.c_str());
			}
		}
		else
		{
			WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[%s]����ͨ����ع���δ����", _id.c_str());
		}
	}
	else
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[%s]����ͨ��û�����÷�ع���", _id.c_str());
	}

	if (params->getString("module").empty())
		return false;

	const char* module = params->getCString("module");

	//�ȿ�����Ŀ¼���Ƿ��н���ģ��
	std::string dllpath = WtHelper::getModulePath(module, "traders", true);
	//���û�У����ٿ�ģ��Ŀ¼����dllͬĿ¼��
	if (!StdFile::exists(dllpath.c_str()))
		dllpath = WtHelper::getModulePath(module, "traders", false);
	DllHandle hInst = DLLHelper::load_library(dllpath.c_str());
	if (hInst == NULL)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]����ģ��%s����ʧ��", _id.c_str(), dllpath.c_str());
		return false;
	}

	FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
	if (NULL == pFunCreateTrader)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[%s]���׽ӿڴ���������ȡʧ��", _id.c_str());
		return false;
	}

	_trader_api = pFunCreateTrader();
	if (NULL == _trader_api)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_FATAL, "[%s]���׽ӿڴ���ʧ��", _id.c_str());
		return false;
	}

	_remover = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");

	WTSParams* cfg = params->toParams();
	if (!_trader_api->init(cfg))
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]���׽ӿ�����ʧ��: ���׽ӿڳ�ʼ��ʧ��", id);
		cfg->release();
		return false;
	}

	cfg->release();

	return true;
}

const TraderAdapter::RiskParams* TraderAdapter::getRiskParams(const char* stdCode)
{
	std::string pid = CodeHelper::stdCodeToStdCommID(stdCode);
	auto it = _risk_params_map.find(pid);
	if (it != _risk_params_map.end())
		return &it->second;

	it = _risk_params_map.find("default");
	return &it->second;
}

bool TraderAdapter::run()
{
	if (_trader_api == NULL)
		return false;

	if (_stat_map == NULL)
		_stat_map = TradeStatMap::create();

	_trader_api->registerListener(this);

	_trader_api->connect();
	_state = AS_LOGINING;
	return true;
}

void TraderAdapter::release()
{
	if (_trader_api)
	{
		_trader_api->registerListener(NULL);
		_trader_api->release();
	}
}

double TraderAdapter::getPosition(const char* stdCode, int32_t flag /* = 3 */)
{
	auto it = _positions.find(stdCode);
	if (it == _positions.end())
		return 0;

	double ret = 0;
	const PosItem& pItem = it->second;
	if(flag & 1)
	{
		ret += (pItem.l_newvol + pItem.l_prevol);
	}

	if (flag & 2)
	{
		ret -= pItem.s_newvol + pItem.s_prevol;
	}
	return ret;
}

OrderMap* TraderAdapter::getOrders(const char* stdCode)
{
	if (_orders == NULL)
		return NULL;

	bool isAll = strlen(stdCode) == 0;

	OrderMap* ret = OrderMap::create();
	for (auto it = _orders->begin(); it != _orders->end(); it++)
	{
		uint32_t localid = it->first;
		WTSOrderInfo* ordInfo = (WTSOrderInfo*)it->second;

		if (isAll || strcmp(ordInfo->getCode(), stdCode) == 0)
			ret->add(localid, ordInfo);
	}
	return ret;
}

uint32_t TraderAdapter::doEntrust(WTSEntrust* entrust)
{
	char entrustid[64] = { 0 };
	if (_trader_api->makeEntrustID(entrustid, 64))
	{
		entrust->setEntrustID(entrustid);
	}

	std::string exchg, pid, code;
	bool isHot = false;
	CodeHelper::extractStdCode(entrust->getCode(), exchg, code, pid, isHot);
	entrust->setCode(code.c_str());
	entrust->setExchange(exchg.c_str());

	uint32_t localid = makeLocalOrderID();
	entrust->setUserTag(StrUtil::printf("%s.%u", _order_pattern.c_str(), localid).c_str());
	
	int32_t ret = _trader_api->orderInsert(entrust);
	if(ret < 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]�µ�ʧ��, ������: %d", _id.c_str(), ret);
	}
	_order_time_cache[entrust->getCode()].push_back(TimeUtils::getLocalTimeNow());
	return localid;
}

WTSContractInfo* TraderAdapter::getContract(const char* stdCode)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdCode(stdCode, cInfo);
	return _bd_mgr->getContract(cInfo._code, cInfo._exchg);
}

WTSCommodityInfo* TraderAdapter::getCommodify(const char* stdCode)
{
	std::string commID = CodeHelper::stdCodeToStdCommID(stdCode);
	return _bd_mgr->getCommodity(commID.c_str());
}

bool TraderAdapter::checkCancelLimits(const char* stdCode)
{
	if (!_risk_mon_enabled)
		return true;

	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	const RiskParams* riskPara = getRiskParams(stdCode);
	if (riskPara == NULL)
		return true;

	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo->total_cancels() >= riskPara->_cancel_total_limits )
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]��Լ %s �������� %u �ѳ������� %u ��,  �����ų��б�",
			_id.c_str(), stdCode, statInfo->total_cancels(), riskPara->_cancel_total_limits);
		_exclude_codes.insert(stdCode);
		return false;
	}

	//����Ƶ�ʼ��
	auto it = _cancel_time_cache.find(stdCode);
	if (it != _cancel_time_cache.end())
	{
		TimeCacheList& cache = it->second;
		uint32_t cnt = cache.size();
		if (cnt >= riskPara->_cancel_times_boundary)
		{
			uint64_t eTime = cache[cnt - 1];
			uint64_t sTime = eTime - riskPara->_cancel_stat_timespan * 1000;
			auto tit = std::lower_bound(cache.begin(), cache.end(), sTime);
			uint32_t sIdx = tit - cache.begin();
			uint32_t times = cnt - sIdx - 1;
			if (times > riskPara->_cancel_times_boundary)
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]��Լ %s ����Ƶ�ʹ���, %u ���ڳ��� %u ��, ���ڱ߽�ֵ %u ��, �����ų��б�",
					_id.c_str(), stdCode, riskPara->_cancel_stat_timespan, times, riskPara->_cancel_times_boundary);
				_exclude_codes.insert(stdCode);
				return false;
			}

			//�������Ҫ����һ��, û���ر�õİ취
			//��Ȼ����ʱ������, vector���Ȼ�Խ��Խ��
			if(tit != cache.begin())
			{
				cache.erase(cache.begin(), tit);
			}
		}
	}

	return true;
}

bool TraderAdapter::isTradeEnabled(const char* stdCode) const
{
	if (!_risk_mon_enabled)
		return true;

	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	return true;
}

bool TraderAdapter::checkOrderLimits(const char* stdCode)
{
	if (!_risk_mon_enabled)
		return true;

	if (_exclude_codes.find(stdCode) != _exclude_codes.end())
		return false;

	const RiskParams* riskPara = getRiskParams(stdCode);
	if (riskPara == NULL)
		return true;

	//����Ƶ�ʼ��
	auto it = _order_time_cache.find(stdCode);
	if (it != _order_time_cache.end())
	{
		TimeCacheList& cache = it->second;
		uint32_t cnt = cache.size();
		if (cnt >= riskPara->_order_times_boundary)
		{
			uint64_t eTime = cache[cnt - 1];
			uint64_t sTime = eTime - riskPara->_order_stat_timespan * 1000;
			auto tit = std::lower_bound(cache.begin(), cache.end(), sTime);
			uint32_t sIdx = tit - cache.begin();
			uint32_t times = cnt - sIdx - 1;
			if (times > riskPara->_order_times_boundary)
			{
				WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]��Լ %s �µ���Ƶ�ʹ���, %u ���ڳ��� %u ��, ���ڱ߽�ֵ %u ��, �����ų��б�",
					_id.c_str(), stdCode, riskPara->_order_stat_timespan, times, riskPara->_order_times_boundary);
				_exclude_codes.insert(stdCode);
				return false;
			}

			//�������Ҫ����һ��, û���ر�õİ취
			//��Ȼ����ʱ������, vector���Ȼ�Խ��Խ��
			if (tit != cache.begin())
			{
				cache.erase(cache.begin(), tit);
			}
		}
	}

	return true;
}

OrderIDs TraderAdapter::buy(const char* stdCode, double price, double qty)
{
	OrderIDs ret;
	if (qty == 0)
		return ret;

	WTSContractInfo* cInfo = getContract(stdCode);
	WTSCommodityInfo* commInfo = getCommodify(stdCode);
	WTSSessionInfo* sInfo = _bd_mgr->getSession(commInfo->getSession());

	if (!sInfo->isInTradingTime(WtHelper::getTime(), true))
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR, "[%s]���� %s ��Լ%f��, %04u ���ڽ���ʱ��", _id.c_str(), stdCode, qty, WtHelper::getTime());
		return ret;
	}

	//if(!checkOrderLimits(stdCode))
	//{
	//	return ret;
	//}

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]���� %s ��Լ%f��", _id.c_str(), stdCode, qty);

	double oldQty = _undone_qty[stdCode];
	double newQty = oldQty + qty;
	_undone_qty[stdCode] = newQty;
	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]��Լ %s δ��ɶ�����������, %f -> %f", _id.c_str(), stdCode, oldQty, newQty);

	const PosItem& pItem = _positions[stdCode];
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode);
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();

	std::string stdCommID = CodeHelper::stdCodeToStdCommID(stdCode);
	const ActionRuleGroup& ruleGP = _policy_mgr->getActionRules(stdCommID.c_str());

	double left = qty;

	double unitQty = (price == 0.0) ? cInfo->getMaxMktVol() : cInfo->getMaxLmtVol();

	for (auto it = ruleGP.begin(); it != ruleGP.end(); it++)
	{
		const ActionRule& curRule = (*it);
		if(curRule._atype == AT_Open)
		{
			//�ȼ���Ƿ��Ѿ������޶�
			//���뿪��, �������
			double maxQty = left;

			if (curRule._limit_l != 0)
			{
				if (statItem.l_openvol >= curRule._limit_l)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[%s]��Լ %s ���տ�����Ѵ��޶�%u", _id.c_str(), stdCode, curRule._limit_l);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit_l - statItem.l_openvol);
				}
			}

			if (curRule._limit != 0)
			{
				if (statItem.l_openvol + statItem.s_openvol >= curRule._limit)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_WARN, "[%s]��Լ %s ���տ����Ѵ��޶�%u", _id.c_str(), stdCode, curRule._limit);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit - statItem.l_openvol - statItem.s_openvol);
				}
			}

			//���ﻹҪ���ǵ������ί������
			double leftQty = maxQty;
			for (;;)
			{
				double curQty = min(leftQty, unitQty);
				uint32_t localid = openLong(stdCode, price, curQty);
				ret.push_back(localid);

				leftQty -= curQty;

				if (decimal::eq(leftQty, 0))
					break;
			}			

			left -= maxQty;

			WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]��Լ %s ����%f���ź�ִ��: ����%f��", _id.c_str(), stdCode, qty, maxQty);
		}
		else if(curRule._atype == AT_CloseToday)
		{
			double maxQty = 0;
			//���Ҫ����ƽ��ƽ���Ʒ��, ��ֻ��ȡ��ƽ��ּ���
			//���������ƽ��ƽ���Ʒ��, ���ȡȫ����ƽ, ��Ϊ��ȡ��ƴ���Ҳû����
			if (commInfo->getCoverMode() == CM_CoverToday)
				maxQty = min(left, pItem.s_newavail);	//�ȿ�����ƽ���
			else
				maxQty = min(left, pItem.avail_pos(false));
			
			//ƽ�ֲ�����޶�			

			//���ﻹҪ���ǵ������ί������
			//if (maxQty > 0)
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeShort(stdCode, price, curQty, (commInfo->getCoverMode() == CM_CoverToday));//�����֧��ƽ��, ��ֱ����ƽ�ֱ�Ǽ���
					ret.push_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				else
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
			}
			
		}
		else if (curRule._atype == AT_CloseYestoday)
		{
			//ƽ��Ƚϼ�, ��Ϊ����Ҫ���ֱ��
			double maxQty = min(left, pItem.s_preavail);
			//ƽ�ֲ�����޶�			

			//���ﻹҪ���ǵ������ί������
			//if (maxQty > 0)
			if (decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeShort(stdCode, price, curQty, false);//�����֧��ƽ��, ��ֱ����ƽ�ֱ�Ǽ���
					ret.push_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}

				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				else
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO, "[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
			}
		}
		else if (curRule._atype == AT_Close)
		{
			//���ֻ��ƽ��, ����������
			//�������ƽ��ƽ��, ��Ҫ��ƽ����ƽ��
			//���������ƽ��ƽ��, ��ͳһƽ��
			if (commInfo->getCoverMode() != CM_CoverToday)
			{
				double maxQty = min(pItem.avail_pos(false), left);
				
				//if (maxQty > 0)
				if (decimal::gt(maxQty, 0))
				{
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, false);
						ret.push_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
				}
			}
			else
			{
				//if (pItem.s_preavail > 0)
				if (decimal::gt(pItem.s_preavail, 0))
				{
					//�Ƚ���ƽ���ƽ��
					double maxQty = min(pItem.s_preavail, qty);
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, false);
						ret.push_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				}

				//if (left > 0 && pItem.s_newavail > 0)
				if (decimal::gt(left, 0) && decimal::gt(pItem.s_newavail, 0))
				{
					//�ٽ���ƽ���ƽ��
					//TODO: ���ﻹ��һ������, ����ǿ������ֵĻ�, ����߼�������ȥ��
					double maxQty = min(pItem.s_newavail, left);
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeShort(stdCode, price, curQty, true);
						ret.push_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				}
			}
		}

		if(left == 0)
			break;
	}

	if(left > 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]%s��Լ����%f�������ź�δִ�����", _id.c_str(), stdCode, left);
	}

	return ret;
}

OrderIDs TraderAdapter::sell(const char* stdCode, double price, double qty)
{
	OrderIDs ret;
	if (qty == 0)
		return ret;

	WTSContractInfo* cInfo = getContract(stdCode);
	WTSCommodityInfo* commInfo = getCommodify(stdCode);
	WTSSessionInfo* sInfo = _bd_mgr->getSession(commInfo->getSession());

	if(!sInfo->isInTradingTime(WtHelper::getTime(), true))
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]���� %s ��Լ%f��, %04u ���ڽ���ʱ��", _id.c_str(), stdCode, qty, WtHelper::getTime());
		return ret;
	}

	//if (!checkOrderLimits(stdCode))
	//{
	//	return ret;
	//}

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]���� %s ��Լ%f��", _id.c_str(), stdCode, qty);

	double oldQty = _undone_qty[stdCode];
	double newQty = oldQty - qty;
	_undone_qty[stdCode] = newQty;
	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ %s δ��ɶ�����������, %f -> %f", _id.c_str(), stdCode, oldQty, newQty);

	const PosItem& pItem = _positions[stdCode];	
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode);
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode);
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();

	std::string stdCommID = CodeHelper::stdCodeToStdCommID(stdCode);
	const ActionRuleGroup& ruleGP = _policy_mgr->getActionRules(stdCommID.c_str());

	double left = qty;

	double unitQty = (price == 0.0) ? cInfo->getMaxMktVol() : cInfo->getMaxLmtVol();

	for (auto it = ruleGP.begin(); it != ruleGP.end(); it++)
	{
		const ActionRule& curRule = (*it);
		if (curRule._atype == AT_Open)
		{
			//�ȼ���Ƿ��Ѿ������޶�
			//���뿪��, �������
			double maxQty = left;

			if (curRule._limit_s != 0)
			{
				if (statItem.s_openvol >= curRule._limit_s)
				{
					WTSLogger::info("[%s]��Լ%s���տ��ղ��Ѵ��޶�%u", _id.c_str(), stdCode, curRule._limit_l);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit_s - statItem.s_openvol);
				}
			}

			if (curRule._limit != 0)
			{
				if (statItem.l_openvol + statItem.s_openvol >= curRule._limit)
				{
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ%s���տ����Ѵ��޶�%u", _id.c_str(), stdCode, curRule._limit);
					continue;
				}
				else
				{
					maxQty = min(maxQty, curRule._limit - statItem.l_openvol - statItem.s_openvol);
				}
			}

			//���ﻹҪ���ǵ������ί������
			double leftQty = maxQty;
			for (;;)
			{
				double curQty = min(leftQty, unitQty);
				uint32_t localid = openShort(stdCode, price, curQty);
				ret.push_back(localid);

				leftQty -= curQty;

				//if (leftQty == 0)
				if (decimal::eq(leftQty, 0))
					break;
			}

			left -= maxQty;

			WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ����%f��", _id.c_str(), stdCode, qty, maxQty);
		}
		else if (curRule._atype == AT_CloseToday)
		{
			double maxQty = 0;
			//���Ҫ����ƽ��ƽ���Ʒ��, ��ֻ��ȡ��ƽ��ּ���
			//���������ƽ��ƽ���Ʒ��, ���ȡȫ����ƽ, ��Ϊ��ȡ��ƽ���Ҳû����
			if (commInfo->getCoverMode() == CM_CoverToday)
				maxQty = min(left, pItem.l_newavail);	//�ȿ�����ƽ���
			else
				maxQty = min(left, pItem.avail_pos(true));

			/*
			 *	ƽ�ֲ�����޶�
			 */	


			//���ﻹҪ���ǵ������ί������
			//if (maxQty > 0)
			if(decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeLong(stdCode, price, curQty, (commInfo->getCoverMode() == CM_CoverToday));//�����֧��ƽ��, ��ֱ����ƽ�ֱ�Ǽ���
					ret.push_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				else
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
			}

		}
		else if (curRule._atype == AT_CloseYestoday)
		{
			//ƽ��Ƚϼ�, ��Ϊ����Ҫ���ֱ��
			double maxQty = min(left, pItem.l_preavail);
			
			/*
			 *	ƽ�ֲ�����޶�
			 */

			//���ﻹҪ���ǵ������ί������
			//if (maxQty > 0)
			if(decimal::gt(maxQty, 0))
			{
				double leftQty = maxQty;
				for (;;)
				{
					double curQty = min(leftQty, unitQty);
					uint32_t localid = closeLong(stdCode, price, curQty, false);//�����֧��ƽ��, ��ֱ����ƽ�ֱ�Ǽ���
					ret.push_back(localid);

					leftQty -= curQty;

					//if (leftQty == 0)
					if (decimal::eq(leftQty, 0))
						break;
				}
				left -= maxQty;

				if (commInfo->getCoverMode() == CM_CoverToday)
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
				else
					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
			}
		}
		else if (curRule._atype == AT_Close)
		{
			//���ֻ��ƽ��, ����������
			//�������ƽ��ƽ��, ��Ҫ��ƽ����ƽ��
			//���������ƽ��ƽ��, ��ͳһƽ��
			if (commInfo->getCoverMode() != CM_CoverToday)
			{
				double maxQty = min(pItem.avail_pos(true), left);	//������ƽ��ƽ��, ���ȡȫ����ƽ��
				//if(maxQty > 0)
				if(decimal::gt(maxQty, 0))
				{
					double leftQty = maxQty;
					for (;;)
					{
						double curQty = min(leftQty, unitQty);
						uint32_t localid = closeLong(stdCode, price, curQty, false);
						ret.push_back(localid);

						leftQty -= curQty;

						//if (leftQty == 0)
						if (decimal::eq(leftQty, 0))
							break;
					}
					left -= maxQty;

					WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ��%f��", _id.c_str(), stdCode, qty, maxQty);
				}
				
			}
			else
			{
				//if (left > 0 && pItem.l_preavail > 0)
				if (decimal::gt(left, 0) && decimal::gt(pItem.l_preavail, 0))
				{
					//�Ƚ���ƽ���ƽ��
					double maxQty = min(pItem.l_preavail, qty);
					//if (maxQty)
					if(decimal::gt(maxQty, 0))
					{
						double leftQty = maxQty;
						for (;;)
						{
							double curQty = min(leftQty, unitQty);
							uint32_t localid = closeLong(stdCode, price, curQty, false);
							ret.push_back(localid);

							leftQty -= curQty;

							//if (leftQty == 0)
							if (decimal::eq(leftQty, 0))
								break;
						}
						left -= maxQty;

						WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
					}
					
				}

				//if (left > 0 && pItem.l_newavail > 0)
				if (decimal::gt(left, 0) && decimal::gt(pItem.l_newavail, 0))
				{
					//�ٽ���ƽ���ƽ��
					//TODO: ���ﻹ��һ������, ����ǿ������ֵĻ�, ����߼�������ȥ��
					double maxQty = min(pItem.l_newavail, left);
					//if(maxQty > 0)
					if(decimal::gt(maxQty, 0))
					{
						double leftQty = maxQty;
						for (;;)
						{
							double curQty = min(leftQty, unitQty);
							uint32_t localid = closeLong(stdCode, price, curQty, true);
							ret.push_back(localid);

							leftQty -= curQty;

							//if (leftQty == 0)
							if (decimal::eq(leftQty, 0))
								break;
						}
						left -= maxQty;

						WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s��Լ����%f���ź�ִ��: ƽ���%f��", _id.c_str(), stdCode, qty, maxQty);
					}
					
				}
			}
		}

		if (left == 0)
			break;
	}

	if (left > 0)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]%s��Լ����%f�������ź�δִ�����", _id.c_str(), stdCode, left);
	}

	return ret;
}

bool TraderAdapter::doCancel(WTSOrderInfo* ordInfo)
{
	if (ordInfo == NULL || !ordInfo->isAlive())
		return false;

	WTSContractInfo* cInfo = _bd_mgr->getContract(ordInfo->getCode(), ordInfo->getExchg());
	WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
	std::string stdCode;
	if (commInfo->getCategoty() == CC_Future)
		stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	else
		stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

	//����Ƶ�ʼ��
	if (!checkCancelLimits(stdCode.c_str()))
		return false;

	WTSEntrustAction* action = WTSEntrustAction::create(ordInfo->getCode(), cInfo->getExchg());
	action->setEntrustID(ordInfo->getEntrustID());
	action->setOrderID(ordInfo->getOrderID());
	int ret = _trader_api->orderAction(action);
	bool isSent = (ret >= 0);
	action->release();
	return isSent;
}

bool TraderAdapter::cancel(uint32_t localid)
{
	if (_orders == NULL || _orders->size() == 0)
		return false;

	BoostUniqueLock lock(_mtx_orders);
	WTSOrderInfo* ordInfo = (WTSOrderInfo*)_orders->get(localid);
	if (ordInfo == NULL)
		return false;

	ordInfo->retain();
	bool bRet = doCancel(ordInfo);
	ordInfo->release();

	_cancel_time_cache[ordInfo->getCode()].push_back(TimeUtils::getLocalTimeNow());

	return bRet;
}

OrderIDs TraderAdapter::cancel(const char* stdCode, bool isBuy, double qty /* = 0 */)
{
	std::string exchg, pid, code;
	bool isHot = false;
	CodeHelper::extractStdCode(stdCode, exchg, code, pid, isHot);

	OrderIDs ret;

	double actQty = 0;
	bool isAll = strlen(stdCode) == 0;
	if (_orders != NULL && _orders->size() > 0)
	{
		for (auto it = _orders->begin(); it != _orders->end(); it++)
		{
			WTSOrderInfo* orderInfo = (WTSOrderInfo*)it->second;
			if (isAll || strcmp(orderInfo->getCode(), code.c_str()) == 0)
			{
				if(doCancel(orderInfo))
				{
					actQty += orderInfo->getVolLeft();
					ret.push_back(it->first);
					_cancel_time_cache[orderInfo->getCode()].push_back(TimeUtils::getLocalTimeNow());
				}
			}

			if (!decimal::eq(qty, 0) && decimal::ge(actQty, qty))
				break;
		}
	}

	return ret;
}

uint32_t TraderAdapter::openLong(const char* stdCode, double price, double qty)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if(price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);
	}
	entrust->setDirection(WDT_LONG);
	entrust->setOffsetType(WOT_OPEN);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::openShort(const char* stdCode, double price, double qty)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);
	}
	entrust->setDirection(WDT_SHORT);
	entrust->setOffsetType(WOT_OPEN);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::closeLong(const char* stdCode, double price, double qty, bool isToday /* = false */)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);
	}
	entrust->setDirection(WDT_LONG);
	entrust->setOffsetType(isToday ? WOT_CLOSETODAY : WOT_CLOSE);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}

uint32_t TraderAdapter::closeShort(const char* stdCode, double price, double qty, bool isToday /* = false */)
{
	WTSEntrust* entrust = WTSEntrust::create(stdCode, qty, price);
	if (price == 0.0)
	{
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);
	}
	else
	{
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);
	}
	entrust->setDirection(WDT_SHORT);
	entrust->setOffsetType(isToday ? WOT_CLOSETODAY : WOT_CLOSE);

	uint32_t ret = doEntrust(entrust);
	entrust->release();
	return ret;
}


#pragma region "ITraderApiListener�ӿ�"
void TraderAdapter::handleEvent(WTSTraderEvent e, int32_t ec)
{
	if(e == WTE_Connect)
	{
		if(ec == 0)
		{
			_trader_api->login(_cfg->getCString("user"), _cfg->getCString("pass"), _cfg->getCString("product"));
		}
		else
		{
			WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]�����˺�����ʧ��: %d", _id.c_str(), ec);
		}
	}
	else if(e == WTE_Close)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]�����˺������ѶϿ�: %d", _id.c_str(), ec);
		for (auto sink : _sinks)
			sink->on_channel_lost();
	}
}

void TraderAdapter::onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
{
	if(!bSucc)
	{
		_state = AS_LOGINFAILED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]�����˺ŵ�¼ʧ��: %s", _id.c_str(), msg);
	}
	else
	{
		_state = AS_LOGINED;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]�����˺ŵ�¼�ɹ�, ��ǰ������:%u", _id.c_str(), tradingdate);

		_trader_api->queryPositions();	//��ֲ�
	}
}

void TraderAdapter::onLogout()
{
	
}

void TraderAdapter::onRspEntrust(WTSEntrust* entrust, WTSError *err)
{
	if (err && err->getErrorCode() != WEC_NONE)
	{
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,err->getMessage());
		WTSContractInfo* cInfo = _bd_mgr->getContract(entrust->getCode(), entrust->getExchg());
		WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
		std::string stdCode;
		if (commInfo->getCategoty() == CC_Future)
			stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
		else
			stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

		bool isLong = (entrust->getDirection() == WDT_LONG);
		bool isToday = (entrust->getOffsetType() == WOT_CLOSETODAY);
		bool isOpen = (entrust->getOffsetType() == WOT_OPEN);
		double qty = entrust->getVolumn();

		std::string action;
		if (isOpen)
			action = "��";
		else if (isToday)
			action = "ƽ��";
		else
			action = "ƽ";
		action += isLong ? "��" : "��";

		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]�µ�ʧ��: %s, ��Լ: %s, ����: %s, ����: %f", _id.c_str(), err->getMessage(), entrust->getCode(), action.c_str(), qty);

		//����µ�ʧ��, Ҫ����δ�������
		bool isBuy = (isLong&&isOpen) || (!isLong&&!isOpen);
		double oldQty = _undone_qty[stdCode];
		double newQty = oldQty - qty*(isBuy ? 1 : -1);
		_undone_qty[stdCode] = newQty;

		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ %s δ��ɶ�����������, %f -> %f", _id.c_str(), stdCode.c_str(), oldQty, newQty);


		if (strlen(entrust->getUserTag()) > 0)
		{
			char* userTag = (char*)entrust->getUserTag();
			userTag += _order_pattern.size() + 1;
			uint32_t localid = strtoul(userTag, NULL, 10);

			for(auto sink : _sinks)
				sink->on_entrust(localid, stdCode.c_str(), false, err->getMessage());
		}
		
	}
}

void TraderAdapter::onRspAccount(WTSArray* ayAccounts)
{
	
}

void TraderAdapter::onRspPosition(const WTSArray* ayPositions)
{
	if (ayPositions && ayPositions->size() > 0)
	{
		for (auto it = ayPositions->begin(); it != ayPositions->end(); it++)
		{
			WTSPositionItem* pItem = (WTSPositionItem*)(*it);
			WTSContractInfo* cInfo = _bd_mgr->getContract(pItem->getCode());
			if (cInfo == NULL)
				continue;

			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
			std::string stdCode;
			if (commInfo->getCategoty() == CC_Future)
				stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
			else
				stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
			PosItem& pos = _positions[stdCode];
			if (pItem->getDirection() == WDT_LONG)
			{
				pos.l_newavail = pItem->getAvailNewPos();
				pos.l_newvol = pItem->getNewPosition();
				pos.l_preavail = pItem->getAvailPrePos();
				pos.l_prevol = pItem->getPrePosition();
			}
			else
			{
				pos.s_newavail = pItem->getAvailNewPos();
				pos.s_newvol = pItem->getNewPosition();
				pos.s_preavail = pItem->getAvailPrePos();
				pos.s_prevol = pItem->getPrePosition();
			}
		}

		for (auto it = _positions.begin(); it != _positions.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosItem& pItem = it->second;
			printPosition(stdCode, pItem);
			for (auto sink : _sinks)
			{
				sink->on_position(stdCode, true, pItem.l_prevol, pItem.l_preavail, pItem.l_newvol, pItem.l_newavail);
				sink->on_position(stdCode, false, pItem.s_prevol, pItem.s_preavail, pItem.s_newvol, pItem.s_newavail);
			}
		}
	}

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]�ֲ������Ѹ���", _id.c_str());

	if (_state == AS_LOGINED)
	{
		_state = AS_POSITION_QRYED;

		_trader_api->queryOrders();
	}
}

void TraderAdapter::onRspOrders(const WTSArray* ayOrders)
{
	if (ayOrders)
	{
		if (_orders == NULL)
			_orders = OrderMap::create();

		_undone_qty.clear();

		for (auto it = ayOrders->begin(); it != ayOrders->end(); it++)
		{
			WTSOrderInfo* orderInfo = (WTSOrderInfo*)(*it);
			if (orderInfo == NULL)
				continue;

			WTSContractInfo* cInfo = _bd_mgr->getContract(orderInfo->getCode());
			if (cInfo == NULL)
				continue;

			bool isBuy = (orderInfo->getDirection() == WDT_LONG && orderInfo->getOffsetType() == WOT_OPEN) || (orderInfo->getDirection() == WDT_SHORT && orderInfo->getOffsetType() != WOT_OPEN);

			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
			std::string stdCode;
			if (commInfo->getCategoty() == CC_Future)
				stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
			else
				stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

			_orderids.insert(orderInfo->getOrderID());

			WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
			if (statInfo == NULL)
			{
				statInfo = WTSTradeStateInfo::create(stdCode.c_str());
				_stat_map->add(stdCode, statInfo, false);
			}
			TradeStatInfo& statItem = statInfo->statInfo();
			if (isBuy)
			{
				statItem.b_orders++;
				statItem.b_ordqty += orderInfo->getVolumn();

				if (orderInfo->isError())
				{
					statItem.b_wrongs++;
					statItem.b_wrongqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
				}
				else if (orderInfo->getOrderState() == WOS_Canceled)
				{			
					statItem.b_cancels++;
					statItem.b_canclqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
				}
				
			}
			else
			{
				statItem.s_orders++;
				statItem.s_ordqty += orderInfo->getVolumn();

				if (orderInfo->isError())
				{
					statItem.s_wrongs++;
					statItem.s_wrongqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
				}
				else if (orderInfo->getOrderState() == WOS_Canceled)
				{
					statItem.s_cancels++;
					statItem.s_canclqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
				}
			}			

			if (!orderInfo->isAlive())
				continue;

			if (!StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern))
				continue;;

			char* userTag = (char*)orderInfo->getUserTag();
			userTag += _order_pattern.size() + 1;
			uint32_t localid = strtoul(userTag, NULL, 10);

			{
				BoostUniqueLock lock(_mtx_orders);
				_orders->add(localid, orderInfo);
			}

			double& curQty = _undone_qty[stdCode];
			curQty += orderInfo->getVolLeft()*(isBuy ? 1 : -1);
		}

		for (auto it = _undone_qty.begin(); it != _undone_qty.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const double& curQty = _undone_qty[stdCode];

			WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ%sδ�������%f", _id.c_str(), stdCode, curQty);
		}
	}

	if (_state == AS_POSITION_QRYED)
	{
		_state = AS_ORDERS_QRYED;

		_trader_api->queryTrades();
	}
}

void TraderAdapter::printPosition(const char* code, const PosItem& pItem)
{
	//WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]%s�ֲָ���, ��:%f[%f]|%f[%f], ��:%f[%f]|%f[%f]", 
	//	_id.c_str(), code, pItem.l_prevol, pItem.l_preavail, pItem.l_newvol, pItem.l_newavail, 
	//	pItem.s_prevol, pItem.s_preavail, pItem.s_newvol, pItem.s_newavail);

	StreamLogger(LL_INFO, _id.c_str(), "trader").self() << "[" << _id << "]" << code << "�ֲָ���, ��:" << pItem.l_prevol << "[" << pItem.l_preavail << "]|" << pItem.l_newvol
		<< "[" << pItem.l_newavail << "], ��:" << pItem.s_prevol << "[" << pItem.s_preavail << "]|" << pItem.s_newvol << "[" << pItem.s_newavail << "]";
}

void TraderAdapter::onRspTrades(const WTSArray* ayTrades)
{
	if (ayTrades)
	{
		for (auto it = ayTrades->begin(); it != ayTrades->end(); it++)
		{
			WTSTradeInfo* tInfo = (WTSTradeInfo*)(*it);

			WTSContractInfo* cInfo = _bd_mgr->getContract(tInfo->getCode());
			if (cInfo == NULL)
				continue;

			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
			std::string stdCode;
			if (commInfo->getCategoty() == CC_Future)
				stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
			else
				stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

			WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
			if(statInfo == NULL)
			{
				statInfo = WTSTradeStateInfo::create(stdCode.c_str());
				_stat_map->add(stdCode, statInfo, false);
			}
			TradeStatInfo& statItem = statInfo->statInfo();

			bool isLong = (tInfo->getDirection() == WDT_LONG);
			bool isOpen = (tInfo->getOffsetType() == WOT_OPEN);
			bool isCloseT = (tInfo->getOffsetType() == WOT_CLOSETODAY);
			double qty = tInfo->getVolumn();

			if (isLong)
			{
				if (isOpen)
					statItem.l_openvol += qty;
				else if (isCloseT)
					statItem.l_closetvol += qty;
				else
					statItem.l_closevol += qty;
			}
			else
			{
				if (isOpen)
					statItem.s_openvol += qty;
				else if (isCloseT)
					statItem.s_closetvol += qty;
				else
					statItem.s_closevol += qty;
			}
		}

		for (auto it = _stat_map->begin(); it != _stat_map->end(); it++)
		{
			const char* stdCode = it->first.c_str();
			WTSTradeStateInfo* pItem = (WTSTradeStateInfo*)it->second;
			//WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ%s��ƽͳ�Ƹ���, ����%f��, ƽ��%u��, ƽ���%f��, ����%f��, ƽ��%f��, ƽ���%f��",
			//	_id.c_str(), stdCode, pItem->open_volumn_long(), pItem->close_volumn_long(), pItem->closet_volumn_long(), 
			//	pItem->open_volumn_short(), pItem->close_volumn_short(), pItem->closet_volumn_short());

			StreamLogger(LL_INFO, _id.c_str(), "trader").self() << "[" << _id << "]" << stdCode << "��ƽͳ�Ƹ���, ����" << pItem->open_volumn_long() 
				<< "��, ƽ��" << pItem->close_volumn_long() << "��, ƽ���" << pItem->closet_volumn_long() << "��, ����" << pItem->open_volumn_short()
				<< "��, ƽ��" << pItem->close_volumn_short() << "��, ƽ���" << pItem->closet_volumn_short() << "��";
		}
	}

	if (_state == AS_ORDERS_QRYED)
	{
		_state = AS_ALLREADY;

		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]����ͨ��ȫ������", _id.c_str());
		for (auto sink : _sinks)
			sink->on_channel_ready();
	}
}

inline const char* stateToName(WTSOrderState woState)
{
	if (woState == WOS_AllTraded)
		return "ȫ���ɽ�";
	else if (woState == WOS_PartTraded_NotQueuing || woState == WOS_PartTraded_Queuing)
		return "���ֳɽ�";
	else if (woState == WOS_NotTraded_NotQueuing || woState == WOS_NotTraded_Queuing)
		return "δ�ɽ�";
	else if (woState == WOS_Canceled)
		return "�ѳ���";
	else if (woState == WOS_Submitting)
		return "�ύ��";
	else if (woState == WOS_Nottouched)
		return "δ�ύ";
	else
		return "��";
}

void TraderAdapter::onPushOrder(WTSOrderInfo* orderInfo)
{
	if (orderInfo == NULL)
		return;


	WTSContractInfo* cInfo = _bd_mgr->getContract(orderInfo->getCode());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
	std::string stdCode;
	if (commInfo->getCategoty() == CC_Future)
		stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	else
		stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

	bool isBuy = (orderInfo->getDirection() == WDT_LONG && orderInfo->getOffsetType() == WOT_OPEN) || (orderInfo->getDirection() == WDT_SHORT && orderInfo->getOffsetType() != WOT_OPEN);
	
	if (orderInfo->getOrderState() == WOS_Canceled)
	{
		//PosItem& pItem = _positions[stdCode];
		//pItem.cancel_cnt++;
		WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
		if (statInfo == NULL)
		{
			statInfo = WTSTradeStateInfo::create(stdCode.c_str());
			_stat_map->add(stdCode, statInfo, false);
		}
		TradeStatInfo& statItem = statInfo->statInfo();
		if(isBuy)
		{
			if (orderInfo->isError())//��Ҫ�ͳ������ֿ�
			{
				statItem.b_wrongs++;
				statItem.b_wrongqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
			}
			else
			{
				statItem.b_cancels++;
				statItem.b_canclqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
			}
		}
		else
		{
			if (orderInfo->isError())//��Ҫ�ͳ������ֿ�
			{
				statItem.s_wrongs++;
				statItem.s_wrongqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
			}
			else
			{
				statItem.s_cancels++;
				statItem.s_canclqty += orderInfo->getVolumn() - orderInfo->getVolTraded();
			}
		}
	}

	WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]�յ������ر�, ��Լ %s, �û���� %s, ״̬ %s", _id.c_str(), stdCode.c_str(), orderInfo->getUserTag(), stateToName(orderInfo->getOrderState()));

	//�����������, �����Լ��Ķ���, ��Ҫ�ȸ���δ�������
	if (orderInfo->getOrderState() == WOS_Canceled && StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern, false))
	{
		//������ʱ��, Ҫ����δ���
		bool isLong = (orderInfo->getDirection() == WDT_LONG);
		bool isOpen = (orderInfo->getOffsetType() == WOT_OPEN);
		bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
		double qty = orderInfo->getVolLeft();

		bool isBuy = (isLong&&isOpen) || (!isLong&&!isOpen);
		double oldQty = _undone_qty[stdCode];
		double newQty = oldQty - qty*(isBuy ? 1 : -1);
		_undone_qty[stdCode] = newQty;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ %s δ��ɶ�����������, %f -> %f", _id.c_str(), stdCode.c_str(), oldQty, newQty);

		//WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]�µ�ʧ��: %s, ��Լ: %s, ����: %s, ����: %d", _id.c_str(), stdCode.c_str(), err->getMessage(), entrust->getCode(), action.c_str(), qty);
		std::string action;
		if (isOpen)
			action = "��";
		else if (isToday)
			action = "ƽ��";
		else
			action = "ƽ";
		action += isLong ? "��" : "��";

		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ%s����%s�ѳ���:%s, ����: %s, ʣ������: %f", 
			_id.c_str(), stdCode.c_str(), orderInfo->getUserTag(), orderInfo->getStateMsg(), action.c_str(), qty);
	}

	//�ȼ��ö����ǲ��ǵ�һ�����͹���
	//����ǵ�һ�����͹���, ��Ҫ���ݿ�ƽ���¿�ƽ
	if (strlen(orderInfo->getOrderID()) > 0)
	{
		auto it = _orderids.find(orderInfo->getOrderID());
		if (it == _orderids.end())
		{
			//�ȰѶ����Ż�������, ��ֹ�ظ�����
			_orderids.insert(orderInfo->getOrderID());

			WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
			if (statInfo == NULL)
			{
				statInfo = WTSTradeStateInfo::create(stdCode.c_str());
				_stat_map->add(stdCode, statInfo, false);
			}
			TradeStatInfo& statItem = statInfo->statInfo();
			if (isBuy)
			{
				statItem.b_orders++;
				statItem.b_ordqty += orderInfo->getVolumn();
			}
			else
			{
				statItem.s_orders++;
				statItem.s_ordqty += orderInfo->getVolumn();
			}

			//ֻ��ƽ����Ҫ���¿�ƽ
			if (orderInfo->getOffsetType() != WOT_OPEN)
			{
				//const char* code = stdCode.c_str();
				bool isLong = (orderInfo->getDirection() == WDT_LONG);
				bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
				double qty = orderInfo->getVolumn();

				PosItem& pItem = _positions[stdCode];
				if (isLong)	//ƽ��
				{
					if (isToday)
					{
						pItem.l_newavail -= min(pItem.l_newavail, qty);	//�����ƽ��, ��ֻ��Ҫ���¿�ƽ���
					}
					else
					{
						double left = qty;

						//�����ƽ��, ���ȸ��¿�ƽ���, ����ʣ��, �ٸ��¿�ƽ���
						//���Ʒ������ƽ��ƽ��, Ҳ�����������, ��Ϊƽ��������������ܳ������
						double maxQty = min(pItem.l_preavail, qty);
						pItem.l_preavail -= maxQty;
						left -= maxQty;

						if (left > 0)
							pItem.l_newavail -= min(pItem.l_newavail, left);
					}
				}
				else //ƽ��
				{
					if (isToday)
					{
						pItem.s_newavail -= min(pItem.s_newavail, qty);
					}
					else
					{
						double left = qty;

						double maxQty = min(pItem.s_preavail, qty);
						pItem.s_preavail -= maxQty;
						left -= maxQty;

						if (left > 0)
							pItem.s_newavail -= min(pItem.s_newavail, left);
					}
				}
				printPosition(stdCode.c_str(), pItem);
			}
		}
		else if (orderInfo->getOrderState() == WOS_Canceled && orderInfo->getOffsetType() != WOT_OPEN)
		{
			//����������ǵ�һ������, �ҳ�����, ��Ҫ���¿�ƽ��
			//const char* code = orderInfo->getCode();
			bool isLong = (orderInfo->getDirection() == WDT_LONG);
			bool isToday = (orderInfo->getOffsetType() == WOT_CLOSETODAY);
			double qty = orderInfo->getVolumn() - orderInfo->getVolTraded();

			PosItem& pItem = _positions[stdCode];
			if (isLong)	//ƽ��
			{
				if (isToday)
				{
					pItem.l_newavail += qty;	//�����ƽ��, ��ֻ��Ҫ���¿�ƽ���
				}
				else
				{
					pItem.l_preavail += qty;
					if (pItem.l_preavail > pItem.l_prevol)
					{
						pItem.l_newavail += (pItem.l_preavail - pItem.l_prevol);
						pItem.l_preavail = pItem.l_prevol;
					}
				}
			}
			else //ƽ��
			{
				if (isToday)
				{
					pItem.s_newavail += qty;
				}
				else
				{
					pItem.s_preavail += qty;
					if (pItem.s_preavail > pItem.s_prevol)
					{
						pItem.s_newavail += (pItem.s_preavail - pItem.s_prevol);
						pItem.s_preavail = pItem.s_prevol;
					}
				}
			}
			printPosition(stdCode.c_str(), pItem);
		}
	}

	//�ȿ����ǲ���otp����ȥ�ĵ���
	if (!StrUtil::startsWith(orderInfo->getUserTag(), _order_pattern))
		return;

	char* userTag = (char*)orderInfo->getUserTag();
	userTag += _order_pattern.size() + 1;
	uint32_t localid = strtoul(userTag, NULL, 10);

	{
		BoostUniqueLock lock(_mtx_orders);
		if (!orderInfo->isAlive() && _orders)
		{
			_orders->remove(localid);
		}
		else
		{
			if (_orders == NULL)
				_orders = OrderMap::create();

			_orders->add(localid, orderInfo);
		}
	}
	

	//ִ֪ͨ����
	for (auto sink : _sinks)
		sink->on_order(localid, stdCode.c_str(), isBuy, orderInfo->getVolumn(), orderInfo->getVolLeft(), orderInfo->getPrice(), orderInfo->getOrderState() == WOS_Canceled);
}

void TraderAdapter::onPushTrade(WTSTradeInfo* tradeRecord)
{
	WTSContractInfo* cInfo = _bd_mgr->getContract(tradeRecord->getCode());
	if (cInfo == NULL)
		return;

	bool isLong = (tradeRecord->getDirection() == WDT_LONG);
	bool isOpen = (tradeRecord->getOffsetType() == WOT_OPEN);
	bool isBuy = (tradeRecord->getDirection() == WDT_LONG && tradeRecord->getOffsetType() == WOT_OPEN) || (tradeRecord->getDirection() == WDT_SHORT && tradeRecord->getOffsetType() != WOT_OPEN);

	WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(cInfo);
	std::string stdCode;
	if (commInfo->getCategoty() == CC_Future)
		stdCode = CodeHelper::bscFutCodeToStdCode(cInfo->getCode(), cInfo->getExchg());
	else
		stdCode = CodeHelper::bscStkCodeToStdCode(cInfo->getCode(), cInfo->getExchg());

	if (StrUtil::startsWith(tradeRecord->getUserTag(), _order_pattern, false))
	{
		double oldQty = _undone_qty[stdCode];
		double newQty = oldQty - tradeRecord->getVolumn()*(isBuy ? 1 : -1);
		_undone_qty[stdCode] = newQty;
		WTSLogger::log_dyn("trader", _id.c_str(), LL_INFO,"[%s]��Լ %s δ��ɶ�����������, %f -> %f", _id.c_str(), stdCode.c_str(), oldQty, newQty);
	}

	PosItem& pItem = _positions[stdCode];
	WTSTradeStateInfo* statInfo = (WTSTradeStateInfo*)_stat_map->get(stdCode.c_str());
	if (statInfo == NULL)
	{
		statInfo = WTSTradeStateInfo::create(stdCode.c_str());
		_stat_map->add(stdCode, statInfo, false);
	}
	TradeStatInfo& statItem = statInfo->statInfo();
	double vol = tradeRecord->getVolumn();
	if(isLong)
	{
		if (isOpen)
		{
			pItem.l_newvol += vol;
			pItem.l_newavail += vol;

			statItem.l_openvol += vol;
		}
		else if (tradeRecord->getOffsetType() == WOT_CLOSETODAY)
		{
			pItem.l_newvol -= vol;

			statItem.l_closevol += vol;
		}
		else
		{
			double left = vol;
			double maxVol = min(left, pItem.l_prevol);
			pItem.l_prevol -= maxVol;
			left -= maxVol;
			pItem.l_newvol -= left;

			statItem.l_closevol += vol;
		}
	}
	else
	{
		if (isOpen)
		{
			pItem.s_newvol += vol;
			pItem.s_newavail += vol;

			statItem.s_openvol += vol;
		}
		else if (tradeRecord->getOffsetType() == WOT_CLOSETODAY)
		{
			pItem.s_newvol -= vol;

			statItem.s_closevol += vol;
		}
		else
		{
			double left = vol;
			double maxVol = min(left, pItem.s_prevol);
			pItem.s_prevol -= maxVol;
			left -= maxVol;
			pItem.s_newvol -= left;

			statItem.s_closevol += vol;
		}
	}

	printPosition(stdCode.c_str(), pItem);

	//updateUndone();

	for (auto sink : _sinks)
		sink->on_trade(stdCode.c_str(), isBuy, vol, tradeRecord->getPrice());
}

void TraderAdapter::onTraderError(WTSError* err)
{
	if(err)
		WTSLogger::log_dyn("trader", _id.c_str(), LL_ERROR,"[%s]����ͨ�����ִ���: %s", _id.c_str(), err->getMessage());
}

IBaseDataMgr* TraderAdapter::getBaseDataMgr()
{
	return _bd_mgr;
}

void TraderAdapter::handleTraderLog(WTSLogLevel ll, const char* format, ...)
{
	char szBuf[2048] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _id.c_str());
	strcat(szBuf, format);
	va_list args;
	va_start(args, format);
	//WTSLogger::log2_direct("trader", ll, szBuf, args);
	WTSLogger::log_dyn_direct("trader", _id.c_str(), ll, szBuf, args);
	va_end(args);
}

#pragma endregion "ITraderApiListener�ӿ�"


//////////////////////////////////////////////////////////////////////////
//CTPWrapperMgr
bool TraderAdapterMgr::addAdapter(const char* tname, TraderAdapterPtr& adapter)
{
	if (adapter == NULL || strlen(tname) == 0)
		return false;

	auto it = _adapters.find(tname);
	if(it != _adapters.end())
	{
		WTSLogger::error("����ͨ��������ͬ: %s", tname);
		return false;
	}

	_adapters[tname] = adapter;

	return true;
}

TraderAdapterPtr TraderAdapterMgr::getAdapter(const char* tname)
{
	auto it = _adapters.find(tname);
	if (it != _adapters.end())
	{
		return it->second;
	}

	return TraderAdapterPtr();
}

void TraderAdapterMgr::run()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->run();
	}

	WTSLogger::info("%u������ͨ��������", _adapters.size());
}

void TraderAdapterMgr::release()
{
	for (auto it = _adapters.begin(); it != _adapters.end(); it++)
	{
		it->second->release();
	}

	_adapters.clear();
}