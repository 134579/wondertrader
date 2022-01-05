#include "WtDataReaderAD.h"
#include "LMDBKeys.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSDataDef.hpp"

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_reader_log(IDataReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	fmt::format_to(buffer, format, args...);

	sink->reader_log(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IDataReader* createDataReader()
	{
		IDataReader* ret = new WtDataReaderAD();
		return ret;
	}

	EXPORT_FLAG void deleteDataReader(IDataReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};


WtDataReaderAD::WtDataReaderAD()
	: _last_time(0)
	, _base_data_mgr(NULL)
	, _hot_mgr(NULL)
{
}


WtDataReaderAD::~WtDataReaderAD()
{
}

void WtDataReaderAD::init(WTSVariant* cfg, IDataReaderSink* sink, IHisDataLoader* loader /* = NULL */)
{
	IDataReader::init(cfg, sink, loader);

	_base_data_mgr = sink->get_basedata_mgr();
	_hot_mgr = sink->get_hot_mgr();

	if (cfg == NULL)
		return ;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	_d1_cache._filename = "cache_d1.dmb";
	_m1_cache._filename = "cache_m1.dmb";
	_m5_cache._filename = "cache_m5.dmb";
}

WTSTickSlice* WtDataReaderAD::readTickSlice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
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

	uint32_t reload_flag = 0; //���ر�ǣ�0-����Ҫ���أ�1-�������£�2-ȫ������
	std::string key = StrUtil::printf("%s.%s", cInfo._exchg, curCode.c_str());
	//�ȼ�黺��
	TicksList& tickList = _ticks_cache[key];
	uint64_t last_access_time = 0;
	do
	{
		if(tickList._ticks.capacity() < count)
		{
			//����������ȫ������
			reload_flag = 2;
			tickList._ticks.rset_capacity(count);
			tickList._ticks.clear();	//���ԭ������
		}

		if(tickList._last_req_time < etime)
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
	{
		pipe_reader_log(_sink, LL_ERROR, "Cannot get tick db stub of {}.{}", cInfo._exchg, cInfo._code);
		return NULL;
	}

	if(reload_flag == 1)
	{
		//�������£����ϴε�ʱ��������ε�ʱ�������ȡ���µļ���
		last_access_time += 1;
		WtLMDBQuery query(*db);
		LMDBHftKey lKey(cInfo._exchg, cInfo._code, (uint32_t)(last_access_time / 1000000000), (uint32_t)(last_access_time % 1000000000));
		LMDBHftKey rKey(cInfo._exchg, cInfo._code, (uint32_t)(etime / 1000000000), (uint32_t)(etime % 1000000000));
		query.get_range(std::string((const char*)&lKey, sizeof(lKey)), 
			std::string((const char*)&rKey, sizeof(rKey)), [this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
			for(const std::string& item : ayVals)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)item.data();
				tickList._ticks.push_back(*curTick);
			}
		});
	}
	else if(reload_flag == 2)
	{
		//ȫ�����£��ӽ���ʱ����ǰ��ȡ����
		WtLMDBQuery query(*db);
		LMDBHftKey rKey(cInfo._exchg, cInfo._code, (uint32_t)(etime / 1000000000), (uint32_t)(etime % 1000000000));
		query.get_lowers(std::string((const char*)&rKey, sizeof(rKey)),
			count, [this, &tickList](const ValueArray& ayKeys, const ValueArray& ayVals) {
			tickList._ticks.resize(ayVals.size());
			for (std::size_t i = 0; i < ayVals.size(); i++)
			{
				const std::string& item = ayVals[i];
				memcpy(&tickList._ticks[i], item.data(), item.size());
			}
		});
	}

	tickList._last_req_time = etime;

	//ȫ����ȡ����Ժ���������Ƭ
	//����Ҫע��һ�£���Ϊ��ȡtick��˳��ģ����Ƿ�������Ҫ�Ӻ���ǰȡָ�������ģ�����Ӧ���ȶ�ȡarray_two
	count = min(tickList._ticks.size(), count);	//�ȶ�count��һ������
	auto ayTwo = tickList._ticks.array_two();
	auto cnt_2 = ayTwo.second;
	if(cnt_2 >= count)
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
		if(cnt_2 > 0)
			ret->appendBlock(ayTwo.first, cnt_2);
		return ret;
	}
}

