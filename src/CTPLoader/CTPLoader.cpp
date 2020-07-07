#include <string>
#include <map>
#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "TraderSpi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/DLLHelper.hpp"
#include <boost/filesystem.hpp>



// UserApi����
CThostFtdcTraderApi* pUserApi;

// ���ò���
std::string	FRONT_ADDR;	// ǰ�õ�ַ
std::string	BROKER_ID;	// ���͹�˾����
std::string	INVESTOR_ID;// Ͷ���ߴ���
std::string	PASSWORD;	// �û�����
std::string SAVEPATH;	//����λ��
std::string APPID;
std::string AUTHCODE;
bool		ISFOROPTION;	//��Ȩ

std::string COMM_FILE;		//�����Ʒ���ļ���
std::string CONT_FILE;		//����ĺ�Լ�ļ���

typedef std::map<std::string, std::string>	SymbolMap;
SymbolMap	MAP_NAME;
SymbolMap	MAP_SESSION;

// ������
int iRequestID = 0;

std::string getBaseFolder()
{
	static std::string basePath;
	if(basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath;
}

int main()
{
#ifdef _WIN32
#else
	DLLHelper::load_library("libthosttraderapi_se.so");
#endif

	std::string cfg = getBaseFolder() + "config.ini";
	IniHelper ini;
	ini.load(cfg.c_str());

	FRONT_ADDR = ini.readString("ctp", "front", "");
	BROKER_ID	= ini.readString("ctp", "broker", "");
	INVESTOR_ID = ini.readString("ctp", "user", "");
	PASSWORD	= ini.readString("ctp", "pass", "");
	APPID = ini.readString("ctp", "appid", "");
	AUTHCODE = ini.readString("ctp", "authcode", "");

	SAVEPATH	= ini.readString("config", "path", "");
	ISFOROPTION = ini.readInt("config", "option", 0) == 1;

	COMM_FILE = ini.readString("config", "commfile", "commodities.json");
	CONT_FILE = ini.readString("config", "contfile", "contracts.json");

	if(FRONT_ADDR.empty() || BROKER_ID.empty() || INVESTOR_ID.empty() || PASSWORD.empty() || SAVEPATH.empty())
	{
		return 0;
	}

	SAVEPATH = StrUtil::standardisePath(SAVEPATH);

	FieldArray ayKeys, ayVals;
	int cout = ini.readSecKeyValArray("Name", ayKeys, ayVals);
	for (int i = 0; i < cout; i++)
	{
		MAP_NAME[ayKeys[i]] = ayVals[i];
		printf("Ʒ������ӳ�䣺%s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
	}

	ayKeys.clear();
	ayVals.clear();
	cout = ini.readSecKeyValArray("Session", ayKeys, ayVals);
	for (int i = 0; i < cout; i++)
	{
		MAP_SESSION[ayKeys[i]] = ayVals[i];
		printf("����ʱ��ӳ�䣺%s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
	}

	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);					// ע��˽����
	pUserApi->RegisterFront((char*)FRONT_ADDR.c_str());				// connect
	pUserApi->Init();

	pUserApi->Join();
	return 0;
}