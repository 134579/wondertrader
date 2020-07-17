/*!
 * \file IHftStraCtx.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>
#include <string>
#include "ExecuteDefs.h"

#include "../Includes/WTSMarcos.h"

NS_OTP_BEGIN
class WTSCommodityInfo;
class WTSTickSlice;
class WTSKlineSlice;
class WTSTickData;
struct WTSBarStruct;

class IHftStraCtx
{
public:
	IHftStraCtx(const char* name):_name(name){}
	virtual ~IHftStraCtx(){}

	const char* name() const{ return _name.c_str(); }

public:
	virtual uint32_t id() = 0;

	//�ص�����
	virtual void on_init() = 0;
	virtual void on_tick(const char* code, WTSTickData* newTick) = 0;
	virtual void on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar){}

	//���Խӿ�
	virtual bool		stra_cancel(uint32_t localid) = 0;
	virtual OrderIDs	stra_cancel(const char* code, bool isBuy, double qty) = 0;
	virtual OrderIDs	stra_buy(const char* stdCode, double price, double qty) = 0;
	virtual OrderIDs	stra_sell(const char* stdCode, double price, double qty) = 0;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* code) = 0;
	virtual WTSKlineSlice*	stra_get_bars(const char* code, const char* period, uint32_t count) = 0;
	virtual WTSTickSlice*	stra_get_ticks(const char* code, uint32_t count) = 0;
	virtual WTSTickData*	stra_get_last_tick(const char* code) = 0;

	virtual double stra_get_position(const char* code) = 0;
	virtual double stra_get_price(const char* code) = 0;
	virtual double stra_get_undone(const char* stdCode) = 0;

	virtual uint32_t stra_get_date() = 0;
	virtual uint32_t stra_get_time() = 0;
	virtual uint32_t stra_get_secs() = 0;

	virtual void stra_sub_ticks(const char* code) = 0;

	virtual void stra_log_text(const char* fmt, ...) = 0;

	virtual void stra_save_user_data(const char* key, const char* val) {}

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") { return defVal; }

protected:
	std::string _name;
};

NS_OTP_END