#pragma once
#include <string>
#include <stdint.h>

#include "../Share/WTSMarcos.h"

NS_OTP_BEGIN
class WTSVariant;
class IHftStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

class HftStrategy
{
public:
	HftStrategy(const char* id) :_id(id){}
	virtual ~HftStrategy(){}

public:
	/*
	*	ִ�е�Ԫ����
	*/
	virtual const char* getName() = 0;

	/*
	*	����ִ������������
	*/
	virtual const char* getFactName() = 0;

	/*
	*	��ʼ��
	*/
	virtual bool init(WTSVariant* cfg){ return true; }

	virtual const char* id() const { return _id.c_str(); }

	//�ص�����
	virtual void on_init(IHftStraCtx* ctx) = 0;
	virtual void on_tick(IHftStraCtx* ctx, const char* code, WTSTickData* newTick) = 0;
	virtual void on_bar(IHftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual void on_schedule(IHftStraCtx* ctx, uint32_t uDate, uint32_t uTime) = 0;

	virtual void on_trade(IHftStraCtx* ctx, const char* stdCode, bool isBuy, uint32_t vol, double price) = 0;
	virtual void on_position(IHftStraCtx* ctx, const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail) = 0;
	virtual void on_order(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled = false) = 0;
	virtual void on_channel_ready(IHftStraCtx* ctx) = 0;
	virtual void on_channel_lost(IHftStraCtx* ctx) = 0;
	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message) = 0;

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//���Թ����ӿ�
typedef void(*FuncEnumHftStrategyCallback)(const char* factName, const char* straName, bool isLast);

class IHftStrategyFact
{
public:
	IHftStrategyFact(){}
	virtual ~IHftStrategyFact(){}

public:
	/*
	*	��ȡ������
	*/
	virtual const char* getName() = 0;

	/*
	*	ö�ٲ���
	*/
	virtual void enumStrategy(FuncEnumHftStrategyCallback cb) = 0;

	/*
	*	�������ƴ���ִ�е�Ԫ
	*/
	virtual HftStrategy* createStrategy(const char* name, const char* id) = 0;

	/*
	*	ɾ��ִ�е�Ԫ
	*/
	virtual bool deleteStrategy(HftStrategy* stra) = 0;
};

//����ִ�й���
typedef IHftStrategyFact* (*FuncCreateHftStraFact)();
//ɾ��ִ�й���
typedef void(*FuncDeleteHftStraFact)(IHftStrategyFact* &fact);
