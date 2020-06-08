/*!
 * \file WtRtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtRtRunner.h"
#include "PyCtaContext.h"
#include "PyMfContext.h"

#include "../WtCore/WtHelper.h"
#include "../WtCore/CtaStraContext.h"
#include "../WtCore/HftStraContext.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/JsonToVariant.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif

extern const char* getBinDir();

WtRtRunner::WtRtRunner()
	: _data_store(NULL)
	, _cb_cta_init(NULL)
	, _cb_cta_tick(NULL)
	, _cb_cta_calc(NULL)
	, _cb_cta_bar(NULL)
	, _cb_mf_init(NULL)
	, _cb_mf_tick(NULL)
	, _cb_mf_calc(NULL)
	, _cb_mf_bar(NULL)
	, _cb_evt(NULL)
	, _is_hft(false)
	, _is_mf(false)
{

}


WtRtRunner::~WtRtRunner()
{
}

bool WtRtRunner::init(const char* logProfile /* = "log4cxx.prop" */)
{
	std::string path = WtHelper::getCWD() + logProfile;
	WTSLogger::init(path.c_str());

	WtHelper::setInstDir(getBinDir());
	return true;
}

void WtRtRunner::registerEvtCallback(FuncEventCallback cbEvt)
{
	_cb_evt = cbEvt;

	_cta_engine.regEventListener(this);
	_hft_engine.regEventListener(this);
	_mf_engine.regEventListener(this);
}

void WtRtRunner::registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_cta_init = cbInit;
	_cb_cta_tick = cbTick;
	_cb_cta_calc = cbCalc;
	_cb_cta_bar = cbBar;
}

void WtRtRunner::registerMfCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_mf_init = cbInit;
	_cb_mf_tick = cbTick;
	_cb_mf_calc = cbCalc;
	_cb_mf_bar = cbBar;
}

uint32_t WtRtRunner::createContext(const char* name)
{
	PyCtaContext* ctx = new PyCtaContext(&_cta_engine, name);
	_cta_engine.addContext(CtaContextPtr(ctx));
	return ctx->id();
}

uint32_t WtRtRunner::createMfContext(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl/* ="CHINA" */)
{
	TaskPeriodType ptype;
	if (my_stricmp(period, "d") == 0)
		ptype = TPT_Daily;
	else if (my_stricmp(period, "w") == 0)
		ptype = TPT_Weekly;
	else if (my_stricmp(period, "m") == 0)
		ptype = TPT_Monthly;
	else if (my_stricmp(period, "y") == 0)
		ptype = TPT_Yearly;
	else
		ptype = TPT_None;

	PyMfContext* ctx = new PyMfContext(&_mf_engine, name);

	_mf_engine.addContext(MfContextPtr(ctx), date, time, ptype, true, trdtpl);

	return ctx->id();
}

CtaContextPtr WtRtRunner::getContext(uint32_t id)
{
	return _cta_engine.getContext(id);
}

MfContextPtr WtRtRunner::getMfContext(uint32_t id)
{
	return _mf_engine.getContext(id);
}

void WtRtRunner::ctx_on_bar(uint32_t id, const char* code, const char* period, WTSBarStruct* newBar, bool isCta/* = true*/)
{
	if (_cb_cta_bar && isCta)
		_cb_cta_bar(id, code, period, newBar);
	else if(!isCta && _cb_mf_bar)
		_cb_mf_bar(id, code, period, newBar);
}

void WtRtRunner::ctx_on_calc(uint32_t id, bool isCta/* = true*/)
{
	if (_cb_cta_calc && isCta)
		_cb_cta_calc(id);
	else if (!isCta && _cb_mf_calc)
		_cb_mf_calc(id);
}

void WtRtRunner::ctx_on_init(uint32_t id, bool isCta/* = true*/)
{
	if (_cb_cta_init && isCta)
		_cb_cta_init(id);
	else if (!isCta && _cb_mf_init)
		_cb_mf_init(id);
}

void WtRtRunner::ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, bool isCta/* = true*/)
{
	if (_cb_cta_tick && isCta)
		_cb_cta_tick(id, stdCode, &newTick->getTickStruct());
	else if (!isCta && _cb_mf_tick)
		_cb_mf_tick(id, stdCode, &newTick->getTickStruct());
}

