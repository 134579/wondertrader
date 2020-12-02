#include <string>
#include <map>
//v6.3.15
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

std::string MODULE_NAME;	//�ⲿģ����

typedef std::map<std::string, std::string>	SymbolMap;
SymbolMap	MAP_NAME;
SymbolMap	MAP_SESSION;

typedef CThostFtdcTraderApi* (*CTPCreator)(const char *);
CTPCreator		g_ctpCreator = NULL;

// ������
int iRequestID = 0;

std::string getBaseFolder()
{
	static std::string basePath;
	if(basePath.empty())
	{
#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(NULL, strPath, MAX_PATH);

		basePath = StrUtil::standardisePath(strPath, false);

		uint32_t nPos = basePath.find_last_of('/');
		basePath = basePath.substr(0, nPos + 1);
#else

		char path[1024];
		int cnt = readlink("/proc/self/exe", path, 1024);
		//���һ��'/' �����ǿ�ִ�г�������ȥ����ִ�г�������֣�ֻ����·��
		for (int i = cnt; i >= 0; --i)
		{
			if (path[i] == '/')
			{
				path[i + 1] = '\0';
				break;
			}
		}
		basePath = path;
		basePath = StrUtil::standardisePath(basePath, false);
#endif
	}

	return basePath;
}

int main()
{
	std::string cfg = "config.ini";
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

#ifdef _WIN32
	MODULE_NAME = ini.readString("config", "module", "thosttraderapi.dll");
#else
	MODULE_NAME = ini.readString("config", "module", "libthosttraderapi.so");
#endif
	if(!boost::filesystem::exists(MODULE_NAME.c_str()))
	{
#ifdef _WIN32
		MODULE_NAME = getBaseFolder() + "traders/thosttraderapi.dll";
#else
		MODULE_NAME = getBaseFolder() + "traders/libthosttraderapi.so";
#endif
	}

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
	DllHandle dllInst = DLLHelper::load_library(MODULE_NAME.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPEAV1@PEBD@Z";
#	else
	const char* creatorName = "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPAV1@PBD@Z";
#	endif
#else
	const char* creatorName = "_ZN19CThostFtdcTraderApi19CreateFtdcTraderApiEPKc";
#endif
	g_ctpCreator = (CTPCreator)DLLHelper::get_symbol(dllInst, creatorName);
	pUserApi = g_ctpCreator("");			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);					// ע��˽����
	pUserApi->RegisterFront((char*)FRONT_ADDR.c_str());				// connect
	pUserApi->Init();

	pUserApi->Join();
	return 0;
}