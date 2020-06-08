/*!
* \file MfStrategyDefs.h
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#pragma once
#include <string>
#include <stdint.h>

#include "../Share/WTSMarcos.h"

NS_OTP_BEGIN
class WTSVariant;
class IMfStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

class MfStrategy
{
public:
	MfStrategy(const char* id) :_id(id){}
	virtual ~MfStrategy(){}

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

	/*
	*	��ʼ���ص�
	*/
	virtual void on_init(IMfStraCtx* ctx){}

	/*
	*	�����߼�ִ�����
	*/
	virtual void on_schedule(IMfStraCtx* ctx, uint32_t uDate, uint32_t uTime){}

	/*
	*	tick����
	*/
	virtual void on_tick(IMfStraCtx* ctx, const char* stdCode, WTSTickData* newTick){}

	/*
	*	K�߱պ�
	*/
	virtual void on_bar(IMfStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar){}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//���Թ����ӿ�
typedef void(*FuncEnumMfStrategyCallback)(const char* factName, const char* straName, bool isLast);

class IMfStrategyFact
{
public:
	IMfStrategyFact(){}
	virtual ~IMfStrategyFact(){}

public:
	/*
	*	��ȡ������
	*/
	virtual const char* getName() = 0;

	/*
	*	ö�ٲ���
	*/
	virtual void enumStrategy(FuncEnumMfStrategyCallback cb) = 0;

	/*
	*	�������ƴ���K�߼������
	*/
	virtual MfStrategy* createStrategy(const char* name, const char* id) = 0;


	/*
	*	ɾ������
	*/
	virtual bool deleteStrategy(MfStrategy* stra) = 0;
};

//����ִ�й���
typedef IMfStrategyFact* (*FuncCreateMfStraFact)();
//ɾ��ִ�й���
typedef void(*FuncDeleteMfStraFact)(IMfStrategyFact* &fact);
