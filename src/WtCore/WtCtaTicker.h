/*!
 * \file WtCtaTicker.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>
#include <atomic>

#include "../Share/WTSMarcos.h"
#include "../Share/BoostDefine.h"

NS_OTP_BEGIN
class WTSSessionInfo;
class IDataReader;
class WTSTickData;

class WtCtaEngine;
//////////////////////////////////////////////////////////////////////////
//����ʱ�䲽����
class WtCtaRtTicker
{
public:
	WtCtaRtTicker(WtCtaEngine* engine) 
		: _engine(engine)
		, _stopped(false)
		, _date(0)
		, _time(UINT_MAX)
		, _next_check_time(0)
		, _last_emit_pos(0)
		, _cur_pos(0){}
	~WtCtaRtTicker(){}

public:
	void	init(IDataReader* store, const char* sessionID);
	//void	set_time(uint32_t uDate, uint32_t uTime);
	void	on_tick(WTSTickData* curTick, bool isHot = false);

	void	run();
	void	stop();

	bool		is_in_trading() const;
	uint32_t	time_to_mins(uint32_t uTime) const;

private:
	void	trigger_price(WTSTickData* curTick, bool isHot);

private:
	WTSSessionInfo*	_s_info;
	WtCtaEngine*	_engine;
	IDataReader*		_store;

	uint32_t	_date;
	uint32_t	_time;

	uint32_t	_cur_pos;

	BoostUniqueMutex	_mtx;
	std::atomic<uint64_t>	_next_check_time;
	std::atomic<uint32_t>	_last_emit_pos;

	bool		_stopped;
	BoostThreadPtr	_thrd;

};
NS_OTP_END