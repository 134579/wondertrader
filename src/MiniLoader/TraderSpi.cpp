#include <iostream>
#include <map>
#include <set>
#include <stdint.h>
#include <fstream>

#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "../Share/StrUtil.hpp"
#include "../Includes/WTSTypes.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include "TraderSpi.h"


USING_NS_OTP;

extern std::map<std::string, std::string>	MAP_NAME;
extern std::map<std::string, std::string>	MAP_SESSION;

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcTraderApi* pUserApi;

// 配置参数
extern std::string	FRONT_ADDR;	// 前置地址
extern std::string	BROKER_ID;	// 经纪公司代码
extern std::string	INVESTOR_ID;// 投资者代码
extern std::string	PASSWORD;	// 用户密码
extern std::string	SAVEPATH;	//保存位置
extern std::string	HOTFILE;
extern std::string	APPID;
extern std::string	AUTHCODE;
extern bool			ISFOROPTION;

extern std::string COMM_FILE;		//输出的品种文件名
extern std::string CONT_FILE;		//输出的合约文件名

// 请求编号
extern int iRequestID;

// 会话参数
TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

typedef struct _Commodity
{
	std::string	m_strName;
	std::string	m_strExchg;
	std::string	m_strProduct;
	std::string	m_strCurrency;
	std::string m_strSession;

	uint32_t	m_uVolScale;
	double		m_fPriceTick;
	uint32_t	m_uPrecision;

	ContractCategory	m_ccCategory;
	CoverMode			m_coverMode;
	PriceMode			m_priceMode;

} Commodity;
typedef std::map<std::string, Commodity> CommodityMap;
CommodityMap _commodities;

typedef struct _Contract
{
	std::string	m_strCode;
	std::string	m_strExchg;
	std::string	m_strName;
	std::string	m_strProduct;


	uint32_t	m_maxMktQty;
	uint32_t	m_maxLmtQty;
} Contract;
typedef std::map<std::string, Contract> ContractMap;
ContractMap _contracts;


std::string extractProductID(const char* instrument)
{
	std::string strRet;
	int nLen = 0;
	while('A' <= instrument[nLen] && instrument[nLen] <= 'z')
	{
		strRet += instrument[nLen];
		nLen++;
	}

	return strRet;
}

std::string extractProductName(const char* cname)
{
	std::string strRet;
	int idx = strlen(cname) - 1;
	while (isdigit(cname[idx]) && idx > 0)
	{
		idx--;
	}

	strRet.append(cname, idx + 1);
	return strRet;
}

std::set<std::string>	prod_set;

double convertInvalidDouble(double val)
{
	if(val == 5.5e-007)
		return -1;

	if(val == 0)
		return -1;

	return val;
}

void CTraderSpi::OnFrontConnected()
{
	std::cerr << "--->>> " << "OnFrontConnected" << std::endl;
	///用户登录请求
	ReqAuth();
}

void CTraderSpi::ReqAuth()
{
	CThostFtdcReqAuthenticateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID.c_str());
	strcpy(req.UserID, INVESTOR_ID.c_str());
	//strcpy(req.UserProductInfo, m_strProdInfo.c_str());
	strcpy(req.AuthCode, AUTHCODE.c_str());
	strcpy(req.AppID, APPID.c_str());
	int iResult = pUserApi->ReqAuthenticate(&req, ++iRequestID);
	std::cerr << "--->>> 发送终端认证录请求: " << ((iResult == 0) ? "成功" : "失败") << std::endl;
}

void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << "OnRspAuthenticate" << std::endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		ReqUserLogin();
	}
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID.c_str());
	strcpy(req.UserID, INVESTOR_ID.c_str());
	strcpy(req.Password, PASSWORD.c_str());
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	std::cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << std::endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << "OnRspUserLogin" << std::endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///获取当前交易日
		m_lTradingDate = atoi(pUserApi->GetTradingDay());
		
		ReqQryInstrument();
	}
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
	std::cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << std::endl;
}

inline bool isOption(TThostFtdcProductClassType pClass)
{
	if (pClass == THOST_FTDC_PC_Options || pClass == THOST_FTDC_PC_SpotOption)
		return true;
	
	return false;
}