bool WtRtRunner::config(const char* cfgFile)
{
	std::string json;
	StdFile::read_file_content(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//���������ļ�
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("����ʱ��ģ��������");
	}

	if (cfgBF->get("commodity"))
	{
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("Ʒ���б�������");
	}

	if (cfgBF->get("contract"))
	{
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("��Լ�б�������");
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("�ڼ���ģ��������");
	}

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("�����л���������");
	}

	//��ʼ�����л���
	initEngine();

	//��ʼ�����ݹ���
	initDataMgr();

	//��ʼ����ƽ����
	if (!initActionPolicy())
		return false;

	//��ʼ������ͨ��
	initParsers();

	//��ʼ������ͨ��
	initTraders();

	//������Ǹ�Ƶ���棬����Ҫ����ִ��ģ��
	if (!_is_hft)
		initExecuters();

	//���ﱾ��Ӧ���У����Ƕ���py���֣��Ͳ�Ҫ�����
	//initStrategies();

	if (!_is_hft)
		initCtaStrategies();
	else
		initHftStrategies();
	
	return true;
}

bool WtRtRunner::initCtaStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("cta");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		CtaStrategyPtr stra = _cta_mgr.createStrategy(name, id);
		stra->self()->init(cfgItem->get("params"));
		CtaStraContext* ctx = new CtaStraContext(&_cta_engine, id);
		ctx->set_strategy(stra->self());
		_cta_engine.addContext(CtaContextPtr(ctx));
	}

	return true;
}

bool WtRtRunner::initHftStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("hft");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		HftStrategyPtr stra = _hft_mgr.createStrategy(name, id);
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

bool WtRtRunner::initEngine()
{
	WTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	const char* name = cfg->getCString("name");

	if (strlen(name) == 0 || my_stricmp(name, "cta") == 0)
	{
		_is_hft = false;
		_is_mf = false;
	}
	else if (my_stricmp(name, "mltfct") == 0)
	{
		_is_mf = true;
	}
	else //if (my_stricmp(name, "hft") == 0)
	{
		_is_hft = true;
	}

	if (_is_hft)
	{
		WTSLogger::info("���׻�����ʼ����ɣ��������棺HFT");
		_hft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_hft_engine;
	}
	else if (_is_mf)
	{
		WTSLogger::info("���׻�����ʼ����ɣ��������棺MultiFactros");
		_mf_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_mf_engine;
	}
	else
	{
		WTSLogger::info("���׻�����ʼ����ɣ��������棺CTA");
		_cta_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_cta_engine;
	}
	
	return true;
}

bool WtRtRunner::initDataMgr()
{
	WTSVariant* cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, _engine);

	WTSLogger::info("���ݹ���ģ���ʼ�����");
	return true;
}

bool WtRtRunner::initParsers()
{
	WTSVariant* cfg = _config->get("parsers");
	if (cfg == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(id, cfgItem, _engine);

		_parsers.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("������%u������ͨ��", count);

	return true;
}

bool WtRtRunner::initExecuters()
{
	WTSVariant* cfg = _config->get("executers");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	//�ȼ����Դ���ִ��������
	std::string path = WtHelper::getInstDir() + "executer//";
	_exe_factory.loadFactories(path.c_str());

	uint32_t count = 0;
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
		count++;
	}

	WTSLogger::info("������%u��ִ����", count);

	return true;
}

bool WtRtRunner::initTraders()
{
	WTSVariant* cfg = _config->get("traders");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;
	
	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter);
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);
		count++;
	}

	WTSLogger::info("������%u������ͨ��", count);

	return true;
}

void WtRtRunner::run(bool bAsync /* = false */)
{
	_parsers.run();
	_traders.run();

	_engine->run(bAsync);
}

void WtRtRunner::release()
{
	WTSLogger::stop();
}

bool WtRtRunner::initActionPolicy()
{
	const char* action_file = _config->getCString("bspolicy");
	if (strlen(action_file) <= 0)
		return false;

	bool ret = _act_policy.init(action_file);
	WTSLogger::info("��ƽ����ģ���ʼ�����");
	return ret;
}

bool WtRtRunner::addCtaFactories(const char* folder)
{
	return _cta_mgr.loadFactories(folder);
}

bool WtRtRunner::addExeFactories(const char* folder)
{
	return _exe_factory.loadFactories(folder);
}

bool WtRtRunner::addHftFactories(const char* folder)
{
	return _hft_mgr.loadFactories(folder);
}