std::string WtDataReaderAD::read_bars_to_buffer(const char* exchg, const char* code, WTSKlinePeriod period)
{
	//ֱ�Ӵ�LMDB��ȡ
	WtLMDBPtr db = get_k_db(exchg, period);
	if (db == NULL)
		return false;

	std::string buffer;
	WtLMDBQuery query(*db);
	LMDBBarKey lKey(exchg, code, 0);
	LMDBBarKey rKey(exchg, code, 0xffffffff);
	query.get_range(std::string((const char*)&lKey, sizeof(lKey)),
		std::string((const char*)&lKey, sizeof(lKey)), [this, &buffer](const ValueArray& ayKeys, const ValueArray& ayVals) {
		if (ayVals.empty())
			return;

		buffer.resize(sizeof(WTSBarStruct)*ayVals.size());
		memcpy((void*)buffer.data(), ayVals.data(), sizeof(WTSBarStruct)*ayVals.size());
	});
	return std::move(buffer);
}

bool WtDataReaderAD::cacheBarsFromStorage(const std::string& key, const char* stdCode, WTSKlinePeriod period, uint32_t count)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(cInfo._exchg, cInfo._product);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;
	barList._exchg = cInfo._exchg;

	//ֱ�Ӵ�LMDB��ȡ
	WtLMDBPtr db = get_k_db(cInfo._exchg, period);
	if (db == NULL)
		return false;

	WtLMDBQuery query(*db);
	LMDBBarKey rKey(cInfo._exchg, cInfo._code, 0xffffffff);
	query.get_lowers(std::string((const char*)&rKey, sizeof(rKey)), 
		count, [this, &barList](const ValueArray& ayKeys, const ValueArray& ayVals) {
		if (ayVals.empty())
			return;

		for(const std::string& item : ayVals)
			barList._bars.push_back(*(WTSBarStruct*)item.data());
	});
	return true;
}

void WtDataReaderAD::update_cache_from_lmdb(BarsList& barsList, const char* exchg, const char* code, WTSKlinePeriod period, uint32_t lastBarTime)
{
	bool isDay = (period == KP_DAY);
	WtLMDBPtr db = get_k_db(exchg, period);
	WtLMDBQuery query(*db);
	LMDBBarKey lKey(exchg, code, lastBarTime);
	query.get_uppers(std::string((const char*)&lKey, sizeof(lKey)), 9999, [this, &barsList, isDay, lastBarTime](const ValueArray& ayKeys, const ValueArray& ayVals) {

		for (const std::string& item : ayVals)
		{
			WTSBarStruct* curBar = (WTSBarStruct*)item.data();
			uint64_t curBarTime = isDay ? curBar->date : curBar->time;
			if (curBarTime == lastBarTime)
			{
				if (barsList._last_from_cache)
					memcpy(&barsList._bars.back(), curBar, sizeof(WTSBarStruct));
			}
			else
			{
				barsList._bars.push_back(*curBar);
			}
		}
	});
}

