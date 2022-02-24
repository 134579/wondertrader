#include "WtRdmDtReaderAD.h"
#include "LMDBKeys.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_reader_log(IRdmDtReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	memset(buffer, 0, 512);
	fmt::format_to(buffer, format, args...);

	sink->reader_log(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IRdmDtReader* createRdmDtReader()
	{
		IRdmDtReader* ret = new WtRdmDtReaderAD();
		return ret;
	}

	EXPORT_FLAG void deleteRdmDtReader(IRdmDtReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};


WtRdmDtReaderAD::WtRdmDtReaderAD()
	: _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtRdmDtReaderAD::~WtRdmDtReaderAD()
{
}

void WtRdmDtReaderAD::init(WTSVariant* cfg, IRdmDtReaderSink* sink)
{
	IRdmDtReader::init(cfg, sink);

	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	pipe_reader_log(sink, LL_INFO, "WtRdmDtReaderAD initialized, root data folder is {}", _base_dir);
}

WTSTickSlice* WtRdmDtReaderAD::readTickSlicesByRange(const char* stdCode, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	WTSSessionInfo* sInfo = commInfo->getSessionInfo();
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t rDate, rTime, rSecs;
	//20190807124533900
	rDate = (uint32_t)(etime / 1000000000);
	rTime = sInfo->offsetTime((uint32_t)(etime % 1000000000) / 100000, false);
	rSecs = (uint32_t)(etime % 100000);

	uint32_t lDate, lTime, lSecs;
	//20190807124533900
	lDate = (uint32_t)(stime / 1000000000);
	lTime = sInfo->offsetTime((uint32_t)(stime % 1000000000) / 100000, true);
	lSecs = (uint32_t)(stime % 100000);

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), rDate, rTime, false);
	uint32_t beginTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), lDate, lTime, false);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && commInfo->isFuture())
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && commInfo->isFuture())
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	uint32_t reload_flag = 0; //���ر�ǣ�0-����Ҫ���أ�1-�������£�2-ȫ������
	std::string key = StrUtil::printf("%s.%s", cInfo._exchg, curCode.c_str());
	//�ȼ�黺��
	TicksList& tickList = _ticks_cache[key];
	uint64_t last_access_time = 0;
	do
	{
		if (tickList._ticks.capacity() < count)
		{
			//����������ȫ������
			reload_flag = 2;
			tickList._ticks.rset_capacity(count);
			tickList._ticks.clear();	//���ԭ������
		}

		if (tickList._last_req_time < etime)
		{
			//����ʱ�䲻����ֻ��ȡ���µ�
			reload_flag = 1;
			last_access_time = tickList._last_req_time;
			break;
		}

	} while (false);

	//����Ҫ�ĳɴ�lmdb��ȡ
	WtLMDBPtr db = get_t_db(cInfo._exchg, cInfo._code);
	if (db == NULL)
		return NULL;

	if (reload_flag == 1)
	{
		//�������£����ϴε�ʱ��������ε�ʱ�������ȡ���µļ���
		last_access_time += 1;
		WtLMDBQuery query(*db);
		LMDBHftKey lKey(cInfo._exchg, cInfo._code, beginTDate, lTime * 100000 + lSecs);
		LMDBHftKey rKey(cInfo._exchg, cInfo._code, endTDate, rTime * 100000 + rSecs);
		int cnt = query.get_range(std::string((const char*)&lKey, sizeof(lKey)),
			std::string((const char*)&rKey, sizeof(rKey)), [this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
			for (const std::string& item : ayVals)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)item.data();
				tickList._ticks.push_back(*curTick);
			}
		});
		if (cnt > 0)
			pipe_reader_log(_sink, LL_DEBUG, "{} ticks after {} of {} append to cache", cnt, last_access_time, stdCode);
	}
	else if (reload_flag == 2)
	{
		//ȫ�����£��ӽ���ʱ����ǰ��ȡ����
		WtLMDBQuery query(*db);
		LMDBHftKey rKey(cInfo._exchg, cInfo._code, (uint32_t)(etime / 1000000000), (uint32_t)(etime % 1000000000));
		LMDBHftKey lKey(cInfo._exchg, cInfo._code, 0, 0);
		int cnt = query.get_lowers(std::string((const char*)&lKey, sizeof(lKey)), std::string((const char*)&rKey, sizeof(rKey)),
			count, [this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
			tickList._ticks.resize(ayVals.size());
			for (std::size_t i = 0; i < ayVals.size(); i++)
			{
				const std::string& item = ayVals[i];
				memcpy(&tickList._ticks[i], item.data(), item.size());
			}
		});

		pipe_reader_log(_sink, LL_DEBUG, "{} ticks of {} loaded to cache for the first time", cnt, stdCode);
	}

	tickList._last_req_time = etime;

	//ȫ����ȡ����Ժ���������Ƭ
	//����Ҫע��һ�£���Ϊ��ȡtick��˳��ģ����Ƿ�������Ҫ�Ӻ���ǰȡָ�������ģ�����Ӧ���ȶ�ȡarray_two
	count = min((uint32_t)tickList._ticks.size(), count);	//�ȶ�count��һ������
	auto ayTwo = tickList._ticks.array_two();
	auto cnt_2 = ayTwo.second;
	if (cnt_2 >= count)
	{
		//���array_two�����㹻����ֱ�ӷ������ݿ�
		return WTSTickSlice::create(stdCode, &tickList._ticks[ayTwo.second - count], count);
	}
	else
	{
		//���array_two������������Ҫ�ٴ�array_one��ȡ
		auto ayOne = tickList._ticks.array_one();
		auto diff = count - cnt_2;
		auto ret = WTSTickSlice::create(stdCode, &tickList._ticks[ayOne.second - diff], diff);
		if (cnt_2 > 0)
			ret->appendBlock(ayTwo.first, cnt_2);
		return ret;
	}
}

