/*!
 * \file WtSimpExeUnit.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include <unordered_set>
#include <mutex>
#include "../Includes/ExecuteDefs.h"
USING_NS_OTP;

class WtSimpExeUnit : public ExecuteUnit
{
public:
	WtSimpExeUnit();
	virtual ~WtSimpExeUnit();

private:
	void	doCalculate();

public:
	/*
	 *	所属执行器工厂名称
	 */
	virtual const char* getFactName() override;

	/*
	 *	执行单元名称
	 */
	virtual const char* getName() override;

	/*
	 *	初始化执行单元
	 *	ctx		执行单元运行环境
	 *	code	管理的合约代码
	 */
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg) override;

	/*
	 *	订单回报
	 *	localid	本地单号
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	leftover	剩余数量
	 *	price	委托价格
	 *	isCanceled	是否已撤销
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled) override;

	/*
	 *	tick数据回调
	 *	newTick	最新的tick数据
	 */
	virtual void on_tick(WTSTickData* newTick) override;

	/*
	 *	成交回报
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	vol		成交数量，这里没有正负，通过isBuy确定买入还是卖出
	 *	price	成交价格
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	/*
	 *	下单结果回报
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	 *	设置新的目标仓位
	 *	code	合约代码
	 *	newVol	新的目标仓位
	 */
	virtual void set_position(const char* stdCode, double newVol) override;

	/*
	 *	交易通道就绪回调
	 */
	virtual void on_channel_ready() override;

	/*
	 *	交易通道丢失回调
	 */
	virtual void on_channel_lost() override;

private:
	WTSTickData* _last_tick;	//上一笔行情
	double		_target_pos;	//目标仓位
	double		_unsent_qty;	//未发送数量


	WTSCommodityInfo*	_comm_info;
	WTSSessionInfo*		_sess_info;

	//////////////////////////////////////////////////////////////////////////
	//执行参数
	int32_t		_price_offset;
	uint32_t	_expire_secs;
	int32_t		_price_mode;

	typedef std::unordered_set<uint32_t> IDSet;
	IDSet			_orders;
	std::mutex		_mtx_ords;
	uint64_t		_last_entry_time;
	uint32_t		_cancel_cnt;
};

