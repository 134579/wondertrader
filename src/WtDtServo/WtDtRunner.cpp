/*!
 * \file WtDtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtRunner.h"

#include "../WtDtCore/WtHelper.h"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSUtils/SignalHook.hpp"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../WTSTools/WTSLogger.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/CodeHelper.hpp"

WtDtRunner::WtDtRunner()
	: _data_store(NULL)
	, _is_inited(false)
{
#if _WIN32
#pragma message("Signal hooks disabled in WIN32")
#else
#pragma message("Signal hooks enabled in UNIX")
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
#endif
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::initialize(const char* cfgFile, bool isFile /* = true */, const char* modDir /* = "" */, const char* logCfg /* = "logcfg.yaml" */, FuncOnTickCallback cbTick /* = NULL */)
{
	if(_is_inited)
	{
		WTSLogger::error("WtDtServo has already been initialized");
		return;
	}

	_cb_tick = cbTick;

	WTSLogger::init(logCfg);
	WtHelper::set_module_dir(modDir);

	WTSVariant* config = isFile ? WTSCfgLoader::load_from_file(cfgFile, true) : WTSCfgLoader::load_from_content(cfgFile, false, true);
	if(config == NULL)
	{
		WTSLogger::error("Loading config failed");
		return;
	}

	//���������ļ�
	WTSVariant* cfgBF = config->get("basefiles");
	bool isUTF8 = cfgBF->getBoolean("utf-8");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"), isUTF8);
		WTSLogger::info("Trading sessions loaded");
	}

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::info("Second rules loaded");
	}

	initDataMgr(config->get("data"));

	WTSVariant* cfgParser = config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info("Reading parser config from {}...", filename);
				WTSVariant* var = WTSCfgLoader::load_from_file(filename, isUTF8);
				if (var)
				{
					initParsers(var->get("parsers"));
					var->release();
				}
				else
				{
					WTSLogger::error("Loading parser config {} failed", filename);
				}
			}
			else
			{
				WTSLogger::error("Parser configuration {} not exists", filename);
			}
		}
		else if (cfgParser->type() == WTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}
	else
		WTSLogger::log_raw(LL_WARN, "No parsers config, skipped loading parsers");

	config->release();

	start();

	_is_inited = true;
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	if (config == NULL)
		return;

	_data_mgr.init(config, this);

	WTSLogger::info("Data manager initialized");
}

WTSKlineSlice* WtDtRunner::get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	if(!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime/100000;
	}

	return _data_mgr.get_kline_slice_by_range(stdCode, kp, realTimes, beginTime, endTime);
}

WTSKlineSlice* WtDtRunner::get_bars_by_date(const char* stdCode, const char* period, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
	{
		WTSLogger::log_raw(LL_ERROR, "get_bars_by_date only supports minute period");
		return NULL;
	}

	if (uDate == 0)
	{
		uDate = TimeUtils::getCurDate();
	}

	return _data_mgr.get_kline_slice_by_date(stdCode, kp, realTimes, uDate);
}

WTSTickSlice* WtDtRunner::get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	if(endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime;
	}
	return _data_mgr.get_tick_slices_by_range(stdCode, beginTime, endTime);
}

WTSTickSlice* WtDtRunner::get_ticks_by_date(const char* stdCode, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	return _data_mgr.get_tick_slice_by_date(stdCode, uDate);
}

WTSKlineSlice* WtDtRunner::get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (basePeriod[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime / 100000;
	}

	return _data_mgr.get_kline_slice_by_count(stdCode, kp, realTimes, count, endTime);
}

WTSTickSlice* WtDtRunner::get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime;
	}
	return _data_mgr.get_tick_slice_by_count(stdCode, count, endTime);
}

WTSKlineSlice* WtDtRunner::get_sbars_by_date(const char* stdCode, uint32_t secs, uint32_t uDate /* = 0 */)
{
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	return _data_mgr.get_skline_slice_by_date(stdCode, secs, uDate);
}

void WtDtRunner::initParsers(WTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		// By Wesley @ 2021.12.14
		// ���idΪ�գ��������Զ�id
		std::string realid = id;
		if (realid.empty())
		{
			static uint32_t auto_parserid = 1000;
			realid = StrUtil::printf("auto_parser_%u", auto_parserid++);
		}

		ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, this));
		adapter->init(realid.c_str(), cfgItem);
		_parsers.addAdapter(realid.c_str(), adapter);
	}

	WTSLogger::info("{} market data parsers loaded in total", _parsers.size());
}

void WtDtRunner::start()
{
	_parsers.run();
}

void WtDtRunner::proc_tick(WTSTickData* curTick)
{

}

void WtDtRunner::sub_tick(const char* stdCode)
{
	//�����������Լ����, ��SHFE.ag.HOT, ��ôҪת����ԭ��Լ����, SHFE.ag.1912
	//��Ϊִ����ֻʶ��ԭ��Լ����
	const char* ruleTag = _hot_mgr.getRuleTag(stdCode);
	if (strlen(ruleTag) > 0)
	{
		std::size_t length = strlen(stdCode);
		uint32_t flag = 0;
		if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
		{
			length--;

			flag = (stdCode[length - 1] == SUFFIX_QFQ) ? 1 : 2;
		}

		SubFlags& sids = _tick_sub_map[std::string(stdCode, length)];
		sids.insert(flag);
	}
	else
	{
		std::size_t length = strlen(stdCode);
		uint32_t flag = 0;
		if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
		{
			length--;

			flag = (stdCode[length - 1] == SUFFIX_QFQ) ? 1 : 2;
		}

		SubFlags& sids = _tick_sub_map[std::string(stdCode, length)];
		sids.insert(flag);
	}
}