#include "WtSelEngine.h"
#include "WtDataManager.h"
#include "WtSelTicker.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/IHotMgr.h"
#include "../Share/StrUtil.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSSessionInfo.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/decimal.h"

#include <atomic>

USING_NS_OTP;

inline uint32_t makeTaskId()
{
	static std::atomic<uint32_t> _auto_task_id{ 1 };
	return _auto_task_id.fetch_add(1);
}


WtSelEngine::WtSelEngine()
	: _terminated(false)
	, _cfg(NULL)
{
}


WtSelEngine::~WtSelEngine()
{
}

void WtSelEngine::on_session_end()
{
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, false);
}

void WtSelEngine::on_session_begin()
{
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, true);
}

void WtSelEngine::on_init()
{
	if (_evt_listener)
		_evt_listener->on_initialize_event();
}

void WtSelEngine::handle_push_quote(WTSTickData* curTick, bool isHot)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(curTick, isHot);
}

void WtSelEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	const SIDSet& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = *it;
		auto cit = _ctx_map.find(sid);
		if (cit != _ctx_map.end())
		{
			SelContextPtr& ctx = cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}

	WTSLogger::info("K�� [%s#%s%d] @ %u�ѱպ�", stdCode, period, times, period[0] == 'd' ? newBar->date : newBar->time);
}

void WtSelEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//�������ʵ����, ��Ҫ���ݸ�ִ����
	auto it = _subed_raw_codes.find(stdCode);
	if (it != _subed_raw_codes.end())
	{
		//�Ƿ�������Լ����ı��, ��Ҫ���ڸ�ִ���������ݵ�
		for (auto it = _executers.begin(); it != _executers.end(); it++)
		{
			WtExecuterPtr& executer = (*it);
			executer->on_tick(stdCode, curTick);
		}
	}

	auto sit = _tick_sub_map.find(stdCode);
	if (sit != _tick_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end() && curTick->volumn())
			{
				SelContextPtr& ctx = cit->second;
				ctx->on_tick(stdCode, curTick);
			}
		}
	}
}

void WtSelEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{
	//Ҫ�Ƚ���һ���ӵ�ʱ��
	uint32_t nextTime = TimeUtils::getNextMinute(curTime, 1);
	if (nextTime < curTime)
		curDate = TimeUtils::getNextDate(curDate);

	uint32_t weekDay = TimeUtils::getWeekDay(curDate);

	for (auto& v : _tasks)
	{
		TaskInfoPtr& tInfo = v.second;
		if (tInfo->_time != nextTime)
			continue;

		uint64_t now = (uint64_t)curDate * 10000 + nextTime;
		if (tInfo->_last_exe_time >= now)
			continue;

		if (_base_data_mgr->isHoliday(tInfo->_trdtpl, curDate, true))
			continue;

		//��ȡ��һ�������յ�����
		uint32_t preTDate = TimeUtils::getNextDate(_cur_date, -1);
		bool bHasHoliday = false;
		uint32_t days = 1;
		while (_base_data_mgr->isHoliday(tInfo->_trdtpl, preTDate, true))
		{
			bHasHoliday = true;
			preTDate = TimeUtils::getNextDate(preTDate, -1);
			days++;
		}
		uint32_t preWD = TimeUtils::getWeekDay(preTDate);

		bool bIgnore = true;
		switch (tInfo->_period)
		{
		case TPT_Daily:
			bIgnore = false;
			break;
		case TPT_Monthly:
			//if (preTDate % 1000000 < _task->_day && _cur_date % 1000000 >= _task->_day)
			//	fired = true;
			if (_cur_date % 1000000 == tInfo->_day)
				bIgnore = false;
			else if (bHasHoliday)
			{
				//��һ�����������ϸ��£��ҵ�ǰ���ڴ��ڴ�������
				//˵������µĿ�ʼ�����ڽڼ����ڣ�˳�ӵ�����
				if ((preTDate % 10000 / 100 < _cur_date % 10000 / 100) && _cur_date % 1000000 > tInfo->_day)
				{
					bIgnore = false;
				}
				else if (preTDate % 1000000 < tInfo->_day && _cur_date % 1000000 > tInfo->_day)
				{
					//��һ����������ͬһ���£���С�ڴ������ڣ����ǽ�����ڴ������ڣ�˵����ȷ�������ڵ��ڼ����ڣ�˳�ӵ�����
					bIgnore = false;
				}
			}
			break;
		case TPT_Weekly:
			//if (preWD < _task->_day && weekDay >= _task->_day)
			//	fired = true;
			if (weekDay == tInfo->_day)
				bIgnore = false;
			else if (bHasHoliday)
			{
				if (days >= 7 && weekDay > tInfo->_day)
				{
					bIgnore = false;
				}
				else if (preWD > weekDay && weekDay > tInfo->_day)
				{
					//��һ�������յ����ڴ��ڽ�������ڣ�˵������һ����
					bIgnore = false;
				}
				else if (preWD < tInfo->_day && weekDay > tInfo->_day)
				{
					bIgnore = false;
				}
			}
			break;
		case TPT_Yearly:
			if (preTDate % 10000 < tInfo->_day && _cur_date % 10000 >= tInfo->_day)
				bIgnore = false;
			break;
		}

		if (bIgnore)
			continue;

		//TODO: �ص�����
		SelContextPtr ctx = getContext(tInfo->_id);
		StdThreadPtr thrd(new StdThread([ctx, curDate, curTime, nextTime](){
			if (ctx)
				ctx->on_schedule(curDate, curTime, nextTime);
		}));	

		tInfo->_last_exe_time = now;
	}
}