inline bool isFuture(TThostFtdcProductClassType pClass)
{
	return pClass == THOST_FTDC_PC_Futures;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo))
	{
		if (pInstrument && ISFOROPTION?isOption(pInstrument->ProductClass):isFuture(pInstrument->ProductClass))
		{		
			std::cerr << "--->>> OnRspQryInstrument: " << pInstrument->ExchangeID << "." << pInstrument->InstrumentID << std::endl;
			std::string pname = MAP_NAME[pInstrument->ProductID];
			std::string cname = "";
			if (pname.empty())
			{
				cname = pInstrument->InstrumentName;
				pname = ISFOROPTION ? pInstrument->InstrumentName : extractProductName(pInstrument->InstrumentName);
			}
			else
			{
				if(ISFOROPTION)
				{
					cname = pInstrument->InstrumentName;
				}
				else
				{
					std::string month = pInstrument->InstrumentID;
					month = month.substr(strlen(pInstrument->ProductID));
					cname = pname + month;
				}
			}

			Contract contract;
			contract.m_strCode = pInstrument->InstrumentID;
			contract.m_strExchg = pInstrument->ExchangeID;
			contract.m_strName = cname;
			contract.m_strProduct = pInstrument->ProductID;
			contract.m_maxMktQty = pInstrument->MaxMarketOrderVolume;
			contract.m_maxLmtQty = pInstrument->MaxLimitOrderVolume;

			std::string key = StrUtil::printf("%s.%s", pInstrument->ExchangeID, pInstrument->ProductID);
			auto it = _commodities.find(key);
			if(it == _commodities.end())
			{
				Commodity commInfo;
				commInfo.m_strProduct = pInstrument->ProductID;
				commInfo.m_strName = pname;
				commInfo.m_strExchg = pInstrument->ExchangeID;
				commInfo.m_strCurrency = "CNY";

				commInfo.m_strSession = MAP_SESSION[key];
				commInfo.m_ccCategory = ISFOROPTION ? CC_Option : CC_Future;

				commInfo.m_uVolScale = (pInstrument->VolumeMultiple == 0 ? 1 : pInstrument->VolumeMultiple);
				commInfo.m_fPriceTick = pInstrument->PriceTick;

				CoverMode cm = CM_OpenCover;
				if (!ISFOROPTION)
				{
					if (strcmp(pInstrument->ExchangeID, "SHFE") == 0 || strcmp(pInstrument->ExchangeID, "INE") == 0)
						cm = CM_CoverToday;
					//上期所的就是平今，非上期所的就是开平
				}
				
				commInfo.m_coverMode = cm;

				PriceMode pm = PM_Both;
				if (!ISFOROPTION)
				{
					if (strcmp(pInstrument->ExchangeID, "SHFE") == 0 || strcmp(pInstrument->ExchangeID, "INE") == 0)
						pm = PM_Limit;
				}
				commInfo.m_priceMode = pm;

				if (pInstrument->PriceTick < 0.01)
					commInfo.m_uPrecision = 3;
				else if (pInstrument->PriceTick < 0.1)
					commInfo.m_uPrecision = 2;
				else if (pInstrument->PriceTick < 1)
					commInfo.m_uPrecision = 1;
				else
					commInfo.m_uPrecision = 0;

				_commodities[key] = commInfo;
			}
			
			key = StrUtil::printf("%s.%s", pInstrument->ExchangeID, pInstrument->InstrumentID);
			_contracts[key] = contract;
		}
	}

	if(bIsLast)
	{
		DumpToJson();
		exit(0);
	}
}

