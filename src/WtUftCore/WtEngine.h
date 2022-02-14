/*!
 * \file WtEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <queue>
#include <functional>
#include <stdint.h>

#include "ParserAdapter.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/RiskMonDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Share/BoostFile.hpp"


NS_WTP_BEGIN
class WTSSessionInfo;
class WTSCommodityInfo;
class WTSContractInfo;

class IBaseDataMgr;
class IHotMgr;

class WTSVariant;

class WTSTickData;
struct WTSBarStruct;
class WTSTickSlice;
class WTSKlineSlice;
class WTSPortFundInfo;

class WtUftDtMgr;
class TraderAdapterMgr;

class EventNotifier;

typedef std::function<void()>	TaskItem;

class WtRiskMonWrapper
{
public:
	WtRiskMonWrapper(WtRiskMonitor* mon, IRiskMonitorFact* fact) :_mon(mon), _fact(fact){}
	~WtRiskMonWrapper()
	{
		if (_mon)
		{
			_fact->deleteRiskMonotor(_mon);
		}
	}

	WtRiskMonitor* self(){ return _mon; }


private:
	WtRiskMonitor*		_mon;
	IRiskMonitorFact*	_fact;
};
typedef std::shared_ptr<WtRiskMonWrapper>	WtRiskMonPtr;

class IEngineEvtListener
{
public:
	virtual void on_initialize_event() {}
	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) {}
	virtual void on_session_event(uint32_t uDate, bool isBegin = true) {}
};

class WtEngine : public IParserStub
{
public:
	WtEngine();

	inline void set_adapter_mgr(TraderAdapterMgr* mgr) { _adapter_mgr = mgr; }

	void set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs = 0, uint32_t rawTime = 0);

	void set_trading_date(uint32_t curTDate);

	inline uint32_t get_date() { return _cur_date; }
	inline uint32_t get_min_time() { return _cur_time; }
	inline uint32_t get_raw_time() { return _cur_raw_time; }
	inline uint32_t get_secs() { return _cur_secs; }
	inline uint32_t get_trading_date() { return _cur_tdate; }

	inline IBaseDataMgr*		get_basedata_mgr(){ return _base_data_mgr; }
	WTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	WTSCommodityInfo*	get_commodity_info(const char* stdCode);
	WTSContractInfo*	get_contract_info(const char* stdCode);

	WTSTickData*	get_last_tick(uint32_t sid, const char* stdCode);
	WTSTickSlice*	get_tick_slice(uint32_t sid, const char* stdCode, uint32_t count);
	WTSKlineSlice*	get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, uint64_t etime = 0);

	void sub_tick(uint32_t sid, const char* code);

	double get_cur_price(const char* stdCode);

	inline void regEventListener(IEngineEvtListener* listener)
	{
		_evt_listener = listener;
	}

	//////////////////////////////////////////////////////////////////////////
	/// IParserStub�ӿ�
	virtual void handle_push_quote(WTSTickData* newTick, uint32_t hotFlag) override;

public:
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtUftDtMgr* dataMgr);

	virtual void run(bool bAsync = false) = 0;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick);

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;

	virtual void on_init(){}

protected:
	uint32_t		_cur_date;	//��ǰ����
	uint32_t		_cur_time;		//��ǰʱ��, ��1������ʱ��, ����0900, ���ʱ���1��������0901, _cur_timeҲ����0901, �����Ϊ��CTA���淽��
	uint32_t		_cur_raw_time;	//��ǰ��ʵʱ��
	uint32_t		_cur_secs;	//��ǰ����, ��������
	uint32_t		_cur_tdate;	//��ǰ������

	double			_all_tick_mode;	//ȫtickģʽ�������tick���ݳɽ���Ϊ0��Ҳ����ontick��Ĭ��Ϊfalse����ֻ�����гɽ�����tick

	IBaseDataMgr*	_base_data_mgr;	//�������ݹ�����
	WtUftDtMgr*		_data_mgr;		//���ݹ�����
	IEngineEvtListener*	_evt_listener;

	//By Wesley @ 2022.02.07
	//tick���ݶ����first��contextid��second�Ƕ���ѡ�0-ԭʼ���ģ�1-ǰ��Ȩ��2-��Ȩ
	typedef faster_hashset<uint32_t> SubList;
	typedef faster_hashmap<std::string, SubList>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick���ݶ��ı�
	StraSubMap		_bar_sub_map;	//K�����ݶ��ı�	

	//////////////////////////////////////////////////////////////////////////
	//
	typedef faster_hashmap<std::string, double> PriceMap;
	PriceMap		_price_map;

	TraderAdapterMgr*	_adapter_mgr;
};
NS_WTP_END