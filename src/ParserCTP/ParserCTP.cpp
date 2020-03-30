/*!
 * \file ParserCTP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserCTP.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/WTSParams.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"

#include <boost/filesystem.hpp>

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "./ThostTraderApi/thostmduserapi.lib")
#else
#pragma comment(lib, "./ThostTraderApi/thostmduserapi32.lib")
#endif
#endif

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		QuoteParser_CTP* parser = new QuoteParser_CTP();
		return parser;
	}

	EXPORT_FLAG void deleteParser(IParserApi* &parser)
	{
		if (NULL != parser)
		{
			delete parser;
			parser = NULL;
		}
	}
};


uint32_t strToTime(const char* strTime)
{
	std::string str;
	const char *pos = strTime;
	while(strlen(pos) > 0)
	{
		if(pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	return strtoul(str.c_str(), NULL, 10);
}

inline double checkValid(double val)
{
	if (val == DBL_MAX || val == FLT_MAX)
		return 0;

	return val;
}

QuoteParser_CTP::QuoteParser_CTP()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


QuoteParser_CTP::~QuoteParser_CTP()
{
	m_pUserAPI = NULL;
}

bool QuoteParser_CTP::init(WTSParams* config)
{
	m_strFrontAddr	= config->getCString("front");
	m_strBroker = config->getCString("broker");
	m_strUserID = config->getCString("user");
	m_strPassword = config->getCString("pass");

#ifdef _WIN32
#else
	const char* module = config->getCString("module");
	if (strlen(module) == 0)
		DLLHelper::load_library("libthostmduserapi_se.so");
	else
		DLLHelper::load_library(module);
#endif

	std::string path = StrUtil::printf("CTPParserFlow/%s/%s/", m_strBroker.c_str(), m_strUserID.c_str());
	if (!StdFile::exists(path.c_str()))
	{
		boost::filesystem::create_directories(boost::filesystem::path(path));
	}
	m_pUserAPI = CThostFtdcMdApi::CreateFtdcMdApi(path.c_str());
	m_pUserAPI->RegisterSpi(this);
	m_pUserAPI->RegisterFront((char*)m_strFrontAddr.c_str());

	return true;
}

void QuoteParser_CTP::release()
{
	disconnect();
}

bool QuoteParser_CTP::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool QuoteParser_CTP::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void QuoteParser_CTP::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	IsErrorRspInfo(pRspInfo);
}

void QuoteParser_CTP::OnFrontConnected()
{
	if(m_parserSink)
	{
		m_parserSink->handleParserLog(LL_INFO, "[CTPParser]CTP�������������");
		m_parserSink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void QuoteParser_CTP::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_uTradingDate = strtoul(m_pUserAPI->GetTradingDay(), NULL, 10);
		
		if(m_parserSink)
		{
			m_parserSink->handleEvent(WPE_Login, 0);
		}

		//������������
		SubscribeMarketData();
	}
}

void QuoteParser_CTP::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_parserSink)
	{
		m_parserSink->handleEvent(WPE_Logout, 0);
	}
}

void QuoteParser_CTP::OnFrontDisconnected( int nReason )
{
	if(m_parserSink)
	{
		m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[CTPParser]CTP������������ѶϿ�,ԭ��: %d...", nReason).c_str());
		m_parserSink->handleEvent(WPE_Close, 0);
	}
}

void QuoteParser_CTP::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void QuoteParser_CTP::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = strtoul(pDepthMarketData->ActionDay, NULL, 10);
	uint32_t actTime = strToTime(pDepthMarketData->UpdateTime) * 1000 + pDepthMarketData->UpdateMillisec;
	uint32_t actHour = actTime / 10000000;

	if (actDate == m_uTradingDate && actHour >= 20)
	{
		//������ʱ���������⣬��Ϊҹ��ʱ�������ڲ����ܵ��ڽ�����
		//�����Ҫ�ֶ�����һ��
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		uint32_t curHour = curTime / 10000000;

		//���������Ժ󣬻��յ�����12����ǰ���̵����飬���ʱ����ܻ��з�������=�����յ��������
		//�������ֱ�Ӷ���
		if (curHour >= 3 && curHour < 9)
			return;

		actDate = curDate;

		if (actHour == 23 && curHour == 0)
		{
			//����ʱ������ϵͳʱ��
			actDate = TimeUtils::getNextDate(curDate, -1);
		}
		else if (actHour == 0 && curHour == 23)
		{
			//ϵͳʱ����������ʱ��
			actDate = TimeUtils::getNextDate(curDate, 1);
		}
	}

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID);
	WTSCommodityInfo* pCommInfo = m_pBaseDataMgr->getCommodity(contract);

	//if (strcmp(contract->getExchg(), "CZCE") == 0)
	//{
	//	actTime += (uint32_t)(TimeUtils::getLocalTimeNow() % 1000);
	//}

	WTSTickData* tick = WTSTickData::create(pDepthMarketData->InstrumentID);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());
	
	quote.action_date = actDate;
	quote.action_time = actTime;
	
	quote.price = checkValid(pDepthMarketData->LastPrice);
	quote.open = checkValid(pDepthMarketData->OpenPrice);
	quote.high = checkValid(pDepthMarketData->HighestPrice);
	quote.low = checkValid(pDepthMarketData->LowestPrice);
	quote.total_volumn = pDepthMarketData->Volume;
	quote.trading_date = m_uTradingDate;
	if(pDepthMarketData->SettlementPrice != DBL_MAX)
		quote.settle_price = checkValid(pDepthMarketData->SettlementPrice);
	if(strcmp(quote.exchg, "CZCE") == 0)
	{
		quote.total_turnover = pDepthMarketData->Turnover*pCommInfo->getVolScale();
	}
	else
	{
		if(pDepthMarketData->Turnover != DBL_MAX)
			quote.total_turnover = pDepthMarketData->Turnover;
	}

	quote.open_interest = (uint32_t)pDepthMarketData->OpenInterest;

	quote.upper_limit = checkValid(pDepthMarketData->UpperLimitPrice);
	quote.lower_limit = checkValid(pDepthMarketData->LowerLimitPrice);

	quote.pre_close = checkValid(pDepthMarketData->PreClosePrice);
	quote.pre_settle = checkValid(pDepthMarketData->PreSettlementPrice);
	quote.pre_interest = (uint32_t)pDepthMarketData->PreOpenInterest;

	//ί���۸�
	quote.ask_prices[0] = checkValid(pDepthMarketData->AskPrice1);
	quote.ask_prices[1] = checkValid(pDepthMarketData->AskPrice2);
	quote.ask_prices[2] = checkValid(pDepthMarketData->AskPrice3);
	quote.ask_prices[3] = checkValid(pDepthMarketData->AskPrice4);
	quote.ask_prices[4] = checkValid(pDepthMarketData->AskPrice5);

	//ί��۸�
	quote.bid_prices[0] = checkValid(pDepthMarketData->BidPrice1);
	quote.bid_prices[1] = checkValid(pDepthMarketData->BidPrice2);
	quote.bid_prices[2] = checkValid(pDepthMarketData->BidPrice3);
	quote.bid_prices[3] = checkValid(pDepthMarketData->BidPrice4);
	quote.bid_prices[4] = checkValid(pDepthMarketData->BidPrice5);

	//ί����
	quote.ask_qty[0] = pDepthMarketData->AskVolume1;
	quote.ask_qty[1] = pDepthMarketData->AskVolume2;
	quote.ask_qty[2] = pDepthMarketData->AskVolume3;
	quote.ask_qty[3] = pDepthMarketData->AskVolume4;
	quote.ask_qty[4] = pDepthMarketData->AskVolume5;

	//ί����
	quote.bid_qty[0] = pDepthMarketData->BidVolume1;
	quote.bid_qty[1] = pDepthMarketData->BidVolume2;
	quote.bid_qty[2] = pDepthMarketData->BidVolume3;
	quote.bid_qty[3] = pDepthMarketData->BidVolume4;
	quote.bid_qty[4] = pDepthMarketData->BidVolume5;

	if(m_parserSink)
		m_parserSink->handleQuote(tick, true);

	tick->release();
}

void QuoteParser_CTP::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		//if(m_parserSink)
		//	m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[CTPParser]ʵʱ���鶩�ĳɹ�,��Լ����:%s", pSpecificInstrument->InstrumentID).c_str());
	}
	else
	{
		//if(m_parserSink)
		//	m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[CTPParser]ʵʱ���鶩��ʧ��,��Լ����:%s", pSpecificInstrument->InstrumentID).c_str());
	}
}

void QuoteParser_CTP::OnHeartBeatWarning( int nTimeLapse )
{
	if(m_parserSink)
		m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[CTPParser]������, ʱ��: %d...", nTimeLapse).c_str());
}

void QuoteParser_CTP::ReqUserLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUserID.c_str());
	strcpy(req.Password, m_strPassword.c_str());
	int iResult = m_pUserAPI->ReqUserLogin(&req, ++m_iRequestID);
	if(iResult != 0)
	{
		if(m_parserSink)
			m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[CTPParser]CTP ��¼������ʧ��, ������:%d", iResult).c_str());
	}
}

void QuoteParser_CTP::SubscribeMarketData()
{
	
	CodeSet codeFilter = m_filterSubs;
	if(codeFilter.empty())
	{//����������ֻ�յ�,��ȡ��ȫ����Լ�б�
		return;
	}

	char ** subscribe = new char*[codeFilter.size()];
	int nCount = 0;
	CodeSet::iterator it = codeFilter.begin();
	for(;it != codeFilter.end(); it++)
	{
		subscribe[nCount++] = (char*)(*it).c_str();
	}

	if(m_pUserAPI && nCount > 0)
	{
		int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount);
		if(iResult != 0)
		{
			if(m_parserSink)
				m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[CTPParser]CTP ���鶩��������ʧ��, ������:%d", iResult).c_str());
		}
		else
		{
			if(m_parserSink)
				m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[CTPParser]һ������ %d ����Լ����", nCount).c_str());
		}
	}
	codeFilter.clear();
	delete[] subscribe;
}

bool QuoteParser_CTP::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	return false;
}

void QuoteParser_CTP::subscribe(const CodeSet &vecSymbols)
{
	if(m_uTradingDate == 0)
	{
		m_filterSubs = vecSymbols;
	}
	else
	{
		m_filterSubs = vecSymbols;
		char * subscribe[500] = {NULL};
		int nCount = 0;
		CodeSet::const_iterator it = vecSymbols.begin();
		for(;it != vecSymbols.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if(m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount);
			if(iResult != 0)
			{
				if(m_parserSink)
					m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[CTPParser]CTP ���鶩��������ʧ��, ������:%d", iResult).c_str());
			}
			else
			{
				if(m_parserSink)
					m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[CTPParser]һ������ %d ����Լ����", nCount).c_str());
			}
		}
	}
}

void QuoteParser_CTP::unsubscribe(const CodeSet &vecSymbols)
{
}

bool QuoteParser_CTP::isConnected()
{
	return m_pUserAPI!=NULL;
}

void QuoteParser_CTP::registerListener(IParserApiListener* listener)
{
	m_parserSink = listener;

	if(m_parserSink)
		m_pBaseDataMgr = m_parserSink->getBaseDataMgr();
}