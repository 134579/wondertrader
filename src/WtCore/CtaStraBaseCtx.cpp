/*!
 * \file CtaStraBaseCtx.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaStraBaseCtx.h"
#include "WtCtaEngine.h"
#include "WtHelper.h"

#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"

#include "../WTSTools/WTSLogger.h"

const char* CMP_ALG_NAMES[] =
{
	"＝",
	"＞",
	"＜",
	"≥",
	"≤"
};

const char* ACTION_NAMES[] =
{
	"开多",
	"平多",
	"开空",
	"平空",
	"同步"
};


inline uint32_t makeCtaCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 1 };
	return _auto_context_id.fetch_add(1);
}


CtaStraBaseCtx::CtaStraBaseCtx(WtCtaEngine* engine, const char* name)
	: ICtaStraCtx(name)
	, _engine(engine)
	, _total_calc_time(0)
	, _emit_times(0)
	, _last_cond_min(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
{
	_context_id = makeCtaCtxId();
}


CtaStraBaseCtx::~CtaStraBaseCtx()
{
}

void CtaStraBaseCtx::init_outputs()
{
	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "//";
	BoostFile::create_directories(folder.c_str());	

	std::string filename = folder + "trades.csv";
	_trade_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_trade_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_trade_logs->write_file("code,time,direct,action,price,qty,tag,fee\n");
		}
		else
		{
			_trade_logs->seek_to_end();
		}
	}

	filename = folder + "closes.csv";
	_close_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_close_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_close_logs->write_file("code,direct,opentime,openprice,closetime,closeprice,qty,profit,totalprofit,entertag,exittag\n");
		}
		else
		{
			_close_logs->seek_to_end();
		}
	}

	filename = folder + "funds.csv";
	_fund_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_fund_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_fund_logs->write_file("date,closeprofit,positionprofit,dynbalance,fee\n");
		}
		else
		{
			_fund_logs->seek_to_end();
		}
	}

	filename = folder + "signals.csv";
	_sig_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_sig_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_sig_logs->write_file("code,target,sigprice,gentime,usertag\n");
		}
		else
		{
			_sig_logs->seek_to_end();
		}
	}
}

void CtaStraBaseCtx::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	if (_sig_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
		_sig_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag, double fee)
{
	if (_trade_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") << "," << price << "," << qty << "," << userTag << "," << fee << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
	double profit, double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */)
{
	if (_close_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," 
			<< totalprofit << "," << enterTag << "," << exitTag << "\n";
		_close_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::save_userdata()
{
	rj::Document root(rj::kObjectType);
	rj::Document::AllocatorType &allocator = root.GetAllocator();
	for (auto it = _user_datas.begin(); it != _user_datas.end(); it++)
	{
		root.AddMember(rj::Value(it->first.c_str(), allocator), rj::Value(it->second.c_str(), allocator), allocator);
	}

	{
		std::string filename = WtHelper::getStraUsrDatDir();
		filename += "ud_";
		filename += _name;
		filename += ".json";

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			rj::StringBuffer sb;
			rj::PrettyWriter<rj::StringBuffer> writer(sb);
			root.Accept(writer);
			bf.write_file(sb.GetString());
			bf.close_file();
		}
	}
}

void CtaStraBaseCtx::load_userdata()
{
	std::string filename = WtHelper::getStraUsrDatDir();
	filename += "ud_";
	filename += _name;
	filename += ".json";

	if (!StdFile::exists(filename.c_str()))
	{
		return;
	}

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.empty())
		return;

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
		return;

	for (auto& m : root.GetObject())
	{
		const char* key = m.name.GetString();
		const char* val = m.value.GetString();
		_user_datas[key] = val;
	}
}

