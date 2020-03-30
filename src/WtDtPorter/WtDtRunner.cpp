#include "WtDtRunner.h"

#include "../Share/WTSSessionInfo.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../WTSTools/WTSLogger.h"


WtDtRunner::WtDtRunner()
{
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::start()
{
	m_asyncIO.post([this](){
		boost::this_thread::sleep(boost::posix_time::millisec(5));
		m_stateMon.run();
	});

	boost::asio::io_service::work work(m_asyncIO);
	m_asyncIO.run();
}

void WtDtRunner::initialize(const char* cfgFile, const char* logCfg)
{
	WTSLogger::init(logCfg);

	std::string json;
	StdFile::read_file_content(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	WTSVariant* config = WTSVariant::createObject();
	jsonToVariant(document, config);

	//���������ļ�
	WTSVariant* cfgBF = config->get("basefiles");
	if (cfgBF->get("session"))
	{
		m_baseDataMgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("����ʱ��ģ��������");
	}

	if (cfgBF->get("commodity"))
	{
		m_baseDataMgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("Ʒ���б�������");
	}

	if (cfgBF->get("contract"))
	{
		m_baseDataMgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("��Լ�б�������");
	}

	if (cfgBF->get("holiday"))
	{
		m_baseDataMgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("�ڼ���ģ��������");
	}

	if (cfgBF->get("hot"))
	{
		m_hotMgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("�����л���������");
	}

	m_udpCaster.init(config->get("broadcaster"), &m_baseDataMgr, &m_dataMgr);

	initDataMgr(config->get("writer"));

	m_stateMon.initialize("statemonitor.json", &m_baseDataMgr, &m_dataMgr);

	initParsers(config->get("parsers"));

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	m_dataMgr.init(config, &m_baseDataMgr, &m_stateMon, &m_udpCaster);
}

void WtDtRunner::initParsers(WTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* path = cfgItem->getCString("module");
		DllHandle libParser = DLLHelper::load_library(path);
		if (libParser)
		{
			FuncCreateParser pFuncCreateParser = (FuncCreateParser)DLLHelper::get_symbol(libParser, "createParser");
			if (pFuncCreateParser == NULL)
			{
				WTSLogger::info("����ģ���ʼ��ʧ��,�Ҳ�����Ӧ����ں���...");
			}

			FuncDeleteParser pFuncDeleteParser = (FuncDeleteParser)DLLHelper::get_symbol(libParser, "deleteParser");
			if (pFuncDeleteParser == NULL)
			{
				WTSLogger::info("����ģ���ʼ��ʧ��,�Ҳ�����Ӧ����ں���...");
			}

			if (pFuncCreateParser && pFuncDeleteParser)
			{
				WTSParams* params = cfgItem->toParams();

				ParserAdapterPtr adapter(new ParserAdapter(&m_baseDataMgr, &m_dataMgr));
				adapter->initAdapter(params, pFuncCreateParser, pFuncDeleteParser);
				ParserAdapterMgr::addAdapter(adapter);
				params->release();
			}

		}
		else
		{
			WTSLogger::info("����ģ���ʼ��ʧ��,����ģ��%sʧ��...", path);
		}
	}

	WTSLogger::info("һ������%u��Parser", ParserAdapterMgr::size());
}
