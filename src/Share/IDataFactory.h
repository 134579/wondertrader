/*!
 * \file IDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ����ƴ�ӹ����ӿڶ���
 */
#pragma once
#include <stdint.h>
#include "../Share/WTSTypes.h"

//USING_NS_OTP;

NS_OTP_BEGIN
class WTSKlineData;
class WTSHisTrendData;
class WTSTickData;
class WTSSessionInfo;
class WTSArray;
class WTSContractInfo;
struct WTSBarStruct;

/*
 *	���ݹ���
 *	��Ҫ���ڸ������ݵ�ƴ��
 *	����ֻ����һ���ӿ�
 */
class IDataFactory
{
public:
	/*
	 *	����K������
	 *	@klineData Ҫ���µ�K������
	 *	@tick ���µ�tick����
	 *	����ֵ �Ƿ����µ�K������
	 */
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo)						= 0;
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo)				= 0;

	virtual WTSKlineData*	extractKlineData(WTSKlineData* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true)	= 0;

	virtual WTSKlineData*	extractKlineData(WTSArray* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false)	= 0;

	virtual bool			mergeKlineData(WTSKlineData* klineData, WTSKlineData* newKline)											= 0;

};

NS_OTP_END