void CtaStraBaseCtx::load_data(uint32_t flag /* = 0xFFFFFFFF */)
{
	std::string filename = WtHelper::getStraDataDir();
	filename += _name;
	filename += ".json";
	
	if (!StdFile::exists(filename.c_str()))
	{
		return;
	}

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.empty())
		return;

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
		return;

	if(root.HasMember("fund"))
	{
		//读取资金
		const rj::Value& jFund = root["fund"];
		if(!jFund.IsNull() && jFund.IsObject())
		{
			_fund_info._total_profit = jFund["total_profit"].GetDouble();
			_fund_info._total_dynprofit = jFund["total_dynprofit"].GetDouble();
			uint32_t tdate = jFund["tdate"].GetUint();
			_fund_info._total_fees = jFund["total_fees"].GetDouble();
		}
	}

	{//读取仓位
		double total_profit = 0;
		double total_dynprofit = 0;
		const rj::Value& jPos = root["positions"];
		if (!jPos.IsNull() && jPos.IsArray())
		{
			for (const rj::Value& pItem : jPos.GetArray())
			{
				const char* stdCode = pItem["code"].GetString();
				if (!CodeHelper::isStdFutHotCode(stdCode) && _engine->get_contract_info(stdCode) == NULL)
				{
					stra_log_text("合约%s不存在或者已过期，持仓数据已忽略", stdCode);
					continue;
				}
				PosInfo& pInfo = _pos_map[stdCode];
				pInfo._closeprofit = pItem["closeprofit"].GetDouble();
				pInfo._volumn = pItem["volumn"].GetDouble();
				if (pInfo._volumn == 0)
					pInfo._dynprofit = 0;
				else
					pInfo._dynprofit = pItem["dynprofit"].GetDouble();

				total_profit += pInfo._closeprofit;
				total_dynprofit += pInfo._dynprofit;

				const rj::Value& details = pItem["details"];
				if (details.IsNull() || !details.IsArray() || details.Size() == 0)
					continue;

				pInfo._details.resize(details.Size());

				for (uint32_t i = 0; i < details.Size(); i++)
				{
					const rj::Value& dItem = details[i];
					DetailInfo& dInfo = pInfo._details[i];
					dInfo._long = dItem["long"].GetBool();
					dInfo._price = dItem["price"].GetDouble();
					dInfo._volumn = dItem["volumn"].GetDouble();
					dInfo._opentime = dItem["opentime"].GetUint64();
					if(dItem.HasMember("opentdate"))
						dInfo._opentdate = dItem["opentdate"].GetUint();

					dInfo._profit = dItem["profit"].GetDouble();
					dInfo._max_profit = dItem["maxprofit"].GetDouble();
					dInfo._max_loss = dItem["maxloss"].GetDouble();

					strcpy(dInfo._opentag, dItem["opentag"].GetString());
				}

				stra_log_text("策略仓位确认,%s -> %d", stdCode, pInfo._volumn);
			}
		}

		_fund_info._total_profit = total_profit;
		_fund_info._total_dynprofit = total_dynprofit;
	}

	{//读取条件单
		uint32_t count = 0;
		const rj::Value& jCond = root["conditions"];
		if (!jCond.IsNull() && jCond.IsObject())
		{
			_last_cond_min = jCond["settime"].GetUint64();
			const rj::Value& jItems = jCond["items"];
			for (auto& m : jItems.GetObject())
			{
				const char* stdCode = m.name.GetString();
				if (!CodeHelper::isStdFutHotCode(stdCode) && _engine->get_contract_info(stdCode) == NULL)
				{
					stra_log_text("合约%s不存在或者已过期，条件单已忽略", stdCode);
					continue;
				}

				const rj::Value& cListItem = m.value;

				CondList& condList = _condtions[stdCode];

				for(auto& cItem : cListItem.GetArray())
				{
					CondEntrust condInfo;
					strcpy(condInfo._code, stdCode);
					strcpy(condInfo._usertag, cItem["usertag"].GetString());

					condInfo._field = (WTSCompareField)cItem["field"].GetUint();
					condInfo._alg = (WTSCompareType)cItem["alg"].GetUint();
					condInfo._target = cItem["target"].GetDouble();
					condInfo._qty = cItem["qty"].GetDouble();
					condInfo._action = (char)cItem["action"].GetUint();

					condList.push_back(condInfo);

					stra_log_text(fmt::format("条件单恢复, 合约: {}, {} {}, 触发条件: 最新价 {} {}",
						stdCode, ACTION_NAMES[condInfo._action], condInfo._qty, CMP_ALG_NAMES[condInfo._alg], condInfo._target).c_str());
					count++;
				}
			}

			stra_log_text(fmt::format("条件单共恢复 {} 条, 设置时间为 {}", count, _last_cond_min).c_str());
		}
	}

	if (root.HasMember("signals"))
	{
		//读取信号
		const rj::Value& jSignals = root["signals"];
		if (!jSignals.IsNull() && jSignals.IsObject())
		{
			for (auto& m : jSignals.GetObject())
			{
				const char* stdCode = m.name.GetString();
				if (!CodeHelper::isStdFutHotCode(stdCode) && _engine->get_contract_info(stdCode) == NULL)
				{
					stra_log_text("合约%s不存在或者已过期，信号已忽略", stdCode);
					continue;
				}

				const rj::Value& jItem = m.value;

				SigInfo& sInfo = _sig_map[stdCode];
				sInfo._usertag = jItem["usertag"].GetString();
				sInfo._volumn = jItem["volumn"].GetDouble();
				sInfo._sigprice = jItem["sigprice"].GetDouble();
				sInfo._gentime = jItem["gentime"].GetUint64();
				
				stra_log_text("未触发信号恢复, 合约: %s, 目标部位: %d", stdCode, sInfo._volumn);
			}
		}
	}
}

