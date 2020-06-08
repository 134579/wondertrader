/*!
 * \file PorterDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include "../Share/WTSTypes.h"

NS_OTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
NS_OTP_END

USING_NS_OTP;

#ifdef _WIN32
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#endif

typedef unsigned long		CtxHandler;
typedef unsigned long		WtUInt32;
typedef unsigned long long	WtUInt64;
typedef const char*			WtString;

static const WtUInt32	EVENT_ENGINE_INIT	= 1;	//��ܳ�ʼ��
static const WtUInt32	EVENT_SESSION_BEGIN = 2;	//�����տ�ʼ
static const WtUInt32	EVENT_SESSION_END	= 3;	//�����ս���
static const WtUInt32	EVENT_ENGINE_SCHDL	= 4;	//��ܵ���

/*
*	�ص���������
*/
typedef void(PORTER_FLAG *FuncGetBarsCallback)(CtxHandler cHandle, const char* code, const char* period, WTSBarStruct* bar, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(CtxHandler cHandle, const char* code, WTSTickStruct* tick, bool isLast);
typedef void(PORTER_FLAG *FuncStraInitCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncStraTickCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncStraCalcCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncStraBarCallback)(CtxHandler cHandle, const char* code, const char* period, WTSBarStruct* newBar);


typedef void(PORTER_FLAG *FuncEventCallback)(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime);
