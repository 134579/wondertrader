/*!
 * \file WtHfExeUnit.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include <unordered_map>
#include "../WtCore/ExecuteDefs.h"

#include "../Share/StdUtils.hpp"

USING_NS_OTP;

class WtHfExeUnit : public ExecuteUnit
{
public:
	WtHfExeUnit();
	virtual ~WtHfExeUnit();

private:
	void	doCalculate();

	void	check_orders();

public:
	/*
	*	����ִ������������
	*/
	virtual const char* getFactName() override;

	/*
	*	ִ�е�Ԫ����
	*/
	virtual const char* getName() override;

	/*
	*	��ʼ��ִ�е�Ԫ
	*	ctx		ִ�е�Ԫ���л���
	*	code	����ĺ�Լ����
	*/
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg) override;

	/*
	*	�����ر�
	*	localid	���ص���
	*	code	��Լ����
	*	isBuy	��or��
	*	leftover	ʣ������
	*	price	ί�м۸�
	*	isCanceled	�Ƿ��ѳ���
	*/
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t leftover, double price, bool isCanceled) override;

	/*
	*	tick���ݻص�
	*	newTick	���µ�tick����
	*/
	virtual void on_tick(WTSTickData* newTick) override;

	/*
	*	�ɽ��ر�
	*	code	��Լ����
	*	isBuy	��or��
	*	vol		�ɽ�����������û��������ͨ��isBuyȷ�����뻹������
	*	price	�ɽ��۸�
	*/
	virtual void on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price) override;

	/*
	*	�µ�����ر�
	*/
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	*	�����µ�Ŀ���λ
	*	code	��Լ����
	*	newVol	�µ�Ŀ���λ
	*/
	virtual void set_position(const char* stdCode, int32_t newVol) override;

	/*
	*	����ͨ�������ص�
	*/
	virtual void on_channel_ready() override;

	/*
	*	����ͨ����ʧ�ص�
	*/
	virtual void on_channel_lost() override;


	//***************************************************************************
	virtual double on_tick_bias(WTSTickData* curTick);
	virtual double on_trade_bias(WTSTickData* curTick);
	virtual double on_size_bias(WTSTickData* curTick);
	virtual double on_book_bias(WTSTickData* curTick);
	virtual double on_tickalpha_sum(WTSTickData* curTick);
	//***************************************************************************

private:
	WTSTickData* _last_tick;	//��һ������
	int32_t		_target_pos;	//Ŀ���λ
	int32_t		_unsent_qty;	//δ��������
	bool		_channel_ready;


	WTSCommodityInfo*	_comm_info;

	//////////////////////////////////////////////////////////////////////////
	//ִ�в���
	typedef std::unordered_map<uint32_t, uint64_t> Orders;
	Orders			_orders;
	StdRecurMutex	_mtx_ords;
	uint32_t		_cancel_cnt;

	//////////////////////////////////////////////////////////////////////////
	//����
	uint32_t		_exec_secs;	//ִ����ʱ�䣬��λs
	uint32_t		_exec_tail;	//ִ��β��ʱ��
	uint32_t		_sticky;	//�ҵ�ʱ�ޣ���λs

	//////////////////////////////////////////////////////////////////////////
	//��ʱ����
	uint32_t		_fire_span;	//�������
	bool			_show_hand;	//����ʱ����
	uint64_t		_exec_begin_time;
	uint64_t		_last_fire_time;

	//////////////////////////////////////////////////////////////////////////
	//���߼�����
	int32_t			_total_s;
	double			_alpha_tickbias;
	double			_old_alpha_tickbias;
	uint64_t        _last_tickbias_cal_time;
};

