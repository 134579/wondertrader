#pragma once
#include <set>
#include <map>
#include <stdint.h>
#include <functional>
#include "WTSTypes.h"

NS_OTP_BEGIN
class WTSParams;
class WTSEntrust;
class WTSOrderInfo;
class WTSTradeInfo;
class WTSEntrustAction;
class WTSAccountInfo;
class WTSInvestorInfo;
class WTSPositionItem;
class WTSPositionDetail;
class WTSContractInfo;
class WTSError;
class WTSTickData;
class WTSNotify;
class WTSArray;
class IBaseDataMgr;

typedef std::function<void()>	CommonExecuter;


//ί�лص��ӿ�
class ITraderApiListener
{
public:
	virtual void handleEvent(WTSTraderEvent e, int32_t ec) = 0;
	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) = 0;
	virtual void onLogout(){}
	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err){}
	virtual void onRspAccount(WTSArray* ayAccounts) {}
	virtual void onRspPosition(const WTSArray* ayPositions){}
	virtual void onRspOrders(const WTSArray* ayOrders){}
	virtual void onRspTrades(const WTSArray* ayTrades){}
	virtual void onRspSettlementInfo(uint32_t uDate, const char* content){}

	virtual void onPushOrder(WTSOrderInfo* orderInfo){}
	virtual void onPushTrade(WTSTradeInfo* tradeRecord){}

	virtual void onTraderError(WTSError*	err){}

public:
	virtual IBaseDataMgr*	getBaseDataMgr() = 0;
	virtual void handleTraderLog(WTSLogLevel ll, const char* format, ...){}
};

//�µ��ӿڹ���ӿ�
class ITraderApi
{
public:
	virtual ~ITraderApi(){}

public:
	/*
	*	��ʼ������������
	*/
	virtual bool init(WTSParams *params) = 0;

	/*
	*	�ͷŽ���������
	*/
	virtual void release() = 0;

	/*
	*	ע��ص��ӿ�
	*/
	virtual void registerListener(ITraderApiListener *listener) = 0;


	//////////////////////////////////////////////////////////////////////////
	//ҵ���߼��ӿ�

	/*
	*	���ӷ�����
	*/
	virtual void connect() = 0;

	/*
	*	�Ͽ�����
	*/
	virtual void disconnect() = 0;

	virtual bool isConnected() = 0;

	/*
	*	����ί�е���
	*/
	virtual bool makeEntrustID(char* buffer, int length){ return false; }

	/*
	*	��¼�ӿ�
	*/
	virtual int login(const char* user, const char* pass, const char* productInfo) = 0;

	/*
	*	ע���ӿ�
	*/
	virtual int logout() = 0;

	/*
	*	�µ��ӿ�
	*	entrust �µ��ľ������ݽṹ
	*/
	virtual int orderInsert(WTSEntrust* eutrust) = 0;

	/*
	*	���������ӿ�
	*	action	�����ľ������ݽṹ
	*/
	virtual int orderAction(WTSEntrustAction* action) = 0;

	/*
	*	��ѯ�˻���Ϣ
	*/
	virtual int queryAccount() = 0;

	/*
	*	��ѯ�ֲ���Ϣ
	*/
	virtual int queryPositions() = 0;

	/*
	*	��ѯ���ж���
	*/
	virtual int queryOrders() = 0;

	/*
	*	��ѯ�ɽ���ϸ
	*/
	virtual int	queryTrades() = 0;

	/*
	 *	��ѯ���㵥
	 */
	virtual int querySettlement(uint32_t uDate){ return 0; }

};

NS_OTP_END

//��ȡIDataMgr�ĺ���ָ������
typedef otp::ITraderApi* (*FuncCreateTrader)();
typedef void(*FuncDeleteTrader)(otp::ITraderApi* &trader);
