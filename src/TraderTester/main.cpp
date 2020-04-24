#include <windows.h>
#include <iostream>
#include <mutex>

#include "..\Share\ITraderApi.h"
#include "..\Share\IniFile.hpp"
#include "..\Share\WTSParams.hpp"
#include "..\Share\WTSTradeDef.hpp"
#include "..\Share\WTSError.hpp"
#include "..\Share\TimeUtils.hpp"
#include "..\Share\StdUtils.hpp"
#include "..\Share\WTSCollection.hpp"

#include "..\WTSTools\WTSBaseDataMgr.h"
#include "..\WTSTools\WTSLogger.h"


WTSBaseDataMgr	g_bdMgr;
StdUniqueMutex	g_mtxOpt;
StdCondVariable	g_condOpt;
bool		g_exitNow = false;

USING_NS_OTP;

typedef enum tagTradeAccountType
{
	TAT_CTP,		//CTP�ӿ�
	TAT_CTPTest,	//CTP����
	TAT_Femas,
	TAT_CTPMini,
	TAT_CTPMiniTest
}TraderType;

void log(const char* fmt, ...)
{
	//char szBuf[512] = { 0 };
	va_list args;
	va_start(args, fmt);
	WTSLogger::log_direct(LL_INFO, fmt, args);
	va_end(args);

	//printf(szBuf);
	//printf("\r\n");
	
}

void log_raw(const char* message)
{
	printf(message);
	printf("\r\n");
	//WTSLogger::info(message);
}

class TraderSpi : public ITraderApiListener
{
public:
	TraderSpi() :m_bLogined(false), m_mapOrds(NULL){}

	bool init(WTSParams* params, const char* ttype)
	{
		m_pParams = params;
		if (m_pParams)
			m_pParams->retain();

		m_strModule = ttype;
		return true;
	}

	void release()
	{
		if (m_pTraderApi)
		{
			m_pTraderApi->release();
		}
	}

	void run(bool bRestart = false)
	{
		if (!createTrader(m_strModule.c_str()))
		{
			return;
		}

		m_pTraderApi->registerListener(this);

		if (!m_pTraderApi->init(m_pParams))
		{
			return;
		}

		log("[%s-%s]��ʼ���ӷ���ˣ�%s", m_pParams->getCString("broker"), m_pParams->getCString("user"), m_pParams->getCString("front"));
		m_pTraderApi->connect();
	}

	bool createTrader(const char* moduleName)
	{
		HINSTANCE hInst = LoadLibrary(moduleName);
		if (hInst == NULL)
		{
			log("����ģ��%s����ʧ��", moduleName);
			return false;
		}

		FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)GetProcAddress(hInst, "createTrader");
		if (NULL == pFunCreateTrader)
		{
			log("���׽ӿڴ���������ȡʧ��");
			return false;
		}

		m_pTraderApi = pFunCreateTrader();
		if (NULL == m_pTraderApi)
		{
			log("���׽ӿڴ���ʧ��");
			return false;
		}