WTSKlineSlice* WtDataReaderAD::readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime /* = 0 */)
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
	count = min(barsList._bars.size(), count);

	bool isDay = (period == KP_DAY);
	etime = isDay ? curDate : ((curDate - 19900000)*10000 + curTime);
	if(barsList._last_req_time < etime)
	{
		//�ϴ������ʱ�䣬С�ڵ�ǰ�����ʱ�䣬��Ҫ������һ��K��
		WTSBarStruct& lastBar = barsList._bars.back();
		uint32_t lastBarTime = isDay ? lastBar.date : (uint32_t)lastBar.time;
		if(lastBarTime < etime)
		{
			//������һ��K�ߵ�ʱ��С�ڵ�ǰʱ�䣬�ȴ���LMDB�������µ�K��
			update_cache_from_lmdb(barsList, cInfo._exchg, curCode.c_str(), period, lastBarTime);

			lastBar = barsList._bars.back();
			lastBarTime = isDay ? lastBar.date : (uint32_t)lastBar.time;
		}

		//��lmdb�������Ժ��ټ��
		//���ʱ�����ȻС�ڽ�ֹʱ��
		//��ӻ����ж�ȡ
		if(lastBarTime < etime)
		{
			RTBarCacheWrapper* wrapper = NULL;
			if (period == KP_DAY)
				wrapper = &_d1_cache;
			else if (period == KP_Minute1)
				wrapper = &_m1_cache;
			else if (period == KP_Minute5)
				wrapper = &_m5_cache;

			if(wrapper != NULL)
			{
				if(wrapper->empty())
				{
					//�������Ϊ�գ�����ػ���
					do
					{

						//����Ϊ�գ����Զ�����
						std::string filename = _base_dir + wrapper->_filename;
						if (!StdFile::exists(filename.c_str()))
							break;

						wrapper->_file_ptr.reset(new BoostMappingFile);
						wrapper->_file_ptr->map(filename.c_str());
						wrapper->_cache_block = (RTBarCache*)wrapper->_file_ptr->addr();

						wrapper->_cache_block->_size = min(wrapper->_cache_block->_size, wrapper->_cache_block->_capacity);
						wrapper->_last_size = wrapper->_cache_block->_size;

						for (uint32_t i = 0; i < wrapper->_cache_block->_size; i++)
						{
							const BarCacheItem& item = wrapper->_cache_block->_items[i];
							wrapper->_idx[StrUtil::printf("%s.%s", item._exchg, item._code)] = i;
						}
					} while (false);
				}
				else
				{
					//������治Ϊ�գ����һ����û���µĺ�Լ����������еĻ����Ͱ���������һ��
					if(wrapper->_last_size != wrapper->_cache_block->_size)
					{
						for (uint32_t i = wrapper->_last_size; i < wrapper->_cache_block->_size; i++)
						{
							const BarCacheItem& item = wrapper->_cache_block->_items[i];
							wrapper->_idx[StrUtil::printf("%s.%s", item._exchg, item._code)] = i;
						}
					}
				}

				//��󿴻�������û�иĺ�Լ��Ӧ��K�߻���
				auto it = wrapper->_idx.find(StrUtil::printf("%s.%s", cInfo._exchg, curCode.c_str()));
				if(it != wrapper->_idx.end())
				{
					WTSBarStruct cacheBar;
					memcpy(&cacheBar, &wrapper->_cache_block->_items[it->second]._bar, sizeof(WTSBarStruct));
					uint64_t cacheBarTime = isDay ? cacheBar.date : cacheBar.time;
					if(cacheBarTime > etime)
					{
						//�����K��ʱ������ڽ�ֹʱ�䣬˵�����Ĺ�����Writer�Ѿ�������ת����lmdb����
						//���ʱ����ٶ�һ��lmdb
						update_cache_from_lmdb(barsList, cInfo._exchg, curCode.c_str(), period, lastBarTime);
						barsList._last_from_cache = false;
					}
					else
					{
						//��������K��ʱ��û�г���etime���򽫻����е����һ��K��׷�ӵ�������
						barsList._bars.push_back(cacheBar);
						barsList._last_from_cache = true;
					}
				}
			}
		}
	}
	
	//ȫ�����������Ժ��ٴ�K�߻������ȡ
	barsList._last_req_time = etime;

	//����Ҫע��һ�£���Ϊ��ȡ������˳��ģ����Ƿ�������Ҫ�Ӻ���ǰȡָ�������ģ�����Ӧ���ȶ�ȡarray_two
	count = min(barsList._bars.size(), count);	//�ȶ�count��һ������
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

void WtDataReaderAD::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//����Ӧ�ô������
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;
	if (nowTime <= _last_time)
		return;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = (BarsList&)it->second;
		if (barsList._period != KP_DAY)
		{
			CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(barsList._code.c_str());
			update_cache_from_lmdb(barsList, barsList._exchg.c_str(), cInfo._code, barsList._period, (uint32_t)(nowTime - 19900000));
		}
	}

	if (_sink)
		_sink->on_all_bar_updated(uTime);

	_last_time = nowTime;
}

WtDataReaderAD::WtLMDBPtr WtDataReaderAD::get_k_db(const char* exchg, WTSKlinePeriod period)
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
		pipe_reader_log(_sink, LL_ERROR, "Opening {} db at {} failed: {}", subdir, path, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}

	(*the_map)[exchg] = dbPtr;
	return std::move(dbPtr);
}

WtDataReaderAD::WtLMDBPtr WtDataReaderAD::get_t_db(const char* exchg, const char* code)
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
		pipe_reader_log(_sink, LL_ERROR, "Opening tick db at {} failed: {}", path, dbPtr->errmsg());
		return std::move(WtLMDBPtr());
	}

	_tick_dbs[exchg] = dbPtr;
	return std::move(dbPtr);
}