void CtaStraBaseCtx::save_data(uint32_t flag /* = 0xFFFFFFFF */)
{
	rj::Document root(rj::kObjectType);

	{//持仓数据保存
		rj::Value jPos(rj::kArrayType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosInfo& pInfo = it->second;

			rj::Value pItem(rj::kObjectType);
			pItem.AddMember("code", rj::Value(stdCode, allocator), allocator);
			pItem.AddMember("volumn", pInfo._volumn, allocator);
			pItem.AddMember("closeprofit", pInfo._closeprofit, allocator);
			pItem.AddMember("dynprofit", pInfo._dynprofit, allocator);

			rj::Value details(rj::kArrayType);
			for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
			{
				const DetailInfo& dInfo = *dit;
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("long", dInfo._long, allocator);
				dItem.AddMember("price", dInfo._price, allocator);
				dItem.AddMember("volumn", dInfo._volumn, allocator);
				dItem.AddMember("opentime", dInfo._opentime, allocator);
				dItem.AddMember("opentdate", dInfo._opentdate, allocator);

				dItem.AddMember("profit", dInfo._profit, allocator);
				dItem.AddMember("maxprofit", dInfo._max_profit, allocator);
				dItem.AddMember("maxloss", dInfo._max_loss, allocator);
				dItem.AddMember("opentag", rj::Value(dInfo._opentag, allocator), allocator);

				details.PushBack(dItem, allocator);
			}

			pItem.AddMember("details", details, allocator);

			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	{//资金保存
		rj::Value jFund(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		jFund.AddMember("total_profit", _fund_info._total_profit, allocator);
		jFund.AddMember("total_dynprofit", _fund_info._total_dynprofit, allocator);
		jFund.AddMember("total_fees", _fund_info._total_fees, allocator);
		jFund.AddMember("tdate", _engine->get_trading_date(), allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//信号保存
		rj::Value jSigs(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it:_sig_map)
		{
			const char* stdCode = it.first.c_str();
			const SigInfo& sInfo = it.second;

			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("usertag", rj::Value(sInfo._usertag.c_str(), allocator), allocator);

			jItem.AddMember("volumn", sInfo._volumn, allocator);
			jItem.AddMember("sigprice", sInfo._sigprice, allocator);
			jItem.AddMember("gentime", sInfo._gentime, allocator);

			jSigs.AddMember(rj::Value(stdCode, allocator), jItem, allocator);
		}

		root.AddMember("signals", jSigs, allocator);
	}

	{//条件单保存
		rj::Value jCond(rj::kObjectType);
		rj::Value jItems(rj::kObjectType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _condtions.begin(); it != _condtions.end(); it++)
		{
			const char* code = it->first.c_str();
			const CondList& condList = it->second;

			rj::Value cArray(rj::kArrayType);
			for(auto& condInfo : condList)
			{
				rj::Value cItem(rj::kObjectType);
				cItem.AddMember("code", rj::Value(code, allocator), allocator);
				cItem.AddMember("usertag", rj::Value(condInfo._usertag, allocator), allocator);

				cItem.AddMember("field", (uint32_t)condInfo._field, allocator);
				cItem.AddMember("alg", (uint32_t)condInfo._alg, allocator);
				cItem.AddMember("target", condInfo._target, allocator);
				cItem.AddMember("qty", condInfo._qty, allocator);
				cItem.AddMember("action", (uint32_t)condInfo._action, allocator);

				cArray.PushBack(cItem, allocator);
			}

			jItems.AddMember(rj::Value(code, allocator), cArray, allocator);
		}
		jCond.AddMember("settime", _last_cond_min, allocator);
		jCond.AddMember("items", jItems, allocator);

		root.AddMember("conditions", jCond, allocator);
	}

	{
		std::string filename = WtHelper::getStraDataDir();
		filename += _name;
		filename += ".json";

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			rj::StringBuffer sb;
			rj::PrettyWriter<rj::StringBuffer> writer(sb);
			root.Accept(writer);
			bf.write_file(sb.GetString());
			bf.close_file();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//回调函数
void CtaStraBaseCtx::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	std::string realPeriod;
	if (period[0] == 'd')
		realPeriod = StrUtil::printf("%s%u", period, times);
	else
		realPeriod = StrUtil::printf("m%u", times);

	std::string key = StrUtil::printf("%s#%s", stdCode, realPeriod.c_str());
	KlineTag& tag = _kline_tags[key];
	tag._closed = true;

	on_bar_close(stdCode, realPeriod.c_str(), newBar);
}

void CtaStraBaseCtx::on_init()
{
	init_outputs();

	//读取数据
	load_data();

	//加载用户数据
	load_userdata();
}

void CtaStraBaseCtx::update_dyn_profit(const char* stdCode, double price)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		PosInfo& pInfo = it->second;
		if (pInfo._volumn == 0)
		{
			pInfo._dynprofit = 0;
		}
		else
		{
			WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volumn*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}

	double total_dynprofit = 0;
	for(auto v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		total_dynprofit += pInfo._dynprofit;
	}

	_fund_info._total_dynprofit = total_dynprofit;
}

void CtaStraBaseCtx::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	_price_map[stdCode] = newTick->price();

	//先检查是否要信号要触发
	{
		auto it = _sig_map.find(stdCode);
		if(it != _sig_map.end())
		{
			WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

			if (sInfo->isInTradingTime(_engine->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				do_set_position(stdCode, sInfo._volumn, sInfo._usertag.c_str(), sInfo._triggered);
				_sig_map.erase(it);
			}
			
		}
	}

	//更新浮动盈亏
	update_dyn_profit(stdCode, newTick->price());

	//////////////////////////////////////////////////////////////////////////
	//检查条件单
	if(!_condtions.empty())
	{
		auto it = _condtions.find(stdCode);
		if (it == _condtions.end())
			return;

		const CondList& condList = it->second;
		for (const CondEntrust& entrust : condList)
		{
			double curPrice = newTick->price();

			bool isMatched = false;
			switch (entrust._alg)
			{
			case WCT_Equal:
				isMatched = decimal::eq(curPrice, entrust._target);
				break;
			case WCT_Larger:
				isMatched = decimal::gt(curPrice, entrust._target);
				break;
			case WCT_LargerOrEqual:
				isMatched = decimal::ge(curPrice, entrust._target);
				break;
			case WCT_Smaller:
				isMatched = decimal::lt(curPrice, entrust._target);
				break;
			case WCT_SmallerOrEqual:
				isMatched = decimal::le(curPrice, entrust._target);
				break;
			default:
				break;
			}

			if (isMatched)
			{
				stra_log_text(fmt::format("条件单触发[最新价: {}{}{}], 合约: {}, {} {}", curPrice, CMP_ALG_NAMES[entrust._alg], entrust._target, stdCode, ACTION_NAMES[entrust._action], entrust._qty).c_str());

				switch (entrust._action)
				{
				case COND_ACTION_OL:
				{
					double curQty = stra_get_position(stdCode);
					double desQty = 0;
					if (decimal::lt(curQty, 0))
						desQty = entrust._qty;
					else
						desQty = curQty + entrust._qty;

					//uint64_t sigTime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
					//log_signal(stdCode, desQty, curPrice, sigTime, entrust._usertag);						
					//do_set_position(stdCode, desQty, entrust._usertag, true);
					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_CL:
				{
					double curQty = stra_get_position(stdCode);
					if (decimal::le(curQty, 0))
						return;

					double maxQty = min(curQty, entrust._qty);
					double desQty = curQty - maxQty;

					//uint64_t sigTime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
					//log_signal(stdCode, desQty, curPrice, sigTime, entrust._usertag);
					//do_set_position(stdCode, desQty, entrust._usertag, true);
					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_OS:
				{
					double curQty = stra_get_position(stdCode);
					double desQty = 0;
					if (decimal::gt(curQty, 0))
						desQty = -entrust._qty;
					else
						desQty = curQty - entrust._qty;

					//uint64_t sigTime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
					//log_signal(stdCode, desQty, curPrice, sigTime, entrust._usertag);
					//do_set_position(stdCode, desQty, entrust._usertag, true);
					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_CS:
				{
					double curQty = stra_get_position(stdCode);
					if (decimal::ge(curQty, 0))
						return;

					double maxQty = min(abs(curQty), entrust._qty);
					double desQty = curQty + maxQty;

					//uint64_t sigTime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
					//log_signal(stdCode, desQty, curPrice, sigTime, entrust._usertag);
					//do_set_position(stdCode, desQty, entrust._usertag, true);
					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_SP: 
				{
					//uint64_t sigTime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
					//log_signal(stdCode, entrust._qty, curPrice, sigTime, entrust._usertag);
					//do_set_position(stdCode, entrust._qty, entrust._usertag, true);
					append_signal(stdCode, entrust._qty, entrust._usertag);
				}
				break;
				default: break;
				}

				//同一个bar设置针对同一个合约的条件单, 只可能触发一条
				//所以这里直接清理掉即可
				_condtions.erase(it);
				break;
			}
		}
	}

	if (bEmitStrategy)
		on_tick_updated(stdCode, newTick);

	if(_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

bool CtaStraBaseCtx::on_schedule(uint32_t curDate, uint32_t curTime)
{
	_is_in_schedule = true;//开始调度, 修改标记

	//主要用于保存浮动盈亏的
	save_data();

	bool isMainUdt = false;
	bool emmited = false;

	for (auto it = _kline_tags.begin(); it != _kline_tags.end(); it++)
	{
		const std::string& key = it->first;
		KlineTag& marker = it->second;

		StringVector ay = StrUtil::split(key, "#");
		const char* stdCode = ay[0].c_str();

		if (key == _main_key)
		{
			if (marker._closed)
			{
				isMainUdt = true;
				marker._closed = false;
			}
			else
			{
				isMainUdt = false;
				break;
			}
		}

		WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

		if (isMainUdt || _kline_tags.empty())
		{	
			TimeUtils::Ticker ticker;

			uint32_t offTime = sInfo->offsetTime(curTime);
			if(offTime <= sInfo->getCloseTime(true))
			{
				_condtions.clear();
				on_mainkline_updated(curDate, curTime);
				stra_log_text("策略重算已触发 @ %u", curTime);
				emmited = true;

				_emit_times++;
				_total_calc_time += ticker.micro_seconds();

				if (_emit_times % 20 == 0)
				{
					stra_log_text(fmt::format("策略共触发{}次, 共耗时{}微秒, 平均耗时{}微秒",
						_emit_times, _total_calc_time, _total_calc_time / _emit_times).c_str());
				}

				if (_ud_modified)
				{
					save_userdata();
					_ud_modified = false;
				}

				if(!_condtions.empty())
				{
					_last_cond_min = (uint64_t)curDate * 10000 + curTime;
					save_data();
				}
			}
			else
			{
				stra_log_text("%u 不在交易时间, 策略重算取消", curTime);
			}
			break;
		}
	}

	_is_in_schedule = false;//调度结束, 修改标记
	return emmited;
}

void CtaStraBaseCtx::on_session_begin()
{

}

void CtaStraBaseCtx::enum_position(FuncEnumPositionCallBack cb)
{
	std::unordered_map<std::string, double> desPos;
	for (auto it:_pos_map)
	{
		const char* stdCode = it.first.c_str();
		const PosInfo& pInfo = it.second;
		//cb(stdCode, pInfo._volumn);
		desPos[stdCode] = pInfo._volumn;
	}

	for (auto sit:_sig_map)
	{
		const char* stdCode = sit.first.c_str();
		const SigInfo& sInfo = sit.second;
		desPos[stdCode] = sInfo._volumn;
	}

	for(auto v:desPos)
	{
		cb(v.first.c_str(), v.second);
	}
}

void CtaStraBaseCtx::on_session_end()
{
	uint32_t curDate = _engine->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;
	}

	//这里要把当日结算的数据写到日志文件里
	//而且这里回测和实盘写法不同, 先留着, 后面来做
	if (_fund_logs)
		_fund_logs->write_file(StrUtil::printf("%d,%.2f,%.2f,%.2f,%.2f\n", curDate, 
		_fund_info._total_profit, _fund_info._total_dynprofit, 
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees));

	save_data();
}

CondList& CtaStraBaseCtx::get_cond_entrusts(const char* stdCode)
{
	CondList& ce = _condtions[stdCode];
	return ce;
}

//////////////////////////////////////////////////////////////////////////
//策略接口
void CtaStraBaseCtx::stra_enter_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式, 则直接触发
	{
		double curQty = stra_get_position(stdCode);
		if (decimal::lt(curQty, 0))
		{
			//当前持仓小于0，逻辑是反手到qty，所以设置信号目标仓位为qty
			append_signal(stdCode, qty, userTag);
		}
		else
		{
			//当前持仓大于等于0，则要增加多仓qty
			append_signal(stdCode, curQty + qty, userTag);
		}
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if(!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		
		entrust._action = COND_ACTION_OL;

		condList.push_back(entrust);
	}
}

void CtaStraBaseCtx::stra_enter_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式, 则直接触发
	{
		double curQty = stra_get_position(stdCode);
		if (decimal::gt(curQty, 0))
		{
			//当前仓位大于0，逻辑是反手到qty手，所以设置信号目标仓位为-qty手
			append_signal(stdCode, -qty, userTag);
		}
		else
		{
			//当前仓位小于等于0，则是追加空方手数
			append_signal(stdCode, curQty - qty, userTag);
		}
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_OS;

		condList.push_back(entrust);
	}
}

void CtaStraBaseCtx::stra_exit_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式, 则直接触发
	{
		double curQty = stra_get_position(stdCode);
		//如果持仓为空，则不需要再执行退出多头的逻辑了
		if (decimal::le(curQty, 0))
			return;

		double maxQty = min(curQty, qty);
		append_signal(stdCode, curQty - maxQty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_CL;

		condList.push_back(entrust);
	}
}

void CtaStraBaseCtx::stra_exit_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式, 则直接触发
	{
		double curQty = stra_get_position(stdCode);
		//如果持仓是多，则不需要执行退出空头的逻辑了
		if (decimal::ge(curQty, 0))
			return ;

		double maxQty = min(abs(curQty), qty);
		append_signal(stdCode, curQty + maxQty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}

		entrust._action = COND_ACTION_CS;
		
		condList.push_back(entrust);
	}
}

double CtaStraBaseCtx::stra_get_price(const char* stdCode)
{
	if (_engine)
		return _engine->get_cur_price(stdCode);
	
	return 0.0;
}

void CtaStraBaseCtx::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice /* = 0.0 */, double stopprice /* = 0.0 */)
{
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式, 则直接触发
	{
		append_signal(stdCode, qty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		//根据目标仓位和当前仓位，判断是买还是卖
		double curVol = stra_get_position(stdCode);
		bool isBuy = decimal::gt(qty, curVol);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = isBuy ? WCT_SmallerOrEqual : WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = isBuy ? WCT_LargerOrEqual : WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_SP;

		condList.push_back(entrust);
	}
}

void CtaStraBaseCtx::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */)
{
	double curPx = _price_map[stdCode];

	SigInfo& sInfo = _sig_map[stdCode];
	sInfo._volumn = qty;
	sInfo._sigprice = curPx;
	sInfo._usertag = userTag;
	sInfo._gentime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
	sInfo._triggered = !_is_in_schedule;

	log_signal(stdCode, qty, curPx, sInfo._gentime, userTag);

	save_data();
}

void CtaStraBaseCtx::do_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, bool bTriggered /* = false */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_engine->get_date() * 10000 + _engine->get_min_time();
	uint32_t curTDate = _engine->get_trading_date();

	if (decimal::eq(pInfo._volumn, qty))
		return;

	double diff = qty - pInfo._volumn;

	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);

	if (decimal::gt(pInfo._volumn*diff, 0))
	{//当前持仓和仓位变化方向一致, 增加一条明细, 增加数量即可
		pInfo._volumn = qty;

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = curPx;
		dInfo._volumn = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		strcpy(dInfo._opentag, userTag);
		pInfo._details.push_back(dInfo);

		double fee = _engine->calc_fee(stdCode, curPx, abs(qty), 0);
		_fund_info._total_fees += fee;
		log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(qty), userTag, fee);
	}
	else
	{//持仓方向和仓位变化方向不一致, 需要平仓
		double left = abs(diff);

		pInfo._volumn = qty;
		if (decimal::eq(pInfo._volumn, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			double maxQty = min(dInfo._volumn, left);
			if (decimal::eq(maxQty, 0))
				continue;

			dInfo._volumn -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volumn, 0))
				count++;

			//计算平仓盈亏
			double profit = (curPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;

			//浮盈也要做等比缩放
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volumn / (dInfo._volumn + maxQty);
			_fund_info._total_profit += profit;

			//计算手续费
			double fee = _engine->calc_fee(stdCode, curPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, curPx, maxQty, userTag, fee);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, curPx, maxQty, profit, pInfo._closeprofit, dInfo._opentag, userTag);

			if (decimal::eq(left,0))
				break;
		}

		//需要清理掉已经平仓完的明细
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//最后, 如果还有剩余的, 则需要反手了
		if (decimal::gt(left, 0))
		{
			left = left * qty / abs(qty);

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = curPx;
			dInfo._volumn = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			strcpy(dInfo._opentag, userTag);
			pInfo._details.push_back(dInfo);

			//这里还需要写一笔成交记录
			double fee = _engine->calc_fee(stdCode, curPx, abs(left), 0);
			_fund_info._total_fees += fee;
			log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(left), userTag, fee);
		}
	}


	//存储数据
	save_data();

	if (bTriggered)	//如果是条件单触发, 则向引擎提交变化量
	{
		_engine->handle_pos_change(stdCode, diff);
	}
}

WTSKlineSlice* CtaStraBaseCtx::stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain /* = false */)
{
	std::string key = StrUtil::printf("%s#%s", stdCode, period);
	if (isMain)
	{
		if (_main_key.empty())
			_main_key = key;
		else if (_main_key != key)
			throw std::runtime_error("不能重复设定主K线");
	}

	std::string basePeriod = "";
	uint32_t times = 1;
	if (strlen(period) > 1)
	{
		basePeriod.append(period, 1);
		times = strtoul(period + 1, NULL, 10);
	}
	else
	{
		basePeriod = period;
	}

	WTSKlineSlice* kline = _engine->get_kline_slice(_context_id, stdCode, basePeriod.c_str(), count, times);
	if(kline)
	{
		//如果K线获取不到，说明也不会有闭合事件发生，所以不更新本地标记
		bool isFirst = (_kline_tags.find(key) == _kline_tags.end());	//如果没有保存标记，说明是第一次拉取该K线
		KlineTag& tag = _kline_tags[key];
		tag._closed = false;

		double lastClose = kline->close(-1);
		_price_map[stdCode] = lastClose;

		if(isMain && isFirst && !_condtions.empty())
		{
			//如果是第一次拉取主K线，则检查条件单触发时间
			bool isDay = basePeriod[0] == 'd';
			uint64_t lastBartime = isDay ? kline->date(-1) : kline->time(-1);
			if(!isDay)
				lastBartime += 199000000000;

			if(lastBartime >= _last_cond_min)
			{
				stra_log_text(fmt::format("条件单已过期，设置时间为{}，上一根主K线时间为{}，全部清空", _last_cond_min, lastBartime).c_str());
				_condtions.clear();
			}
		}

		_engine->sub_tick(id(), stdCode);
	}

	return kline;
}

