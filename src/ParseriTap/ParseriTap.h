#pragma once
#include "../Share/IParserApi.h"
#include "./TapQuote/TapQuoteAPI.h"
#include <map>


NS_OTP_BEGIN
class WTSTickData;
NS_OTP_END

USING_NS_OTP;


class ParseriTap :	public IParserApi, public ITapQuoteAPINotify
{
public:
	ParseriTap();
	virtual ~ParseriTap();

public:
	enum LoginStatus
	{
		LS_NOTLOGIN,
		LS_LOGINING,
		LS_LOGINED
	};

//IQuoteParser �ӿ�
public:
	virtual bool init(WTSParams* config);

	virtual void release();

	virtual bool connect();

	virtual bool disconnect();

	virtual bool isConnected();

	virtual void subscribe(const CodeSet& vecSymbols);
	virtual void unsubscribe(const CodeSet& vecSymbols);

	virtual void registerListener(IParserApiListener* listener);


public:
	virtual void TAP_CDECL OnRspLogin(TAPIINT32 errorCode, const TapAPIQuotLoginRspInfo *info);

	virtual void TAP_CDECL OnAPIReady();

	virtual void TAP_CDECL OnDisconnect(TAPIINT32 reasonCode);

	virtual void TAP_CDECL OnRspChangePassword(TAPIUINT32 sessionID, TAPIINT32 errorCode){}

	virtual void TAP_CDECL OnRspQryExchange(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIExchangeInfo *info){}

	virtual void TAP_CDECL OnRspQryCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteCommodityInfo *info){}

	//virtual void TAP_CDECL OnRspQryTimeBucketOfCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPITimeBucketOfCommodityInfo *info){}

	//virtual void TAP_CDECL OnRtnTimeBucketOfCommodity(const TapAPITimeBucketOfCommodityInfo *info){}

	virtual void TAP_CDECL OnRspQryContract(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteContractInfo *info){}

	virtual void TAP_CDECL OnRtnContract(const TapAPIQuoteContractInfo *info){}

	virtual void TAP_CDECL OnRspSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIQuoteWhole *info);

	virtual void TAP_CDECL OnRspUnSubscribeQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIContract *info);

	virtual void TAP_CDECL OnRtnQuote(const TapAPIQuoteWhole *info);

	//virtual void TAP_CDECL OnRspQryHisQuote(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisQuoteQryRsp *info){}

private:
	/*
	 *	���͵�¼����
	 */
	bool login(bool bNeedReconn = false);
	/*
	 *	����Ʒ������
	 */
	void subscribe();

	void reconnect();

	const char* exchgI2O(const char* exchg);
	const char* exchgO2I(const char* exchg);

	const char* productI2O(const char* pid);
	const char* productO2I(const char* pid);


private:
	uint32_t			m_uTradingDate;
	LoginStatus			m_loginState;
	ITapQuoteAPI*		m_pUserAPI;
	bool				m_bReconnect;

	std::string			m_strHost;
	int					m_nPort;
	std::string			m_strUser;
	std::string			m_strPass;
	std::string			m_strAuthCode;

	CodeSet				m_filterSubs;
	CodeSet				m_setComms;

	uint32_t			m_uSessionID;
	bool				m_bStopped;

	IParserApiListener*	m_parserSink;
	IBaseDataMgr*		m_bdMgr;

	typedef std::map<std::string,std::string>	StringMap;
	StringMap			m_mapExchgIToO;
	StringMap			m_mapExchgOToI;

	StringMap			m_mapProductIToO;
	StringMap			m_mapProductOToI;
};

