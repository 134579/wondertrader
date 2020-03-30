/*!
 * \file ParserCTP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Share/IParserApi.h"
#include "./ThostTraderApi/ThostFtdcMdApi.h"
#include <map>

NS_OTP_BEGIN
class WTSTickData;
NS_OTP_END

USING_NS_OTP;

class QuoteParser_CTP :	public IParserApi, public CThostFtdcMdSpi
{
public:
	QuoteParser_CTP();
	virtual ~QuoteParser_CTP();

public:
	enum LoginStatus
	{
		LS_NOTLOGIN,
		LS_LOGINING,
		LS_LOGINED
	};

//IQuoteParser �ӿ�
public:
	virtual bool init(WTSParams* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerListener(IParserApiListener* listener) override;


//CThostFtdcMdSpi �ӿ�
public:
	virtual void OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnFrontConnected();

	virtual void OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnFrontDisconnected( int nReason );

	virtual void OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData );

	virtual void OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast );

	virtual void OnHeartBeatWarning( int nTimeLapse );

private:
	/*
	 *	���͵�¼����
	 */
	void ReqUserLogin();
	/*
	 *	����Ʒ������
	 */
	void SubscribeMarketData();
	/*
	 *	��������Ϣ
	 */
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);


private:
	uint32_t			m_uTradingDate;
	LoginStatus			m_loginState;
	CThostFtdcMdApi*	m_pUserAPI;

	std::string			m_strFrontAddr;
	std::string			m_strBroker;
	std::string			m_strUserID;
	std::string			m_strPassword;

	CodeSet				m_filterSubs;

	int					m_iRequestID;

	IParserApiListener*	m_parserSink;
	IBaseDataMgr*		m_pBaseDataMgr;
};