WTSTickSlice* CtaStraBaseCtx::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ret = _engine->get_tick_slice(_context_id, stdCode, count);
	if (ret)
		_engine->sub_tick(id(), stdCode);

	return ret;
}

WTSTickData* CtaStraBaseCtx::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void CtaStraBaseCtx::stra_sub_ticks(const char* code)
{
	_engine->sub_tick(_context_id, code);
	stra_log_text("实时行情已订阅: %s", code);
}

WTSCommodityInfo* CtaStraBaseCtx::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

uint32_t CtaStraBaseCtx::stra_get_date()
{
	return _engine->get_date();
}

uint32_t CtaStraBaseCtx::stra_get_time()
{
	return _engine->get_min_time();
}

void CtaStraBaseCtx::stra_log_text(const char* fmt, ...)
{
	char szBuf[256] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _name.c_str());
	strcat(szBuf, fmt);
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog_dyn("strategy", _name.c_str(), LL_INFO, szBuf, args);
	va_end(args);
}

const char* CtaStraBaseCtx::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void CtaStraBaseCtx::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

uint64_t CtaStraBaseCtx::stra_get_first_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[0]._opentime;
}

uint64_t CtaStraBaseCtx::stra_get_last_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[pInfo._details.size() - 1]._opentime;
}

double CtaStraBaseCtx::stra_get_last_enterprice(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_DOUBLE;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_DOUBLE;

	return pInfo._details[pInfo._details.size() - 1]._price;
}

double CtaStraBaseCtx::stra_get_position(const char* stdCode, const char* userTag /* = "" */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (strlen(userTag) == 0)
		return pInfo._volumn;

	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._volumn;
	}

	return 0;
}

double CtaStraBaseCtx::stra_get_position_avgpx(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._volumn == 0)
		return 0.0;

	double amount = 0.0;
	for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
	{
		const DetailInfo& dInfo = *dit;
		amount += dInfo._price*dInfo._volumn;
	}

	return amount / pInfo._volumn;
}

double CtaStraBaseCtx::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

uint64_t CtaStraBaseCtx::stra_get_detail_entertime(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._opentime;
	}

	return 0;
}

double CtaStraBaseCtx::stra_get_detail_cost(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._price;
	}

	return 0.0;
}

double CtaStraBaseCtx::stra_get_detail_profit(const char* stdCode, const char* userTag, int flag /* = 0 */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		if (flag == 0)
			return dInfo._profit;
		else if (flag > 0)
			return dInfo._max_profit;
		else
			return dInfo._max_loss;
	}

	return 0.0;
}


