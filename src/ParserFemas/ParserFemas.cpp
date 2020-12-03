/*!
 * \file ParserFemas.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserFemas.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Share/StdUtils.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSParams.hpp"
#include "../Share/StrUtil.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"

#include <boost/filesystem.hpp>

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#ifdef _WIN32
#include <wtypes.h>
HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}
#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void *)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif


std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = g_moduleName;
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir;
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserFemas* parser = new ParserFemas();
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

ParserFemas::ParserFemas()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserFemas::~ParserFemas()
{
	m_pUserAPI = NULL;
}

bool ParserFemas::init(WTSParams* config)
{
	m_strFrontAddr = config->getCString("front");
	m_strBroker = config->getCString("broker");
	m_strUserID = config->getCString("user");
	m_strPassword = config->getCString("pass");

	std::string module = config->getCString("ctpmodule");
	if (module.empty())
	{

#ifdef _WIN32
		module = "USTPmduserapiAF.dll";
#else
		module = "libUSTPmduserapiAF.so";
#endif
	}
	std::string dllpath = getBinDir() + module;
	std::string path = StrUtil::printf("FemasParserFlow/%s/%s/", m_strBroker.c_str(), m_strUserID.c_str());
	if (!StdFile::exists(path.c_str()))
	{
		boost::filesystem::create_directories(boost::filesystem::path(path));
	}
	m_hInst = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateFtdcMduserApi@CUstpFtdcMduserApi@@SAPEAV1@PEBD@Z";
#	else
	const char* creatorName = "?CreateFtdcMduserApi@CUstpFtdcMduserApi@@SAPAV1@PBD@Z";
#	endif
#else
	const char* creatorName = "_ZN18CUstpFtdcMduserApi19CreateFtdcMduserApiEPKc";
#endif
	m_funcCreator = (FemasCreator)DLLHelper::get_symbol(m_hInst, creatorName);
	m_pUserAPI = m_funcCreator(path.c_str());
	m_pUserAPI->RegisterSpi(this);
	m_pUserAPI->RegisterFront((char*)m_strFrontAddr.c_str());

	return true;
}

void ParserFemas::release()
{
	disconnect();
}

bool ParserFemas::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool ParserFemas::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserFemas::OnRspError( CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	IsErrorRspInfo(pRspInfo);
}

void ParserFemas::OnFrontConnected()
{
	if(m_parserSink)
	{
		m_parserSink->handleParserLog(LL_INFO, "[ParserFemas]行情服务已连接");
		m_parserSink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void ParserFemas::OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_uTradingDate = strtoul(m_pUserAPI->GetTradingDay(), NULL, 10);
		
		if(m_parserSink)
		{
			m_parserSink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		SubscribeMarketData();
	}
}

void ParserFemas::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_parserSink)
	{
		m_parserSink->handleEvent(WPE_Logout, 0);
	}
}

void ParserFemas::OnFrontDisconnected( int nReason )
{
	if(m_parserSink)
	{
		m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserFemas]行情服务连接已断开,原因: %d...", nReason).c_str());
		m_parserSink->handleEvent(WPE_Close, 0);
	}
}

void ParserFemas::OnRspUnSubMarketData( CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void ParserFemas::OnRtnDepthMarketData( CUstpFtdcDepthMarketDataField *pDepthMarketData )
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
		//这样的时间是有问题，因为夜盘时发生日期不可能等于交易日
		//这就需要手动设置一下
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		uint32_t curHour = curTime / 10000000;

		//早上启动以后，会收到昨晚12点以前收盘的行情，这个时候可能会有发生日期=交易日的情况出现
		//这笔数据直接丢掉
		if (curHour >= 3 && curHour < 9)
			return;

		actDate = curDate;

		if (actHour == 23 && curHour == 0)
		{
			//行情时间慢于系统时间
			actDate = TimeUtils::getNextDate(curDate, -1);
		}
		else if (actHour == 0 && curHour == 23)
		{
			//系统时间慢于行情时间
			actDate = TimeUtils::getNextDate(curDate, 1);
		}
	}

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(pDepthMarketData->InstrumentID);
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

	//委卖价格
	quote.ask_prices[0] = checkValid(pDepthMarketData->AskPrice1);
	quote.ask_prices[1] = checkValid(pDepthMarketData->AskPrice2);
	quote.ask_prices[2] = checkValid(pDepthMarketData->AskPrice3);
	quote.ask_prices[3] = checkValid(pDepthMarketData->AskPrice4);
	quote.ask_prices[4] = checkValid(pDepthMarketData->AskPrice5);

	//委买价格
	quote.bid_prices[0] = checkValid(pDepthMarketData->BidPrice1);
	quote.bid_prices[1] = checkValid(pDepthMarketData->BidPrice2);
	quote.bid_prices[2] = checkValid(pDepthMarketData->BidPrice3);
	quote.bid_prices[3] = checkValid(pDepthMarketData->BidPrice4);
	quote.bid_prices[4] = checkValid(pDepthMarketData->BidPrice5);

	//委卖量
	quote.ask_qty[0] = pDepthMarketData->AskVolume1;
	quote.ask_qty[1] = pDepthMarketData->AskVolume2;
	quote.ask_qty[2] = pDepthMarketData->AskVolume3;
	quote.ask_qty[3] = pDepthMarketData->AskVolume4;
	quote.ask_qty[4] = pDepthMarketData->AskVolume5;

	//委买量
	quote.bid_qty[0] = pDepthMarketData->BidVolume1;
	quote.bid_qty[1] = pDepthMarketData->BidVolume2;
	quote.bid_qty[2] = pDepthMarketData->BidVolume3;
	quote.bid_qty[3] = pDepthMarketData->BidVolume4;
	quote.bid_qty[4] = pDepthMarketData->BidVolume5;

	if(m_parserSink)
		m_parserSink->handleQuote(tick, true);

	tick->release();
}

void ParserFemas::OnRspSubMarketData( CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		//if(m_parserSink)
		//	m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[ParserFemas]实时行情订阅成功,合约代码:%s", pSpecificInstrument->InstrumentID).c_str());
	}
	else
	{
		//if(m_parserSink)
		//	m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[ParserFemas]实时行情订阅失败,合约代码:%s", pSpecificInstrument->InstrumentID).c_str());
	}
}

void ParserFemas::OnHeartBeatWarning( int nTimeLapse )
{
	if(m_parserSink)
		m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[ParserFemas]心跳包, 时长: %d...", nTimeLapse).c_str());
}

void ParserFemas::ReqUserLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	CUstpFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUserID.c_str());
	strcpy(req.Password, m_strPassword.c_str());
	int iResult = m_pUserAPI->ReqUserLogin(&req, ++m_iRequestID);
	if(iResult != 0)
	{
		if(m_parserSink)
			m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserFemas]登录请求发送失败, 错误码:%d", iResult).c_str());
	}
}

void ParserFemas::SubscribeMarketData()
{
	CodeSet codeFilter = m_filterSubs;
	if(codeFilter.empty())
	{//如果订阅礼包只空的,则取出全部合约列表
		return;
	}

	char ** subscribe = new char*[codeFilter.size()];
	int nCount = 0;
	for(auto& code : codeFilter)
	{
		std::size_t pos = code.find(".");
		if (pos != std::string::npos)
			subscribe[nCount++] = (char*)code.c_str() + pos + 1;
		else
			subscribe[nCount++] = (char*)code.c_str();
	}

	if(m_pUserAPI && nCount > 0)
	{
		int iResult = m_pUserAPI->SubMarketData(subscribe, nCount);
		if(iResult != 0)
		{
			if(m_parserSink)
				m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserFemas]行情订阅请求发送失败, 错误码:%d", iResult).c_str());
		}
		else
		{
			if(m_parserSink)
				m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[ParserFemas]一共订阅 %d 个合约行情", nCount).c_str());
		}
	}
	codeFilter.clear();
	delete[] subscribe;
}

bool ParserFemas::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	return false;
}

void ParserFemas::subscribe(const CodeSet &vecSymbols)
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
		for (auto& code  : vecSymbols)
		{
			std::size_t pos = code.find(".");
			if (pos != std::string::npos)
				subscribe[nCount++] = (char*)code.c_str() + pos + 1;
			else
				subscribe[nCount++] = (char*)code.c_str();
		}

		if(m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubMarketData(subscribe, nCount);
			if(iResult != 0)
			{
				if(m_parserSink)
					m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserFemas]行情订阅请求发送失败, 错误码:%d", iResult).c_str());
			}
			else
			{
				if(m_parserSink)
					m_parserSink->handleParserLog(LL_INFO, StrUtil::printf("[ParserFemas]一共订阅 %d 个合约行情", nCount).c_str());
			}
		}
	}
}

void ParserFemas::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserFemas::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserFemas::registerListener(IParserApiListener* listener)
{
	m_parserSink = listener;

	if(m_parserSink)
		m_pBaseDataMgr = m_parserSink->getBaseDataMgr();
}