void CTraderSpi::DumpToJson()
{
	//两个文件，一个contracts.json,一个commodities.json
	//Json::Value jComms(Json::objectValue);
	rj::Document jComms(rj::kObjectType);
	{
		rj::Document::AllocatorType &allocator = jComms.GetAllocator();
		for (auto it = _commodities.begin(); it != _commodities.end(); it++)
		{
			const Commodity& commInfo = it->second;
			if (!jComms.HasMember(commInfo.m_strExchg.c_str()))
			{
				//jComms[commInfo.m_strExchg] = Json::Value(Json::objectValue);
				jComms.AddMember(rj::Value(commInfo.m_strExchg.c_str(), allocator), rj::Value(rj::kObjectType), allocator);
			}

			rj::Value jComm(rj::kObjectType);
			jComm.AddMember("covermode", (uint32_t)commInfo.m_coverMode, allocator);
			jComm.AddMember("pricemode", (uint32_t)commInfo.m_priceMode, allocator);
			jComm.AddMember("category", (uint32_t)commInfo.m_priceMode, allocator);
			jComm.AddMember("precision", commInfo.m_uPrecision, allocator);
			jComm.AddMember("pricetick", commInfo.m_fPriceTick, allocator);
			jComm.AddMember("volscale", commInfo.m_uVolScale, allocator);

			jComm.AddMember("name", rj::Value(commInfo.m_strName.c_str(), allocator), allocator);
			jComm.AddMember("exchg", rj::Value(commInfo.m_strExchg.c_str(), allocator), allocator);
			jComm.AddMember("session", rj::Value(commInfo.m_strSession.c_str(), allocator), allocator);
			jComm.AddMember("holiday", rj::Value("CHINA", allocator), allocator);

			//jComms[commInfo.m_strExchg][commInfo.m_strProduct] = jComm;
			jComms[commInfo.m_strExchg.c_str()].AddMember(rj::Value(commInfo.m_strProduct.c_str(), allocator), jComm, allocator);
		}
	}

	//Json::Value jContracts(Json::objectValue);
	rj::Document jContracts(rj::kObjectType);
	{
		rj::Document::AllocatorType &allocator = jContracts.GetAllocator();
		for (auto it = _contracts.begin(); it != _contracts.end(); it++)
		{
			const Contract& cInfo = it->second;
			if (!jContracts.HasMember(cInfo.m_strExchg.c_str()))
			{
				//jComms[commInfo.m_strExchg] = Json::Value(Json::objectValue);
				jContracts.AddMember(rj::Value(cInfo.m_strExchg.c_str(), allocator), rj::Value(rj::kObjectType), allocator);
			}

			//Json::Value jcInfo(Json::objectValue);
			rj::Value jcInfo(rj::kObjectType);
			//jcInfo["name"] = cInfo.m_strName;
			//jcInfo["code"] = cInfo.m_strCode;
			//jcInfo["exchg"] = cInfo.m_strExchg;
			//jcInfo["product"] = cInfo.m_strProduct;

			jcInfo.AddMember("name", rj::Value(cInfo.m_strName.c_str(), allocator), allocator);
			jcInfo.AddMember("code", rj::Value(cInfo.m_strCode.c_str(), allocator), allocator);
			jcInfo.AddMember("exchg", rj::Value(cInfo.m_strExchg.c_str(), allocator), allocator);
			jcInfo.AddMember("product", rj::Value(cInfo.m_strProduct.c_str(), allocator), allocator);

			//jcInfo["maxlimitqty"] = cInfo.m_maxLmtQty;
			//jcInfo["maxmarketqty"] = cInfo.m_maxMktQty;
			jcInfo.AddMember("maxlimitqty", cInfo.m_maxLmtQty, allocator);
			jcInfo.AddMember("maxmarketqty", cInfo.m_maxMktQty, allocator);


			//jContracts[cInfo.m_strExchg][cInfo.m_strCode] = jcInfo;
			jContracts[cInfo.m_strExchg.c_str()].AddMember(rj::Value(cInfo.m_strCode.c_str(), allocator), jcInfo, allocator);
		}
	}

	std::ofstream ofs;
	std::string path = SAVEPATH;
	path += COMM_FILE;
	ofs.open(path);
	{
		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		jComms.Accept(writer);
		ofs << sb.GetString();
	}
	ofs.close();

	path = SAVEPATH;
	path += CONT_FILE;
	ofs.open(path);
	{
		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		jContracts.Accept(writer);
		ofs << sb.GetString();
	}
	ofs.close();
}


void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	std::cerr << "--->>> " << "OnFrontDisconnected" << std::endl;
	std::cerr << "--->>> Reason = " << nReason << std::endl;
}
		

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << "OnRspError" << std::endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		std::cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	return bResult;
}