WTSKlineSlice* WtRdmDtReaderAD::readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime /* = 0 */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = _sink->get_date();
		curTime = _sink->get_min_time();
		curSecs = _sink->get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	uint32_t endTDate = _base_data_mgr->calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string curCode = cInfo._code;
	if (cInfo.isHot() && commInfo->isFuture())
		curCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, endTDate);
	else if (cInfo.isSecond() && commInfo->isFuture())
		curCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, endTDate);

	std::string key = StrUtil::printf("%s#%u", stdCode, period);
	BarsList& barsList = _bars_cache[key];
	if (barsList._bars.capacity() < count)
	{
		//����������ȫ������
		barsList._bars.rset_capacity(count);
		barsList._bars.clear();	//���ԭ������
		cacheBarsFromStorage(key, stdCode, period, count);
	}

	//����ֻ��Ҫ���һ��RTBarCache��K��ʱ�����etime�Ƿ�һ��
	//���һ�£�˵�����һ��K�߻�û��ɣ�����ϵͳҪ��ȡ�����ʱ����û����е����һ��bar׷�ӵ����
	//�����һ�£�˵��Writer�Ǳ��Ѿ���������ˣ�ֱ���û���õ�K�߼���
	//OnMinuteEnd��ʱ��ҲҪ�����Ƶļ��
	if (barsList._bars.empty())
		return NULL;

	//����������һ������
	count = min((uint32_t)barsList._bars.size(), count);

	bool isDay = (period == KP_DAY);
	etime = isDay ? curDate : ((curDate - 19900000) * 10000 + curTime);
	if (barsList._last_req_time < etime)
	{
		//�ϴ������ʱ�䣬С�ڵ�ǰ�����ʱ�䣬��Ҫ������һ��K��
		WTSBarStruct& lastBar = barsList._bars.back();
		uint32_t lastBarTime = isDay ? lastBar.date : (uint32_t)lastBar.time;
		if (lastBarTime < etime)
		{
			//������һ��K�ߵ�ʱ��С�ڵ�ǰʱ�䣬�ȴ���LMDB�������µ�K��
			update_cache_from_lmdb(barsList, cInfo._exchg, curCode.c_str(), period, lastBarTime);

			lastBar = barsList._bars.back();
			lastBarTime = isDay ? lastBar.date : (uint32_t)lastBar.time;
		}

		//��lmdb�������Ժ��ټ��
		//���ʱ�����ȻС�ڽ�ֹʱ��
		//��ӻ����ж�ȡ
		if (lastBarTime < etime)
		{
			WTSBarStruct* rtBar = get_rt_cache_bar(cInfo._exchg, curCode.c_str(), period);
			if (rtBar != NULL)
			{
				uint64_t cacheBarTime = isDay ? rtBar->date : rtBar->time;
				if (cacheBarTime > etime)
				{
					//�����K��ʱ������ڽ�ֹʱ�䣬˵�����Ĺ�����Writer�Ѿ�������ת����lmdb����
					//���ʱ����ٶ�һ��lmdb
					update_cache_from_lmdb(barsList, cInfo._exchg, curCode.c_str(), period, lastBarTime);
					barsList._last_from_cache = false;
				}
				else
				{
					//��������K��ʱ��û�г���etime���򽫻����е����һ��K��׷�ӵ�������
					barsList._bars.push_back(*rtBar);
					barsList._last_from_cache = true;
					pipe_reader_log(_sink, LL_DEBUG,
						"{} bars @  {} of {} updated from cache instead of lmdb in {}", PERIOD_NAME[period], etime, stdCode, __FUNCTION__);
				}
			}
		}
	}

	//ȫ�����������Ժ��ٴ�K�߻������ȡ
	barsList._last_req_time = etime;

	//����Ҫע��һ�£���Ϊ��ȡ������˳��ģ����Ƿ�������Ҫ�Ӻ���ǰȡָ�������ģ�����Ӧ���ȶ�ȡarray_two
	count = min((uint32_t)barsList._bars.size(), count);	//�ȶ�count��һ������
	auto ayTwo = barsList._bars.array_two();
	auto cnt_2 = ayTwo.second;
	if (cnt_2 >= count)
	{
		//���array_two�����㹻����ֱ�ӷ������ݿ�
		return WTSKlineSlice::create(stdCode, period, 1, &barsList._bars[ayTwo.second - count], count);
	}
	else
	{
		//���array_two������������Ҫ�ٴ�array_one��ȡ
		auto ayOne = barsList._bars.array_one();
		auto diff = count - cnt_2;
		auto ret = WTSKlineSlice::create(stdCode, period, 1, &barsList._bars[ayOne.second - diff], diff);
		if (cnt_2 > 0)
			ret->appendBlock(ayTwo.first, cnt_2);
		return ret;
	}


	return NULL;
}

