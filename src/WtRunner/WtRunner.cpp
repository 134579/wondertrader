/*!
 * \file WtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "stdafx.h"
#include "WtRunner.h"

#include "..\WtCore\WtHelper.h"
#include "..\WtCore\CtaStraContext.h"
#include "..\WtCore\HftStraContext.h"

#include "..\WtCore\WtDataStorage.h"

#include "..\Share\WTSVariant.hpp"
#include "..\Share\StdUtils.hpp"
#include "..\WTSTools\WTSLogger.h"
#include "..\Share\JsonToVariant.hpp"

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif


WtRunner::WtRunner()
	: _data_store(NULL)
	, _is_hft(false)
{
}


WtRunner::~WtRunner()
{
}

bool WtRunner::init()
{
	std::string path = WtHelper::getCWD() + "log4cxx.prop";
	WTSLogger::init(path.c_str());

	return true;
}

bool WtRunner::config(const char* cfgFile)
{
	std::string json;
	BoostFile::read_file_contents(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//���������ļ�
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

	if (cfgBF->get("commodity"))
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));

	if (cfgBF->get("contract"))
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	//��ʼ�����л���
	initEnv();

	//��ʼ�����ݹ���
	initDataMgr();

	if (!initActionPolicy())
		return false;

	//��ʼ������ͨ��
	initParsers();

	//��ʼ������ͨ��
	initTraders();

	//������Ǹ�Ƶ���棬����Ҫ����ִ��ģ��
	if (!_is_hft)
		initExecuters();

	if (!_is_hft)
		initCtaStrategies();
	else
		initHftStrategies();
	
	return true;
}

bool WtRunner::initCtaStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("cta");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "cta/";
	_cta_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		CtaStrategyPtr stra = _cta_stra_mgr.createStrategy(name, id);
		stra->self()->init(cfgItem->get("params"));
		CtaStraContext* ctx = new CtaStraContext(&_cta_engine, id);
		ctx->set_strategy(stra->self());
		_cta_engine.addContext(CtaContextPtr(ctx));
	}

	return true;
}

bool WtRunner::initHftStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("hft");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "hft/";
	_hft_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		HftStrategyPtr stra = _hft_stra_mgr.createStrategy(name, id);
		if (stra == NULL)
			continue;

		stra->self()->init(cfgItem->get("params"));
		HftStraContext* ctx = new HftStraContext(&_hft_engine, id);
		ctx->set_strategy(stra->self());

		TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
		ctx->setTrader(trader.get());
		trader->addSink(ctx);

		_hft_engine.addContext(HftContextPtr(ctx));
	}

	return true;
}


bool WtRunner::initEnv()
{
	WTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	const char* name = cfg->getCString("name");
	
	if (strlen(name) == 0 || my_stricmp(name, "cta") == 0)
	{
		_is_hft = false;
	}
	else
	{
		_is_hft = true;
		
	}

	if(_is_hft)
	{
		_hft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_hft_engine;

		WTSLogger::info("���׻�����ʼ����ɣ��������棺HFT");
	}
	else
	{
		_cta_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_cta_engine;

		WTSLogger::info("���׻�����ʼ����ɣ��������棺CTA");
	}

	return true;
}

bool WtRunner::initActionPolicy()
{
	return _act_policy.init(_config->getCString("bspolicy"));
}

bool WtRunner::initDataMgr()
{
	WTSVariant* cfg = _config->get("store");
	if (cfg == NULL)
		return false;

	IDataStore* pStore = NULL;
	{
		_data_store = new WtDataStorage();
		_data_store->init(cfg, _engine, &_bd_mgr, &_hot_mgr);
		pStore = _data_store;
	}

	cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, pStore, _engine);

	return true;
}

bool WtRunner::initParsers()
{
	WTSVariant* cfg = _config->get("parsers");
	if (cfg == NULL)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if(!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(id, cfgItem, _engine);

		_parsers.addAdapter(id, adapter);
	}

	return true;
}

bool WtRunner::initExecuters()
{
	WTSVariant* cfg = _config->get("executers");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "executer/";
	_exe_factory.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		WtExecuterPtr executer(new WtExecuter(&_exe_factory, id, &_data_mgr));
		if (!executer->init(cfgItem))
			return false;

		TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
		executer->setTrader(trader.get());
		trader->addSink(executer.get());

		_cta_engine.addExecuter(executer);
	}

	return true;
}

bool WtRunner::initTraders()
{
	WTSVariant* cfg = _config->get("traders");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;
	
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter);
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);
	}

	//_adapter_mgr.runAdapters();

	return true;
}

void WtRunner::run(bool bAsync /* = false */)
{
	_parsers.run();
	_traders.run();

	_engine->run(bAsync);
}