		m_funcDelTrader = (FuncDeleteTrader)GetProcAddress(hInst, "deleteTrader");
		return true;
	}

	bool qryFund()
	{
		log("[%s-%s]���ڲ�ѯ�ʽ𡭡�", m_pParams->getCString("broker"), m_pParams->getCString("user"));
		m_pTraderApi->queryAccount();

		return true;
	}

	bool qryOrders()
	{
		log("[%s-%s]���ڲ�ѯ����ί�С���", m_pParams->getCString("broker"), m_pParams->getCString("user"));
		m_pTraderApi->queryOrders();

		return true;
	}

	bool qryTrades()
	{
		log("[%s-%s]���ڲ�ѯ���ճɽ�����", m_pParams->getCString("broker"), m_pParams->getCString("user"));
		m_pTraderApi->queryTrades();

		return true;
	}

	bool qryPosition()
	{
		log("[%s-%s]���ڲ�ѯ�ֲ֡���", m_pParams->getCString("broker"), m_pParams->getCString("user"));
		m_pTraderApi->queryPositions();

		return true;
	}

	bool qrySettle()
	{
		uint32_t uDate = TimeUtils::getNextDate(TimeUtils::getCurDate(), -1);
		log("[%s-%s]���ڲ�ѯ%u�Ľ��㵥����", m_pParams->getCString("broker"), m_pParams->getCString("user"), uDate);
		m_pTraderApi->querySettlement(uDate);

		return true;
	}

	bool entrustLmt()
	{
		char code[32] = { 0 };
		double price = 0.0;
		uint32_t qty = 0;

		for (;;)
		{
			printf("�������Լ����: ");
			std::cin >> code;

			printf("������ί�м۸�: ");
			std::cin >> price;

			printf("����������: ");
			std::cin >> qty;

			printf("��Լ��%s���۸�%f��������%u��ȷ��y/n? ", code, price, qty);
			char c;
			std::cin >> c;
			if(c == 'y')
				break;
		}

		WTSEntrust* entrust = WTSEntrust::create(code, qty, price, "SHFE");
		entrust->setDirection(WDT_LONG);
		entrust->setOffsetType(WOT_OPEN);
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		log("[%s-%s]��ʼ�µ�����Լ: %s���۸�: %f������: %d������: ����", m_pParams->getCString("broker"), m_pParams->getCString("user"), entrust->getCode(), entrust->getPrice(), entrust->getVolumn());

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool entrustMkt()
	{
		char code[32] = { 0 };
		uint32_t qty = 0;

		for (;;)
		{
			printf("�������Լ����: ");
			std::cin >> code;

			printf("����������: ");
			std::cin >> qty;

			printf("��Լ��%s��������%u��ȷ��y/n? ", code, qty);
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		WTSEntrust* entrust = WTSEntrust::create(code, qty, 0, "SZSE");
		entrust->setDirection(WDT_LONG);
		entrust->setOffsetType(WOT_OPEN);
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		log("[%s-%s]��ʼ�µ�����Լ: %s���۸�: �мۣ�����: %d������: ����", m_pParams->getCString("broker"), m_pParams->getCString("user"), entrust->getCode(), entrust->getVolumn());

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool cancel()
	{
		char orderid[128] = { 0 };

		for (;;)
		{
			printf("�����붩��ID: ");
			std::cin >> orderid;

			printf("����ID��%s��ȷ��y/n? ", orderid);
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		if (m_mapOrds == NULL)
			m_mapOrds = WTSObjectMap::create();

		WTSOrderInfo* ordInfo = (WTSOrderInfo*)m_mapOrds->get(orderid);
		if (ordInfo == NULL)
		{
			printf("���������ڣ����鶩�����Ƿ����󣬻����Ȳ�ѯ����\r\n");
			return false;
		}


		log("[%s-%s]��ʼ����[%s]����", m_pParams->getCString("broker"), m_pParams->getCString("user"), orderid);
		WTSEntrustAction* action = WTSEntrustAction::create(ordInfo->getCode());
		action->setEntrustID(ordInfo->getEntrustID());
		action->setOrderID(orderid);
		action->setActionFlag(WAF_CANCEL);

		m_pTraderApi->orderAction(action);
		action->release();

		return true;
	}

public:
	virtual void handleEvent(WTSTraderEvent e, int32_t ec)
	{
		if(e == WTE_Connect)
		{
			if (ec == 0)
			{
				log("[%s-%s]���ӳɹ�", m_pParams->getCString("broker"), m_pParams->getCString("user"));
				m_pTraderApi->login(m_pParams->getCString("user"), m_pParams->getCString("pass"), "");
			}
			else
			{
				g_exitNow = true;
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
	}

	virtual void handleTraderLog(WTSLogLevel ll, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		WTSLogger::log_direct(LL_INFO, format, args);
		va_end(args);
	}

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
	{
		if(bSucc)
		{
			log("[%s-%s]��¼�ɹ�" , m_pParams->getCString("broker"), m_pParams->getCString("user"));	
			m_bLogined = true;
		}
		else
		{
			log("[%s-%s]��¼ʧ�ܣ�%s", m_pParams->getCString("broker"), m_pParams->getCString("user"), msg);
			g_exitNow = true;
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err)
	{
		if(err)
		{
			log("[%s-%s]�µ�ʧ�ܣ�%s", m_pParams->getCString("broker"), m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		
	}

	virtual void onRspAccount(WTSArray* ayAccounts)
	{
		if(ayAccounts != NULL)
		{
			WTSAccountInfo* accInfo = (WTSAccountInfo*)ayAccounts->at(0);
			if(accInfo)
			{
				log("[%s-%s]�ʽ����ݸ���, ��ǰ��̬Ȩ��: %.2f", m_pParams->getCString("broker"), m_pParams->getCString("user"), accInfo->getBalance());
			}
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspPosition(const WTSArray* ayPositions)
	{
		uint32_t cnt = 0;
		if (ayPositions != NULL)
			cnt = ayPositions->size();

		log("[%s-%s]�ֲ������Ѹ���, ���չ���%u�ʳֲ�", m_pParams->getCString("broker"), m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspOrders(const WTSArray* ayOrders)
	{
		uint32_t cnt = 0;
		if (ayOrders != NULL)
			cnt = ayOrders->size();

		if (m_mapOrds == NULL)
			m_mapOrds = WTSObjectMap::create();

		m_mapOrds->clear();
		for (uint32_t i = 0; i < cnt; i++)
		{
			WTSOrderInfo* ordInfo = (WTSOrderInfo*)((WTSArray*)ayOrders)->at(i);
			if (ordInfo->isAlive())
				m_mapOrds->add(ordInfo->getOrderID(), ordInfo, true);
		}

		log("[%s-%s]ί���б��Ѹ���, ���չ���%u��ί��, δ���%u��", m_pParams->getCString("broker"), m_pParams->getCString("user"), cnt, m_mapOrds->size());

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspTrades(const WTSArray* ayTrades)
	{
		uint32_t cnt = 0;
		if (ayTrades != NULL)
			cnt = ayTrades->size();

		log("[%s-%s]�ɽ���ϸ�Ѹ���, ���չ���%u�ʳɽ�", m_pParams->getCString("broker"), m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspSettlementInfo(uint32_t uDate, const char* content)
	{
		log("[%s-%s]%u���㵥�ѽ���", m_pParams->getCString("broker"), m_pParams->getCString("user"), uDate);
		log_raw(content);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onPushOrder(WTSOrderInfo* orderInfo)
	{
		if(orderInfo->getOrderState() != WOS_Canceled)
		{
			if(strlen(orderInfo->getOrderID()) > 0)
			{
				if (m_mapOrds == NULL)
					m_mapOrds = WTSObjectMap::create();

				if (m_mapOrds->find(orderInfo->getOrderID()) == m_mapOrds->end())
				{
					log("[%s-%s]�µ��ɹ�������ID: %s", m_pParams->getCString("broker"), m_pParams->getCString("user"), orderInfo->getOrderID());
					m_mapOrds->add(orderInfo->getOrderID(), orderInfo, true);
				}

				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
		else 
		{
			if (m_mapOrds)
				m_mapOrds->remove(orderInfo->getOrderID());

			if (strlen(orderInfo->getOrderID()) == 0)
			{
				log("[%s-%s]����%s�ύʧ�ܱ�����:%s", m_pParams->getCString("broker"), m_pParams->getCString("user"), orderInfo->getEntrustID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
			else
			{
				log("[%s-%s]����%s�ѳ���:%s", m_pParams->getCString("broker"), m_pParams->getCString("user"), orderInfo->getOrderID(), orderInfo->getStateMsg());
			}			
		}
	}

	virtual void onPushTrade(WTSTradeInfo* tradeRecord)
	{
		log("[%s-%s]�յ��ɽ��ر�����Լ%s���ɽ��ۣ�%f���ɽ�������%u", m_pParams->getCString("broker"), m_pParams->getCString("user"), tradeRecord->getCode(), tradeRecord->getPrice(), tradeRecord->getVolumn());
	}

	virtual void onTraderError(WTSError*	err)
	{
		if(err && err->getErrorCode() == WEC_ORDERCANCEL)
		{
			log("[%s-%s]����ʧ��: %s", m_pParams->getCString("broker"), m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
	}

public:
	virtual IBaseDataMgr*	getBaseDataMgr()
	{
		return &g_bdMgr;
	}
	

private:
	ITraderApi*			m_pTraderApi;
	FuncDeleteTrader	m_funcDelTrader;
	std::string			m_strModule;
	WTSParams*			m_pParams;

	typedef WTSHashMap<std::string>	WTSObjectMap;
	WTSObjectMap*		m_mapOrds;

	bool				m_bLogined;
};

std::string getBaseFolder()
{
	static std::string basePath;
	if (basePath.empty())
	{
		char path[MAX_PATH] = { 0 };
		GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);

		basePath = path;
		int pos = basePath.find_last_of('\\');
		basePath = basePath.substr(0, pos + 1);
	}

	return basePath;
}

void main()
{
	WTSLogger::init();

	std::string cfg = getBaseFolder() + "config.ini";

	std::string file = IniFile::ReadConfigString("config", "session", "", cfg.c_str());
	if(!file.empty())
		g_bdMgr.loadSessions(file.c_str());

	file = IniFile::ReadConfigString("config", "commodity", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadCommodities(file.c_str());

	file = IniFile::ReadConfigString("config", "contract", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadContracts(file.c_str());

	file = IniFile::ReadConfigString("config", "holiday", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadHolidays(file.c_str());


	WTSParams* params = WTSParams::createObject();

	std::string module = IniFile::ReadConfigString("config", "trader", "", cfg.c_str());
	std::string profile = IniFile::ReadConfigString("config", "profile", "", cfg.c_str());

	StringVector ayKeys, ayVals;
	IniFile::ReadConfigSectionKeyValueArray(ayKeys, ayVals, profile.c_str(), cfg.c_str());
	for (uint32_t i = 0; i < ayKeys.size(); i++)
	{
		const char* key = ayKeys[i].c_str();
		const char* val = ayVals[i].c_str();

		params->append(key, val);
	}

	TraderSpi* trader = new TraderSpi;
	trader->init(params, module.c_str());
	trader->run();
	params->release();

	{
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.wait(lock);
	}
	
	while(true)
	{
		printf("��ѡ�����\r\n");
		printf("1����ѯ�ʽ�\r\n");
		printf("2����ѯ����\r\n");
		printf("3����ѯ�ɽ�\r\n");
		printf("4����ѯ�ֲ�\r\n");
		printf("5����ѯ���㵥\r\n");
		printf("6���޼��µ�\r\n");
		printf("7���м��µ�\r\n");
		printf("8������\r\n");
		printf("0���˳�\r\n");

		char cmd;
		for (;;)
		{
			scanf("%c", &cmd);

			if(cmd >= '0' && cmd <= '8')
				break;
		}

		bool bSucc = false;
		switch (cmd)
		{
		case '1':
			bSucc = trader->qryFund();
			break;
		case '2': 
			bSucc = trader->qryOrders();
			break;
		case '3': 
			bSucc = trader->qryTrades();
			break;
		case '4': 
			bSucc = trader->qryPosition();
			break;
		case '5': 
			bSucc = trader->qrySettle();
			break;
		case '6': 
			bSucc = trader->entrustLmt();
			break;
		case '7': 
			bSucc = trader->entrustMkt();
			break;
		case '8': 
			bSucc = trader->cancel();
			break;
		case '0': break;
		default:
			cmd = 'X';
			break;
		}

		if (cmd != '0' && cmd != 'X' && bSucc)
		{
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.wait(lock);
		}
		else if(cmd == '0')
			break;
	}
	
	trader->release();
	delete trader;
}