WtRdmDtReaderAD::WtLMDBPtr WtRdmDtReaderAD::get_k_db(const char* exchg, WTSKlinePeriod period)
{
	WtLMDBMap* the_map = NULL;
	std::string subdir;
	if (period == KP_Minute1)
	{
		the_map = &_exchg_m1_dbs;
		subdir = "min1";
	}
	else if (period == KP_Minute5)
	{
		the_map = &_exchg_m5_dbs;
		subdir = "min5";
	}
	else if (period == KP_DAY)
	{
		the_map = &_exchg_d1_dbs;
		subdir = "day";
	}
	else
		return std::move(WtLMDBPtr());

	auto it = the_map->find(exchg);
	if (it != the_map->end())
		return std::move(it->second);

	WtLMDBPtr dbPtr(new WtLMDB(true));
	std::string path = StrUtil::printf("%s%s/%s/", _base_dir.c_str(), subdir.c_str(), exchg);
	boost::filesystem::create_directories(path);
	if (!dbPtr->open(path.c_str()))
	{
		pipe_reader_log(_sink, LL_ERROR, "Opening {} db if {} failed: {}", subdir, exchg, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}
	else
	{
		pipe_reader_log(_sink, LL_DEBUG, "{} db of {} opened", subdir, exchg);
	}

	(*the_map)[exchg] = dbPtr;
	return std::move(dbPtr);
}

WtRdmDtReaderAD::WtLMDBPtr WtRdmDtReaderAD::get_t_db(const char* exchg, const char* code)
{
	std::string key = StrUtil::printf("%s.%s", exchg, code);
	auto it = _tick_dbs.find(key);
	if (it != _tick_dbs.end())
		return std::move(it->second);

	WtLMDBPtr dbPtr(new WtLMDB(true));
	std::string path = StrUtil::printf("%sticks/%s/%s", _base_dir.c_str(), exchg, code);
	boost::filesystem::create_directories(path);
	if (!dbPtr->open(path.c_str()))
	{
		pipe_reader_log(_sink, LL_ERROR, "Opening tick db of {}.{} failed: {}", exchg, code, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}
	else
	{
		pipe_reader_log(_sink, LL_DEBUG, "Tick db of {}.{} opened", exchg, code);
	}

	_tick_dbs[exchg] = dbPtr;
	return std::move(dbPtr);
}