void WtSelEngine::run(bool bAsync /*= false*/)
{
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker = new WtSelRtTicker(this);
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	_tm_ticker->run();
}

void WtSelEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr);

	_cfg = cfg;
	_cfg->retain();
}

void WtSelEngine::addContext(SelContextPtr ctx, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict /* = true */, const char* trdtpl /* = "CHINA" */)
{
	if (ctx == NULL)
		return;

	auto it = _tasks.find(ctx->id());
	if(it != _tasks.end())
	{
		WTSLogger::error("����ע��ʧ�ܣ�������%u�ѱ�ע��", ctx->id());
		return;
	}

	TaskInfoPtr tInfo(new TaskInfo);
	strcpy(tInfo->_name, ctx->name());
	strcpy(tInfo->_trdtpl, trdtpl);
	tInfo->_day = date;
	tInfo->_time = time;
	tInfo->_period = period;
	tInfo->_strict_time = bStrict;
	tInfo->_id = makeTaskId();

	_tasks[ctx->id()] = tInfo;

	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

SelContextPtr WtSelEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return SelContextPtr();

	return it->second;
}

void WtSelEngine::handle_pos_change(const char* stdCode, double diffQty)
{
	std::string realCode = stdCode;
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		std::string exchg, pid, code;
		bool isHot = false;
		CodeHelper::extractStdCode(stdCode, exchg, code, pid, isHot);
		code = _hot_mgr->getRawCode(exchg.c_str(), pid.c_str(), _cur_tdate);
		realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), exchg.c_str());
	}

	PosInfo& pItem = _pos_map[realCode];
	double targetPos = pItem._volumn + diffQty;

	bool bRiskEnabled = false;
	if (!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::info2("risk", "����̲�λ���ϵ��Ϊ%.2f", _risk_volscale);
		bRiskEnabled = true;
	}
	if (bRiskEnabled && targetPos != 0)
	{
		double symbol = targetPos / abs(targetPos);
		targetPos = abs(targetPos)*_risk_volscale*symbol;
	}

	push_task([this, realCode, targetPos](){
		append_signal(realCode.c_str(), targetPos);
	});

	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		WtExecuterPtr& executer = (*it);
		executer->on_position_changed(realCode.c_str(), targetPos);
	}
}