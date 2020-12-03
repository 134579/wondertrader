#include "WtDataWriter.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IniHelper.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Includes/IBaseDataMgr.h"

#include "../WTSTools/WTSCmpHelper.hpp"

#pragma comment(lib, "libmysql.lib")


#include <set>

extern "C"
{
	EXPORT_FLAG IDataWriter* createWriter()
	{
		IDataWriter* ret = new WtDataWriter();
		return ret;
	}

	EXPORT_FLAG void deleteWriter(IDataWriter* &writer)
	{
		if (writer != NULL)
		{
			delete writer;
			writer = NULL;
		}
	}
};

static const uint32_t CACHE_SIZE_STEP = 200;
static const uint32_t TICK_SIZE_STEP = 2500;
static const uint32_t KLINE_SIZE_STEP = 200;

const char CMD_CLEAR_CACHE[] = "CMD_CLEAR_CACHE";
const char MARKER_FILE[] = "marker.ini";

#ifdef _WIN32
#include <wtypes.h>
HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}
#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void *)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif

std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = g_moduleName;
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir;
}



WtDataWriter::WtDataWriter()
	: _terminated(false)
	, _save_tick_log(false)
	, _log_group_size(1000)
{
}


WtDataWriter::~WtDataWriter()
{
}

bool WtDataWriter::isSessionProceeded(const char* sid)
{
	auto it = _proc_date.find(sid);
	if (it == _proc_date.end())
		return false;

	return (it->second >= TimeUtils::getCurDate());
}

void WtDataWriter::init_db()
{
	if (!_db_conf._active)
		return;

#ifdef _WIN32
	std::string module = getBinDir() + "libmysql.dll";
	DLLHelper::load_library(module.c_str());
#endif

	_db_conn.reset(new MysqlDb);
	my_bool autoreconnect = true;
	_db_conn->options(MYSQL_OPT_RECONNECT, &autoreconnect);
	_db_conn->options(MYSQL_SET_CHARSET_NAME, "utf8");

	if (_db_conn->connect(_db_conf._dbname, _db_conf._host, _db_conf._user, _db_conf._pass, _db_conf._port, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
	{
		if(_sink)
			_sink->outputWriterLog(LL_INFO, "数据库连接成功:%s:%d", _db_conf._host, _db_conf._port);
	}
	else
	{
		if (_sink)
			_sink->outputWriterLog(LL_ERROR, "数据库连接失败[%s:%d]:%s", _db_conf._host, _db_conf._port, _db_conn->errstr());
		_db_conn.reset();
	}
}

bool WtDataWriter::init(WTSVariant* params, IDataWriterSink* sink)
{
	_sink = sink;
	_bd_mgr = sink->getBDMgr();
	_save_tick_log = params->getBoolean("savelog");

	_base_dir = StrUtil::standardisePath(params->getCString("path"));
	if (!BoostFile::exists(_base_dir.c_str()))
		BoostFile::create_directories(_base_dir.c_str());
	_cache_file = params->getCString("cache");
	if (_cache_file.empty())
		_cache_file = "cache.dmb";

	_async_proc = params->getBoolean("async");
	_log_group_size = params->getUInt32("groupsize");

	{
		std::string filename = _base_dir + MARKER_FILE;
		IniHelper iniHelper;
		iniHelper.load(filename.c_str());
		StringVector ayKeys, ayVals;
		iniHelper.readSecKeyValArray("markers", ayKeys, ayVals);
		for (uint32_t idx = 0; idx < ayKeys.size(); idx++)
		{
			_proc_date[ayKeys[idx].c_str()] = strtoul(ayVals[idx].c_str(), 0, 10);
		}
	}

	WTSVariant* dbConf = params->get("db");
	if(dbConf)
	{
		strcpy(_db_conf._host, dbConf->getCString("host"));
		strcpy(_db_conf._dbname, dbConf->getCString("dbname"));
		strcpy(_db_conf._user, dbConf->getCString("user"));
		strcpy(_db_conf._pass, dbConf->getCString("pass"));
		_db_conf._port = dbConf->getInt32("port");

		_db_conf._active = (strlen(_db_conf._host) > 0) && (strlen(_db_conf._dbname) > 0) && (_db_conf._port != 0);
		if (_db_conf._active)
			init_db();
	}

	loadCache();

	_proc_chk.reset(new BoostThread(boost::bind(&WtDataWriter::check_loop, this)));
	return true;
}

void WtDataWriter::release()
{
	_terminated = true;
	if (_proc_thrd)
	{
		_proc_cond.notify_all();
		_proc_thrd->join();
	}
}

/*
void DataManager::preloadRtCaches(const char* exchg)
{
	if (!_preload_enable || _preloaded)
		return;

	_sink->outputWriterLog(LL_INFO, "开始预加载实时数据缓存文件……");
	TimeUtils::Ticker ticker;
	uint32_t cnt = 0;
	uint32_t codecnt = 0;
	WTSArray* ayCts = _bd_mgr->getContracts(exchg);
	if (ayCts != NULL && ayCts->size() > 0)
	{
		for (auto it = ayCts->begin(); it != ayCts->end(); it++)
		{
			WTSContractInfo* ct = (WTSContractInfo*)(*it);
			if (ct == NULL)
				continue;
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);
			if(commInfo == NULL)
				continue;

			bool isStk = (commInfo->getCategoty() == CC_Stock);
			codecnt++;
			
			releaseBlock(getTickBlock(ct->getCode(), TimeUtils::getCurDate(), true));
			releaseBlock(getKlineBlock(ct->getCode(), KP_Minute1, true));
			releaseBlock(getKlineBlock(ct->getCode(), KP_Minute5, true));
			cnt += 3;
			if (isStk && strcmp(commInfo->getProduct(), "STK") == 0)
			{
				releaseBlock(getOrdQueBlock(ct->getCode(), TimeUtils::getCurDate(), true));
				releaseBlock(getTransBlock(ct->getCode(), TimeUtils::getCurDate(), true));
				cnt += 2;
				if (strcmp(ct->getExchg(), "SZSE") == 0)
				{
					releaseBlock(getOrdDtlBlock(ct->getCode(), TimeUtils::getCurDate(), true));
					cnt++;
				}
			}
		}
	}

	if (ayCts != NULL)
		ayCts->release();
	_sink->outputWriterLog(LL_INFO, "预加载%个品种的实时数据缓存文件%u个，耗时%s微秒", codecnt, cnt, WTSLogger::fmtInt64(ticker.micro_seconds()));
	_preloaded = true;
}
*/

void WtDataWriter::loadCache()
{
	if (_tick_cache_file != NULL)
		return;

	bool bNew = false;
	std::string filename = _base_dir + _cache_file;
	if (!BoostFile::exists(filename.c_str()))
	{
		uint64_t uSize = sizeof(RTTickCache) + sizeof(TickCacheItem) * CACHE_SIZE_STEP;
		BoostFile bf;
		bf.create_new_file(filename.c_str());
		bf.truncate_file((uint32_t)uSize);
		bf.close_file();
		bNew = true;
	}

	_tick_cache_file.reset(new BoostMappingFile);
	_tick_cache_file->map(filename.c_str());
	_tick_cache_block = (RTTickCache*)_tick_cache_file->addr();

	_tick_cache_block->_size = min(_tick_cache_block->_size, _tick_cache_block->_capacity);

	if(bNew)
	{
		memset(_tick_cache_block, 0, _tick_cache_file->size());

		_tick_cache_block->_capacity = CACHE_SIZE_STEP;
		_tick_cache_block->_type = BT_RT_Cache;
		_tick_cache_block->_size = 0;
		_tick_cache_block->_version = 1;
		strcpy(_tick_cache_block->_blk_flag, BLK_FLAG);
	}
	else
	{
		for (uint32_t i = 0; i < _tick_cache_block->_size; i++)
		{
			const TickCacheItem& item = _tick_cache_block->_ticks[i];
			std::string key = StrUtil::printf("%s.%s", item._tick.exchg, item._tick.code);
			_tick_cache_idx[key] = i;
		}
	}
}

/*
void* DataManager::resizeKBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	RTKlineBlock* kBlock = (RTKlineBlock*)mfPtr->addr();
	if (kBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(RTKlineBlock) + sizeof(WTSBarStruct)*nCount;
	std::filebuf fbuf;
	fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	//这里没大搞明白
	fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
	fbuf.sputc(0);
	fbuf.close();

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	kBlock = (RTKlineBlock*)mfPtr->addr();
	kBlock->_capacity = nCount;

	return mfPtr->addr();
}
*/

template<typename HeaderType, typename T>
void* WtDataWriter::resizeRTBlock(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	//调用该函数之前，应该已经申请了写锁了
	RTBlockHeader* tBlock = (RTBlockHeader*)mfPtr->addr();
	if (tBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(HeaderType) + sizeof(T)*nCount;
	try
	{
		std::filebuf fbuf;
		fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		//这里没大搞明白
		fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
		fbuf.sputc(0);
		fbuf.close();
	}
	catch(std::exception& ex)
	{
		_sink->outputWriterLog(LL_ERROR, "%s[%u]的实时缓存文件扩容出现异常: %s", ex.what());
		return mfPtr->addr();
	}

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	tBlock = (RTBlockHeader*)mfPtr->addr();
	tBlock->_capacity = nCount;
	return mfPtr->addr();
}

/*
void* DataManager::resizeTCache(BoostMFPtr& mfPtr, uint32_t nCount)
{
	if (mfPtr == NULL)
		return NULL;

	RTTickCache* tCache = (RTTickCache*)mfPtr->addr();
	if (tCache->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(RTTickCache) + sizeof(TickCacheItem)*nCount;
	std::filebuf fbuf;
	fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	//这里没大搞明白
	fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
	fbuf.sputc(0);
	fbuf.close();

	BoostMappingFile* pNewMf = new BoostMappingFile();
	if (!pNewMf->map(filename))
	{
		delete pNewMf;
		return NULL;
	}

	mfPtr.reset(pNewMf);

	tCache = (RTTickCache*)mfPtr->addr();
	tCache->_capacity = nCount;
	return mfPtr->addr();
}
*/

bool WtDataWriter::writeTick(WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (curTick == NULL)
		return false;

	curTick->retain();
	pushTask([this, curTick, bNeedSlice](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curTick->code(), curTick->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			//先更新缓存
			if (!updateCache(ct, curTick, bNeedSlice))
				break;

			//写到tick缓存
			pipeToTicks(ct, curTick);

			//写到K线缓存
			pipeToKlines(ct, curTick);

			_sink->broadcastTick(curTick);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTick->exchg()]++;
			if (_tcnt_map[curTick->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的tick数据%s条", curTick->exchg(), StrUtil::fmtUInt64(_tcnt_map[curTick->exchg()]).c_str());
			}
		} while (false);

		curTick->release();
	});
	return true;
}

bool WtDataWriter::writeOrderQueue(WTSOrdQueData* curOrdQue)
{
	if (curOrdQue == NULL)
		return false;

	curOrdQue->retain();
	pushTask([this, curOrdQue](){

		do
		{
			WTSContractInfo* ct = _bd_mgr->getContract(curOrdQue->code(), curOrdQue->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdQueBlockPair* pBlockPair = getOrdQueBlock(ct, curOrdQue->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTOrdQueBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdQueBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdQueStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_queues[blk->_size], &curOrdQue->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdQue->exchg()]++;
			if (_tcnt_map[curOrdQue->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的委托队列数据%s条", curOrdQue->exchg(), StrUtil::fmtUInt64(_tcnt_map[curOrdQue->exchg()]).c_str());
			}
		} while (false);
		curOrdQue->release();
	});
	return true;
}

void WtDataWriter::pushTask(TaskInfo task)
{
	if(_async_proc)
	{
		BoostUniqueLock lck(_task_mtx);
		_tasks.push(task);
		_task_cond.notify_all();
	}
	else
	{
		task();
		return;
	}

	if(_task_thrd == NULL)
	{
		_task_thrd.reset(new StdThread([this](){
			while (!_terminated)
			{
				if(_tasks.empty())
				{
					BoostUniqueLock lck(_task_mtx);
					_task_cond.wait(_task_mtx);
					continue;
				}

				std::queue<TaskInfo> tempQueue;
				{
					BoostUniqueLock lck(_task_mtx);
					tempQueue.swap(_tasks);
				}

				while(!tempQueue.empty())
				{
					TaskInfo& curTask = tempQueue.front();
					curTask();
					tempQueue.pop();
				}
			}
		}));
	}
}

bool WtDataWriter::writeOrderDetail(WTSOrdDtlData* curOrdDtl)
{
	if (curOrdDtl == NULL)
		return false;

	curOrdDtl->retain();
	pushTask([this, curOrdDtl](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curOrdDtl->code(), curOrdDtl->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdDtlBlockPair* pBlockPair = getOrdDtlBlock(ct, curOrdDtl->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTOrdDtlBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdDtlBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdDtlStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_details[blk->_size], &curOrdDtl->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdDtl->exchg()]++;
			if (_tcnt_map[curOrdDtl->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的逐笔委托数据%s条", curOrdDtl->exchg(), StrUtil::fmtUInt64(_tcnt_map[curOrdDtl->exchg()]).c_str());
			}
		} while (false);

		curOrdDtl->release();
	});
	
	return true;
}

bool WtDataWriter::writeTransaction(WTSTransData* curTrans)
{
	if (curTrans == NULL)
		return false;

	curTrans->retain();
	pushTask([this, curTrans](){

		do
		{

			WTSContractInfo* ct = _bd_mgr->getContract(curTrans->code(), curTrans->exchg());
			WTSCommodityInfo* commInfo = _bd_mgr->getCommodity(ct);

			//再根据状态过滤
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			TransBlockPair* pBlockPair = getTransBlock(ct, curTrans->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//先检查容量够不够，不够要扩
			RTTransBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTTransBlock*)resizeRTBlock<RTDayBlockHeader, WTSTransStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_trans[blk->_size], &curTrans->getTransStruct(), sizeof(WTSTransStruct));
			blk->_size += 1;

			//TODO: 要广播的
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTrans->exchg()]++;
			if (_tcnt_map[curTrans->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "共收到交易所%s的逐笔成交数据%s条", curTrans->exchg(), StrUtil::fmtUInt64(_tcnt_map[curTrans->exchg()]).c_str());
			}
		} while (false);

		curTrans->release();
	});
	return true;
}

void WtDataWriter::pipeToTicks(WTSContractInfo* ct, WTSTickData* curTick)
{
	TickBlockPair* pBlockPair = getTickBlock(ct, curTick->tradingdate());
	if (pBlockPair == NULL)
		return;

	BoostUniqueLock lock(pBlockPair->_mutex);

	//先检查容量够不够，不够要扩
	RTTickBlock* blk = pBlockPair->_block;
	if(blk->_size >= blk->_capacity)
	{
		pBlockPair->_file->sync();
		pBlockPair->_block = (RTTickBlock*)resizeRTBlock<RTDayBlockHeader, WTSTickStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
		blk = pBlockPair->_block;
	}

	memcpy(&blk->_ticks[blk->_size], &curTick->getTickStruct(), sizeof(WTSTickStruct));
	blk->_size += 1;

	if(_save_tick_log && pBlockPair->_fstream)
	{
		*(pBlockPair->_fstream) << curTick->code() << ","
			<< curTick->tradingdate() << ","
			<< curTick->actiondate() << ","
			<< curTick->actiontime() << ","
			<< TimeUtils::getLocalTime(false) << ","
			<< curTick->price() << ","
			<< curTick->totalvolumn() << ","
			<< curTick->openinterest() << ","
			<< (uint64_t)curTick->totalturnover() << ","
			<< curTick->volumn() << ","
			<< curTick->additional() << ","
			<< (uint64_t)curTick->turnover() << std::endl;
	}
}

WtDataWriter::OrdQueBlockPair* WtDataWriter::getOrdQueBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdQueBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_ordque_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/queue/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdQueBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orderqueue缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_queues, 0, sizeof(WTSOrdQueStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_OrdQueue;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			do
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdQueStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的委托队列缓存文件已修复", ct->getCode(), curDate);
				}

			} while (false);

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::OrdDtlBlockPair* WtDataWriter::getOrdDtlBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	OrdDtlBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_orddtl_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/orders/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdDtlBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orders缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_details, 0, sizeof(WTSOrdDtlStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_OrdDetail;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdDtlStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的逐笔成交缓存文件已修复", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::TransBlockPair* WtDataWriter::getTransBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TransBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_trans_blocks[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/trans/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());
		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTransBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "trans缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_trans, 0, sizeof(WTSTransStruct)*pBlock->_block->_capacity);
		}

		if (isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_Trnsctn;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSTransStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s股票%u的逐笔成交缓存文件已修复", ct->getCode(), curDate);
				}

				break;
			}

		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WtDataWriter::TickBlockPair* WtDataWriter::getTickBlock(WTSContractInfo* ct, uint32_t curDate, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	TickBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	pBlock = &_rt_ticks_blocks[key];
	if(pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/ticks/%s/", _base_dir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());

		if(_save_tick_log)
		{
			std::stringstream fname;
			fname << path << ct->getCode() << "." << curDate << ".csv";
			pBlock->_fstream.reset(new std::ofstream());
			pBlock->_fstream->open(fname.str().c_str(), std::ios_base::app);
		}

		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * TICK_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s开始映射", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if(!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTickBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%s映射成功", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "tick缓存模块%s日期%u不等于当前日期%u，重新初始化", path.c_str(), pBlock->_block->_date, curDate);
			pBlock->_block->_size = 0;
			pBlock->_block->_date = curDate;

			memset(&pBlock->_block->_ticks, 0, sizeof(WTSTickStruct)*pBlock->_block->_capacity);
		}

		if(isNew)
		{
			pBlock->_block->_capacity = TICK_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = BT_RT_Ticks;
			pBlock->_block->_date = curDate;
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
		else
		{
			//检查缓存文件是否有问题，要自动恢复
			for (;;)
			{
				uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTTickBlock)) / sizeof(WTSTickStruct));
					//文件大小不匹配，一般是因为capacity改了，但是实际没扩容
					//这是做一次扩容即可
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s合约%u的tick缓存文件已修复", ct->getCode(), curDate);
				}
				
				break;
			}
			
		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

void WtDataWriter::pipeToKlines(WTSContractInfo* ct, WTSTickData* curTick)
{
	uint32_t uDate = curTick->actiondate();
	WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
	uint32_t curTime = curTick->actiontime() / 100000;

	uint32_t minutes = sInfo->timeToMinutes(curTime, false);
	if (minutes == INVALID_UINT32)
		return;

	//当秒数为0，要专门处理，比如091500000，这笔tick要算作0915的
	//如果是小节结束，要算作小节结束那一分钟，因为经常会有超过结束时间的价格进来，如113000500
	//不能同时处理，所以用or	
	if (sInfo->isLastOfSection(curTime))
	{
		minutes--;
	}

	//更新1分钟线
	KBlockPair* pBlockPair = getKlineBlock(ct, KP_Minute1);
	if (pBlockPair && pBlockPair->_block)
	{
		BoostUniqueLock lock(pBlockPair->_mutex);
		RTKlineBlock* blk = pBlockPair->_block;
		if (blk->_size == blk->_capacity)
		{
			pBlockPair->_file->sync();
			pBlockPair->_block = (RTKlineBlock*)resizeRTBlock<RTKlineBlock, WTSBarStruct>(pBlockPair->_file, blk->_capacity + KLINE_SIZE_STEP);
			blk = pBlockPair->_block;
		}

		WTSBarStruct* lastBar = NULL;
		if(blk->_size > 0)
		{
			lastBar = &blk->_bars[blk->_size - 1];
		}

		//拼接1分钟线
		uint32_t barMins = minutes + 1;
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

		bool bNew = false;
		if (lastBar == NULL || barTime > lastBar->time)
		{
			bNew = true;
		}

		WTSBarStruct* newBar = NULL;
		if (bNew)
		{
			newBar = &blk->_bars[blk->_size];
			blk->_size += 1;

			newBar->date = curTick->tradingdate();
			newBar->time = barTime;
			newBar->open = curTick->price();
			newBar->high = curTick->price();
			newBar->low = curTick->price();
			newBar->close = curTick->price();

			newBar->vol = curTick->volumn();
			newBar->money = curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add = curTick->additional();
		}
		else
		{
			newBar = &blk->_bars[blk->_size-1];

			newBar->close = curTick->price();
			newBar->high = max(curTick->price(), newBar->high);
			newBar->low = min(curTick->price(), newBar->low);

			newBar->vol += curTick->volumn();
			newBar->money += curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add += curTick->additional();
		}
	}


	pBlockPair = getKlineBlock(ct, KP_Minute5);
	if (pBlockPair && pBlockPair->_block)
	{
		BoostUniqueLock lock(pBlockPair->_mutex);
		RTKlineBlock* blk = pBlockPair->_block;
		if (blk->_size == blk->_capacity)
		{
			pBlockPair->_file->sync();
			pBlockPair->_block = (RTKlineBlock*)resizeRTBlock<RTKlineBlock, WTSBarStruct>(pBlockPair->_file, blk->_capacity + KLINE_SIZE_STEP);
			blk = pBlockPair->_block;
		}

		WTSBarStruct* lastBar = NULL;
		if (blk->_size > 0)
		{
			lastBar = &blk->_bars[blk->_size - 1];
		}

		uint32_t barMins = (minutes / 5) * 5 + 5;
		uint32_t barTime = sInfo->minuteToTime(barMins);
		uint32_t barDate = uDate;
		if (barTime == 0)
		{
			barDate = TimeUtils::getNextDate(barDate);
		}
		barTime = TimeUtils::timeToMinBar(barDate, barTime);

		bool bNew = false;
		if (lastBar == NULL || barTime > lastBar->time)
		{
			bNew = true;
		}

		WTSBarStruct* newBar = NULL;
		if (bNew)
		{
			newBar = &blk->_bars[blk->_size];
			blk->_size += 1;

			newBar->date = curTick->tradingdate();
			newBar->time = barTime;
			newBar->open = curTick->price();
			newBar->high = curTick->price();
			newBar->low = curTick->price();
			newBar->close = curTick->price();

			newBar->vol = curTick->volumn();
			newBar->money = curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add = curTick->additional();
		}
		else
		{
			newBar = &blk->_bars[blk->_size - 1];

			newBar->close = curTick->price();
			newBar->high = max(curTick->price(), newBar->high);
			newBar->low = min(curTick->price(), newBar->low);

			newBar->vol += curTick->volumn();
			newBar->money += curTick->turnover();
			newBar->hold = curTick->openinterest();
			newBar->add += curTick->additional();
		}
	}
}

template<typename T>
void WtDataWriter::releaseBlock(T* block)
{
	if (block == NULL || block->_file == NULL)
		return;

	BoostUniqueLock lock(block->_mutex);
	block->_block = NULL;
	block->_file.reset();
	block->_lasttime = 0;
}

WtDataWriter::KBlockPair* WtDataWriter::getKlineBlock(WTSContractInfo* ct, WTSKlinePeriod period, bool bAutoCreate /* = true */)
{
	if (ct == NULL)
		return NULL;

	KBlockPair* pBlock = NULL;
	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());

	KBlockFilesMap* cache_map = NULL;
	std::string subdir = "";
	BlockType bType;
	switch(period)
	{
	case KP_Minute1: 
		cache_map = &_rt_min1_blocks; 
		subdir = "min1";
		bType = BT_RT_Minute1;
		break;
	case KP_Minute5: 
		cache_map = &_rt_min5_blocks;
		subdir = "min5";
		bType = BT_RT_Minute5;
		break;
	default: break;
	}

	if (cache_map == NULL)
		return NULL;

	pBlock = &(*cache_map)[key];
	if (pBlock->_block == NULL)
	{
		std::string path = StrUtil::printf("%srt/%s/%s/", _base_dir.c_str(), subdir.c_str(), ct->getExchg());
		BoostFile::create_directories(path.c_str());

		path += ct->getCode();
		path += ".dmb";

		bool isNew = false;
		if (!BoostFile::exists(path.c_str()))
		{
			if (!bAutoCreate)
				return NULL;

			_sink->outputWriterLog(LL_INFO, "数据文件%s不存在，正在初始化……", path.c_str());

			uint64_t uSize = sizeof(RTKlineBlock) + sizeof(WTSBarStruct) * KLINE_SIZE_STEP;
			/*
			std::filebuf fbuf;
			fbuf.open(path, std::ios_base::in | std::ios_base::out
				| std::ios_base::trunc | std::ios_base::binary);

			//定位到倒数第二个字符，然后写入一个0，这样文件大小就固定为指定大小了
			fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
			fbuf.sputc(0);
			fbuf.close();
			*/

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		pBlock->_file.reset(new BoostMappingFile);
		if(pBlock->_file->map(path.c_str()))
		{
			pBlock->_block = (RTKlineBlock*)pBlock->_file->addr();
		}
		else
		{
			_sink->outputWriterLog(LL_INFO, "数据文件%s映射失败", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}

		//if(pBlock->_block->_date != uDate)
		//{
		//	pBlock->_block->_size = 0;
		//	pBlock->_block->_date = uDate;

		//	memset(&pBlock->_block->_bars, 0, sizeof(WTSBarStruct)*pBlock->_block->_capacity);
		//}

		if (isNew)
		{
			//memset(pBlock->_block, 0, pBlock->_file->size());
			pBlock->_block->_capacity = KLINE_SIZE_STEP;
			pBlock->_block->_size = 0;
			pBlock->_block->_version = BLOCK_VERSION_RAW;
			pBlock->_block->_type = bType;
			pBlock->_block->_date = TimeUtils::getCurDate();
			strcpy(pBlock->_block->_blk_flag, BLK_FLAG);
		}
	}

	pBlock->_lasttime = time(NULL);
	return pBlock;
}

WTSTickData* WtDataWriter::getCurTick(const char* code, const char* exchg/* = ""*/)
{
	if (strlen(code) == 0)
		return NULL;

	WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
	if (ct == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());
	BoostUniqueLock lock(_mtx_tick_cache);
	auto it = _tick_cache_idx.find(key);
	if (it == _tick_cache_idx.end())
		return NULL;

	uint32_t idx = it->second;
	TickCacheItem& item = _tick_cache_block->_ticks[idx];
	return WTSTickData::create(item._tick);
}

bool WtDataWriter::updateCache(WTSContractInfo* ct, WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (curTick == NULL || _tick_cache_block == NULL)
	{
		_sink->outputWriterLog(LL_ERROR, "tick数据缓存尚未初始化");
		return false;
	}

	BoostUniqueLock lock(_mtx_tick_cache);
	std::string key = StrUtil::printf("%s.%s", curTick->exchg(), curTick->code());
	uint32_t idx = 0;
	if (_tick_cache_idx.find(key) == _tick_cache_idx.end())
	{
		idx = _tick_cache_block->_size;
		_tick_cache_idx[key] = _tick_cache_block->_size;
		_tick_cache_block->_size += 1;
		if(_tick_cache_block->_size >= _tick_cache_block->_capacity)
		{
			_tick_cache_block = (RTTickCache*)resizeRTBlock<RTTickCache, TickCacheItem>(_tick_cache_file, _tick_cache_block->_capacity + CACHE_SIZE_STEP);
			_sink->outputWriterLog(LL_INFO, "%s缓存已重构为%u大小", key.c_str(), _tick_cache_block->_capacity);
		}
	}
	else
	{
		idx = _tick_cache_idx[key];
	}


	TickCacheItem& item = _tick_cache_block->_ticks[idx];
	if (curTick->tradingdate() < item._date)
	{
		_sink->outputWriterLog(LL_INFO, "%s行情日期%u小于缓存交易日%u", curTick->tradingdate(), item._date);
		return false;
	}

	WTSTickStruct& newTick = curTick->getTickStruct();

	if (curTick->tradingdate() > item._date)
	{
		//新数据交易日大于老数据，则认为是新一天的数据
		item._date = curTick->tradingdate();
		memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		if (bNeedSlice)
		{
			item._tick.volumn = item._tick.total_volumn;
			item._tick.turn_over = item._tick.total_turnover;
			item._tick.diff_interest = item._tick.open_interest - item._tick.pre_interest;

			newTick.volumn = newTick.total_volumn;
			newTick.turn_over = newTick.total_turnover;
			newTick.diff_interest = newTick.open_interest - newTick.pre_interest;
		}

		_sink->outputWriterLog(LL_INFO, "新交易日%u的第一笔数据,%s.%s,%u,%f,%d,%d", newTick.trading_date, curTick->exchg(), curTick->code(), curTick->volumn(), curTick->turnover(), curTick->openinterest(), curTick->additional());
	}
	else
	{
		//如果缓存里的数据日期大于最新行情的日期
		//或者缓存里的时间大于等于最新行情的时间，数据就不需要处理
		WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
		uint32_t tdate = sInfo->getOffsetDate(curTick->actiondate(), curTick->actiontime() / 100000);
		if (tdate > curTick->tradingdate())
		{
			_sink->outputWriterLog(LL_ERROR, "合约%s.%s最新tick数据(时间%u.%u)异常，丢弃", curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime());
			return false;
		}
		else if (curTick->totalvolumn() < item._tick.total_volumn)
		{
			_sink->outputWriterLog(LL_ERROR, "合约%s.%s最新tick数据(时间%u.%u，总成交%u小于缓存总成交%u)异常，丢弃", 
				curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime(), curTick->totalvolumn(), item._tick.total_volumn);
			return false;
		}

		//时间戳相同，但是成交量大于等于原来的，这种情况一般是郑商所，这里的处理方式就是时间戳+500毫秒
		if(newTick.action_date == item._tick.action_date && newTick.action_time == item._tick.action_time && newTick.total_volumn >= item._tick.volumn)
		{
			newTick.action_time += 500;
		}

		//这里就要看需不需要预处理了
		if(!bNeedSlice)
		{
			memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		}
		else
		{
			newTick.volumn = newTick.total_volumn - item._tick.total_volumn;
			newTick.turn_over = newTick.total_turnover - item._tick.total_turnover;
			newTick.diff_interest = newTick.open_interest - item._tick.open_interest;

			memcpy(&item._tick, &newTick, sizeof(WTSTickStruct));
		}
	}

	return true;
}

void WtDataWriter::transHisData(const char* sid)
{
	BoostUniqueLock lock(_proc_mtx);
	if (strcmp(sid, CMD_CLEAR_CACHE) != 0)
	{
		CodeSet* pCommSet = _sink->getSessionComms(sid);
		if (pCommSet == NULL)
			return;

		for (auto it = pCommSet->begin(); it != pCommSet->end(); it++)
		{
			const std::string& key = *it;

			const StringVector& ay = StrUtil::split(key, ".");
			const char* exchg = ay[0].c_str();
			const char* pid = ay[1].c_str();

			WTSCommodityInfo* pCommInfo = _bd_mgr->getCommodity(exchg, pid);
			if (pCommInfo == NULL)
				continue;

			const CodeSet& codes = pCommInfo->getCodes();
			for (auto code : codes)
			{
				WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg);
				if(ct)
					_proc_que.push(ct->getFullCode());
			}
		}

		_proc_que.push(StrUtil::printf("MARK.%s", sid));
	}
	else
	{
		_proc_que.push(sid);
	}

	if (_proc_thrd == NULL)
	{
		_proc_thrd.reset(new BoostThread(boost::bind(&WtDataWriter::proc_loop, this)));
	}
	else
	{
		_proc_cond.notify_all();
	}
}

void WtDataWriter::check_loop()
{
	uint32_t expire_secs = 600;
	while(!_terminated)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(10));
		uint64_t now = time(NULL);
		for (auto it = _rt_ticks_blocks.begin(); it != _rt_ticks_blocks.end(); it++)
		{
			const std::string& key = it->first;
			TickBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "tick缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<TickBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_trans_blocks.begin(); it != _rt_trans_blocks.end(); it++)
		{
			const std::string& key = it->first;
			TransBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "trans缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<TransBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_orddtl_blocks.begin(); it != _rt_orddtl_blocks.end(); it++)
		{
			const std::string& key = it->first;
			OrdDtlBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "order缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<OrdDtlBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_ordque_blocks.begin(); it != _rt_ordque_blocks.end(); it++)
		{
			const std::string& key = it->first;
			OrdQueBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "queue缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<OrdQueBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_min1_blocks.begin(); it != _rt_min1_blocks.end(); it++)
		{
			const std::string& key = it->first;
			KBlockPair* kBlk = &it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "1分钟缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<KBlockPair>(kBlk);
			}
		}

		for (auto it = _rt_min5_blocks.begin(); it != _rt_min5_blocks.end(); it++)
		{
			const std::string& key = it->first;
			KBlockPair* kBlk = &it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "5分钟缓存 %s 映射超时，自动关闭", key.c_str());
				releaseBlock<KBlockPair>(kBlk);
			}
		}
	}
}

uint32_t WtDataWriter::dump_hisdata_to_db(WTSContractInfo* ct)
{
	if (ct == NULL)
		return 0;

	if (!_db_conf._active || _db_conn == NULL)
		return 0;

	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());

	uint32_t count = 0;

	MysqlDb& db = *_db_conn;
	MysqlTransaction trans(db);
	//从缓存中读取最新tick，更新到历史日线
	auto it = _tick_cache_idx.find(key);
	if (it != _tick_cache_idx.end())
	{
		uint32_t idx = it->second;

		const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
		const WTSTickStruct& ts = tci._tick;

		char sql[512] = { 0 };
		sprintf(sql, "REPLACE INTO tb_kline_day(exchange,code,date,open,high,low,close,settle,volume,turnover,interest,diff_interest) "
			"VALUES('%s','%s',%u,%f,%f,%f,%f,%f,%u,%f,%u,%d);", ct->getExchg(), ct->getCode(), ts.trading_date, ts.open, ts.high, ts.low, ts.price, 
			ts.settle_price, ts.total_volumn, ts.total_turnover, ts.open_interest, ts.diff_interest);

		MysqlQuery query(db);
		if(!query.exec(sql))
		{
			_sink->outputWriterLog(LL_ERROR, "day数据收盘作业失败：%s", query.errormsg());
		}
		else
		{
			count++;
		}
	}

	//转移实时1分钟线
	KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute1, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		_sink->outputWriterLog(LL_INFO, "开始转移合约%s的1分钟数据", ct->getFullCode());
		BoostUniqueLock lock(kBlkPair->_mutex);

		std::string sql = "REPLACE INTO tb_kline_min1(exchange,code,date,time,open,high,low,close,volume,turnover,interest,diff_interest) VALUES";
		for(uint32_t i = 0; i < size; i++)
		{
			const WTSBarStruct& bs = kBlkPair->_block->_bars[i];
			sql += StrUtil::printf("('%s','%s',%u,%u,%f,%f,%f,%f,%u,%f,%u,%d),", ct->getExchg(), ct->getCode(), bs.date, bs.time, bs.open,
				bs.high, bs.low, bs.close, bs.vol, bs.money, bs.hold, bs.add);
		}
		sql = sql.substr(0, sql.size() - 1);
		sql += ";";

		MysqlQuery query(db);
		if (!query.exec(sql))
		{
			_sink->outputWriterLog(LL_ERROR, "min1数据收盘作业失败：%s", query.errormsg());
		}
		else
		{
			count += size;
			//最后将缓存清空
			kBlkPair->_block->_size = 0;
		}
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	//第四步，转移实时5分钟线
	kBlkPair = getKlineBlock(ct, KP_Minute5, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		_sink->outputWriterLog(LL_INFO, "开始转移合约%s的5分钟数据", ct->getFullCode());
		BoostUniqueLock lock(kBlkPair->_mutex);

		std::string sql = "REPLACE INTO tb_kline_min5(exchange,code,date,time,open,high,low,close,volume,turnover,interest,diff_interest) VALUES";
		for (uint32_t i = 0; i < size; i++)
		{
			const WTSBarStruct& bs = kBlkPair->_block->_bars[i];
			sql += StrUtil::printf("('%s','%s',%u,%u,%f,%f,%f,%f,%u,%f,%u,%d),", ct->getExchg(), ct->getCode(), bs.date, bs.time, bs.open,
				bs.high, bs.low, bs.close, bs.vol, bs.money, bs.hold, bs.add);
		}
		sql = sql.substr(0, sql.size() - 1);
		sql += ";";

		MysqlQuery query(db);
		if (!query.exec(sql))
		{
			_sink->outputWriterLog(LL_ERROR, "min5数据收盘作业失败：%s", query.errormsg());
		}
		else
		{
			count += size;
			//最后将缓存清空
			kBlkPair->_block->_size = 0;
		}
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	trans.commit();

	return count;
}

uint32_t WtDataWriter::dump_hisdata_to_file(WTSContractInfo* ct)
{
	if (ct == NULL)
		return 0;

	std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());

	uint32_t count = 0;

	//从缓存中读取最新tick，更新到历史日线
	auto it = _tick_cache_idx.find(key);
	if (it != _tick_cache_idx.end())
	{
		uint32_t idx = it->second;

		const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
		const WTSTickStruct& ts = tci._tick;

		WTSBarStruct bs;
		bs.date = ts.trading_date;
		bs.time = 0;
		bs.open = ts.open;
		bs.close = ts.price;
		bs.high = ts.high;
		bs.low = ts.low;
		bs.settle = ts.settle_price;
		bs.vol = ts.total_volumn;
		bs.hold = ts.open_interest;
		bs.money = ts.total_turnover;
		bs.add = ts.open_interest - ts.pre_interest;

		std::stringstream ss;
		ss << _base_dir << "his/day/" << ct->getExchg() << "/";
		std::string path = ss.str();
		BoostFile::create_directories(ss.str().c_str());
		std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

		bool bNew = false;
		if (!BoostFile::exists(filename.c_str()))
			bNew = true;

		BoostFile f;
		if (f.create_or_open_file(filename.c_str()))
		{
			bool bNeedWrite = true;
			if (bNew)
			{
				BlockHeader header;
				strcpy(header._blk_flag, BLK_FLAG);
				header._type = BT_HIS_Day;
				header._version = BLOCK_VERSION_RAW;

				f.write_file(&header, sizeof(header));

				f.write_file(&bs, sizeof(WTSBarStruct));
				count++;
			}
			else
			{
				//日线必须要检查一下
				std::string content;
				BoostFile::read_file_contents(filename.c_str(), content);
				uint64_t flength = content.size();

				HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
				if (strcmp(kBlock->_blk_flag, BLK_FLAG) != 0)
				{
					_sink->outputWriterLog(LL_ERROR, "历史数据文件%s头部异常，将清除数据重建", filename.c_str());
					f.truncate_file(0);
					BlockHeader header;
					strcpy(header._blk_flag, BLK_FLAG);
					header._type = BT_HIS_Day;
					header._version = BLOCK_VERSION_RAW;

					f.write_file(&header, sizeof(header));

					f.write_file(&bs, sizeof(WTSBarStruct));
					count++;
				}
				else
				{
					std::vector<WTSBarStruct>	bars;
					if (kBlock->_version == BLOCK_VERSION_RAW)	//如果老的文件是非压缩版本，则直接把K线读到缓存里去
					{
						uint32_t barcnt = (uint32_t)(flength - BLOCK_HEADER_SIZE) / sizeof(WTSBarStruct);
						bars.resize(barcnt);
						memcpy(bars.data(), kBlock->_bars, (uint32_t)(flength - BLOCK_HEADER_SIZE));
					}
					else if (kBlock->_version == BLOCK_VERSION_CMP)	//压缩版本
					{
						//现在就需要把数据解压以后读到缓存里去
						HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.data();
						std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
						uint32_t barcnt = rawData.size() / sizeof(WTSBarStruct);
						bars.resize(barcnt);
						memcpy(bars.data(), rawData.data(), rawData.size());
					}

					//开始比较K线时间标签，主要为了防止数据重复写
					if (!bars.empty())
					{
						WTSBarStruct& oldBS = bars.at(bars.size() - 1);	//先取出最后一条K线

						if (oldBS.date == bs.date && memcmp(&oldBS, &bs, sizeof(WTSBarStruct)) != 0)
						{
							//日期相同且数据不同，则用最新的替换最后一条
							oldBS = bs;
						}
						else if (oldBS.date < bs.date)	//老的K线日期小于新的，则直接追加到后面
						{
							bars.push_back(bs);
						}
					}

					//如果老的文件已经是压缩版本，或者最终数据大小大于100条，则进行压缩
					bool bNeedCompress = false;
					if (kBlock->_version == BLOCK_VERSION_CMP || bars.size() > 100)
					{
						bNeedCompress = true;
					}

					if (bNeedCompress)
					{
						std::string cmpData = WTSCmpHelper::compress_data(bars.data(), bars.size() * sizeof(WTSBarStruct));
						BlockHeaderV2 header;
						strcpy(header._blk_flag, BLK_FLAG);
						header._type = BT_HIS_Day;
						header._version = BLOCK_VERSION_CMP;
						header._size = cmpData.size();

						f.truncate_file(0);
						f.seek_to_begin();
						f.write_file(&header, sizeof(header));

						f.write_file(cmpData.data(), cmpData.size());
						count++;
					}
					else
					{
						BlockHeader header;
						strcpy(header._blk_flag, BLK_FLAG);
						header._type = BT_HIS_Day;
						header._version = BLOCK_VERSION_RAW;

						f.truncate_file(0);
						f.seek_to_begin();
						f.write_file(&header, sizeof(header));
						f.write_file(bars.data(), bars.size() * sizeof(WTSBarStruct));
						count++;
					}
				}
			}

			f.close_file();
		}
		else
		{
			_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，day数据收盘作业失败", filename.c_str());
		}
	}

	//转移实时1分钟线
	KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute1, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		_sink->outputWriterLog(LL_INFO, "开始转移合约%s的1分钟数据", ct->getFullCode());
		BoostUniqueLock lock(kBlkPair->_mutex);

		std::stringstream ss;
		ss << _base_dir << "his/min1/" << ct->getExchg() << "/";
		BoostFile::create_directories(ss.str().c_str());
		std::string path = ss.str();
		BoostFile::create_directories(ss.str().c_str());
		std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

		bool bNew = false;
		if (!BoostFile::exists(filename.c_str()))
			bNew = true;

		_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());

		BoostFile f;
		if (f.create_or_open_file(filename.c_str()))
		{
			std::string newData;
			if (!bNew)
			{
				std::string content;
				BoostFile::read_file_contents(filename.c_str(), content);
				HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
				if (kBlock->_version == BLOCK_VERSION_RAW)
				{
					uint32_t barcnt = (content.size() - BLOCK_HEADER_SIZE) / sizeof(WTSBarStruct);
					newData.resize(sizeof(WTSBarStruct)*barcnt);
					memcpy((void*)newData.data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
				}
				else //if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.data();
					newData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				}
			}
			newData.append((const char*)kBlkPair->_block->_bars, sizeof(WTSBarStruct)*size);

			std::string cmpData = WTSCmpHelper::compress_data(newData.data(), newData.size());

			f.truncate_file(0);
			f.seek_to_begin(0);

			BlockHeaderV2 header;
			strcpy(header._blk_flag, BLK_FLAG);
			header._type = BT_HIS_Minute1;
			header._version = BLOCK_VERSION_CMP;
			header._size = cmpData.size();
			f.write_file(&header, sizeof(header));
			f.write_file(cmpData);
			count += size;

			//最后将缓存清空
			//memset(kBlkPair->_block->_bars, 0, sizeof(WTSBarStruct)*kBlkPair->_block->_size);
			kBlkPair->_block->_size = 0;
		}
		else
		{
			_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，min1数据收盘作业失败", filename.c_str());
		}
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	//第四步，转移实时5分钟线
	kBlkPair = getKlineBlock(ct, KP_Minute5, false);
	if (kBlkPair != NULL && kBlkPair->_block->_size > 0)
	{
		uint32_t size = kBlkPair->_block->_size;
		_sink->outputWriterLog(LL_INFO, "开始转移合约%s的5分钟数据", ct->getFullCode());
		BoostUniqueLock lock(kBlkPair->_mutex);

		std::stringstream ss;
		ss << _base_dir << "his/min5/" << ct->getExchg() << "/";
		BoostFile::create_directories(ss.str().c_str());
		std::string path = ss.str();
		BoostFile::create_directories(ss.str().c_str());
		std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), ct->getCode());

		bool bNew = false;
		if (!BoostFile::exists(filename.c_str()))
			bNew = true;

		_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());

		BoostFile f;
		if (f.create_or_open_file(filename.c_str()))
		{
			std::string newData;
			if (!bNew)
			{
				std::string content;
				BoostFile::read_file_contents(filename.c_str(), content);
				HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
				if (kBlock->_version == BLOCK_VERSION_RAW)
				{
					uint32_t barcnt = (content.size() - BLOCK_HEADER_SIZE) / sizeof(WTSBarStruct);
					newData.resize(sizeof(WTSBarStruct)*barcnt);
					memcpy((void*)newData.data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
				}
				else //if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.data();
					newData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				}
			}
			newData.append((const char*)kBlkPair->_block->_bars, sizeof(WTSBarStruct)*size);

			std::string cmpData = WTSCmpHelper::compress_data(newData.data(), newData.size());

			f.truncate_file(0);
			f.seek_to_begin(0);

			BlockHeaderV2 header;
			strcpy(header._blk_flag, BLK_FLAG);
			header._type = BT_HIS_Minute5;
			header._version = BLOCK_VERSION_CMP;
			header._size = cmpData.size();
			f.write_file(&header, sizeof(header));
			f.write_file(cmpData);
			count += size;

			//最后将缓存清空
			kBlkPair->_block->_size = 0;
		}
		else
		{
			_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，min5数据收盘作业失败", filename.c_str());
		}
	}

	if (kBlkPair)
		releaseBlock(kBlkPair);

	return count;
}

void WtDataWriter::proc_loop()
{
	while (!_terminated)
	{
		if(_proc_que.empty())
		{
			BoostUniqueLock lock(_proc_mtx);
			_proc_cond.wait(_proc_mtx);
			continue;
		}

		std::string fullcode;
		try
		{
			BoostUniqueLock lock(_proc_mtx);
			fullcode = _proc_que.front().c_str();
			_proc_que.pop();
		}
		catch(std::exception& e)
		{
			_sink->outputWriterLog(LL_ERROR, e.what());
			continue;
		}

		if (fullcode.compare(CMD_CLEAR_CACHE) == 0)
		{
			//清理缓存
			BoostUniqueLock lock(_mtx_tick_cache);

			std::set<std::string> setCodes;
			std::stringstream ss_snapshot;
			ss_snapshot << "date,exchg,code,open,high,low,close,settle,volumn,turnover,openinterest,upperlimit,lowerlimit,preclose,presettle,preinterest" << std::endl << std::fixed;
			for (auto it = _tick_cache_idx.begin(); it != _tick_cache_idx.end(); it++)
			{
				const std::string& key = it->first;
				const StringVector& ay = StrUtil::split(key, ".");
				WTSContractInfo* ct = _bd_mgr->getContract(ay[1].c_str(), ay[0].c_str());
				if (ct != NULL)
				{
					setCodes.insert(key);

					uint32_t idx = it->second;

					const TickCacheItem& tci = _tick_cache_block->_ticks[idx];
					const WTSTickStruct& ts = tci._tick;
					ss_snapshot << ts.trading_date << ","
						<< ts.exchg << ","
						<< ts.code << ","
						<< ts.open << ","
						<< ts.high << ","
						<< ts.low << ","
						<< ts.price << ","
						<< ts.settle_price << ","
						<< ts.total_volumn << ","
						<< ts.total_turnover << ","
						<< ts.open_interest << ","
						<< ts.upper_limit << ","
						<< ts.lower_limit << ","
						<< ts.pre_close << ","
						<< ts.pre_settle << ","
						<< ts.pre_interest << std::endl;
				}
				else
				{
					_sink->outputWriterLog(LL_WARN, "合约%s[%s]已过期，缓存即将清理", ay[1].c_str(), ay[0].c_str());

					//删除已经过期合约的实时tick文件
					std::string path = StrUtil::printf("%srt/ticks/%s/%s.dmb", _base_dir.c_str(), ay[0].c_str(), ay[1].c_str());
					BoostFile::delete_file(path.c_str());
				}
			}

			//如果两组合约个数不同，说明有合约过期了，被排除了
			if(setCodes.size() != _tick_cache_idx.size())
			{
				uint32_t diff = _tick_cache_idx.size() - setCodes.size();

				uint32_t scale = setCodes.size() / CACHE_SIZE_STEP;
				if (setCodes.size() % CACHE_SIZE_STEP != 0)
					scale++;

				uint32_t size = sizeof(RTTickCache) + sizeof(TickCacheItem)*scale*CACHE_SIZE_STEP;
				std::string buffer;
				buffer.resize(size, 0);

				RTTickCache* newCache = (RTTickCache*)buffer.data();
				newCache->_capacity = scale*CACHE_SIZE_STEP;
				newCache->_type = BT_RT_Cache;
				newCache->_size = setCodes.size();
				newCache->_version = BLOCK_VERSION_RAW;
				strcpy(newCache->_blk_flag, BLK_FLAG);

				std::unordered_map<std::string, uint32_t> newIdxMap;

				uint32_t newIdx = 0;
				for (const std::string& key : setCodes)
				{
					uint32_t oldIdx = _tick_cache_idx[key];
					newIdxMap[key] = newIdx;

					memcpy(&newCache->_ticks[newIdx], &_tick_cache_block->_ticks[oldIdx], sizeof(TickCacheItem));

					newIdx++;
				}

				//索引替换
				_tick_cache_idx = newIdxMap;
				_tick_cache_file->close();
				_tick_cache_block = NULL;

				std::string filename = _base_dir + _cache_file;
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					f.write_file(buffer.data(), buffer.size());
					f.close_file();
				}

				_tick_cache_file->map(filename.c_str());
				_tick_cache_block = (RTTickCache*)_tick_cache_file->addr();
				
				_sink->outputWriterLog(LL_INFO, "行情缓存清理成功，共清理%u条过期合约的缓存", diff);
			}

			std::string path = StrUtil::printf("%srt/min1/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));
			path = StrUtil::printf("%srt/min5/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));
			path = StrUtil::printf("%srt/ticks/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));
			path = StrUtil::printf("%srt/orders/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));
			path = StrUtil::printf("%srt/queue/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));
			path = StrUtil::printf("%srt/trans/", _base_dir.c_str());
			boost::filesystem::remove_all(boost::filesystem::path(path));

			//将当日的日线快照落地到一个快照文件
			{

				std::stringstream ss;
				ss << _base_dir << "his/snapshot/";
				BoostFile::create_directories(ss.str().c_str());
				ss << TimeUtils::getCurDate() << ".csv";
				std::string path = ss.str();

				const std::string& content = ss_snapshot.str();
				BoostFile f;
				f.create_new_file(path.c_str());
				f.write_file(content.data());
				f.close_file();
			}

			continue;
		}
		else if (StrUtil::startsWith(fullcode, "MARK.", false))
		{
			//如果指令以MARK.开头，说明是标记指令，要写一条标记
			std::string filename = _base_dir + MARKER_FILE;
			std::string sid = fullcode.substr(5);
			uint32_t curDate = TimeUtils::getCurDate();
			//IniFile::WriteConfigInt("markers", sid.c_str(), curDate, filename.c_str());
			IniHelper iniHelper;
			iniHelper.load(filename.c_str());
			iniHelper.writeInt("markers", sid.c_str(), curDate);
			iniHelper.save();
			_sink->outputWriterLog(LL_INFO, "交易时间模板[%s]收盘作业标记已更新：%u", sid.c_str(), curDate);
		}

		auto pos = fullcode.find(".");
		std::string exchg = fullcode.substr(0, pos);
		std::string code = fullcode.substr(pos + 1);
		WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if(ct == NULL)
			continue;

		uint32_t count = 0;

		uint32_t uDate = _sink->getTradingDate(ct->getFullCode());
		//转移实时tick数据
		{
			TickBlockPair *tBlkPair = getTickBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "开始转移合约%s的tick数据", fullcode.c_str());
				BoostUniqueLock lock(tBlkPair->_mutex);

				std::stringstream ss;
				ss << _base_dir << "his/ticks/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
				std::string path = ss.str();
				_sink->outputWriterLog(LL_INFO, path.c_str());
				BoostFile::create_directories(ss.str().c_str());
				std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

				bool bNew = false;
				if (!BoostFile::exists(filename.c_str()))
					bNew = true;

				_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//先压缩数据
					std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_ticks, sizeof(WTSTickStruct)*tBlkPair->_block->_size);

					BlockHeaderV2 header;
					strcpy(header._blk_flag, BLK_FLAG);
					header._type = BT_HIS_Ticks;
					header._version = BLOCK_VERSION_CMP;
					header._size = cmp_data.size();
					f.write_file(&header, sizeof(header));

					f.write_file(cmp_data.c_str(), cmp_data.size());
					f.close_file();

					count += tBlkPair->_block->_size;

					//最后将缓存清空
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，tick数据收盘作业失败", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<TickBlockPair>(tBlkPair);
		}

		//转移实时trans数据
		{
			TransBlockPair *tBlkPair = getTransBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "开始转移%s的trans数据", fullcode.c_str());
				BoostUniqueLock lock(tBlkPair->_mutex);

				std::stringstream ss;
				ss << _base_dir << "his/trans/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
				std::string path = ss.str();
				_sink->outputWriterLog(LL_INFO, path.c_str());
				BoostFile::create_directories(ss.str().c_str());
				std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

				bool bNew = false;
				if (!BoostFile::exists(filename.c_str()))
					bNew = true;

				_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//先压缩数据
					std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_trans, sizeof(WTSTransStruct)*tBlkPair->_block->_size);

					BlockHeaderV2 header;
					strcpy(header._blk_flag, BLK_FLAG);
					header._type = BT_HIS_Trnsctn;
					header._version = BLOCK_VERSION_CMP;
					header._size = cmp_data.size();
					f.write_file(&header, sizeof(header));

					f.write_file(cmp_data.c_str(), cmp_data.size());
					f.close_file();

					count += tBlkPair->_block->_size;

					//最后将缓存清空
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，trans数据收盘作业失败", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<TransBlockPair>(tBlkPair);
		}

		//转移实时order数据
		{
			OrdDtlBlockPair *tBlkPair = getOrdDtlBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "开始转移%s的order数据", fullcode.c_str());
				BoostUniqueLock lock(tBlkPair->_mutex);

				std::stringstream ss;
				ss << _base_dir << "his/orders/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
				std::string path = ss.str();
				_sink->outputWriterLog(LL_INFO, path.c_str());
				BoostFile::create_directories(ss.str().c_str());
				std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

				bool bNew = false;
				if (!BoostFile::exists(filename.c_str()))
					bNew = true;

				_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//先压缩数据
					std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_details, sizeof(WTSOrdDtlStruct)*tBlkPair->_block->_size);

					BlockHeaderV2 header;
					strcpy(header._blk_flag, BLK_FLAG);
					header._type = BT_HIS_OrdDetail;
					header._version = BLOCK_VERSION_CMP;
					header._size = cmp_data.size();
					f.write_file(&header, sizeof(header));

					f.write_file(cmp_data.c_str(), cmp_data.size());
					f.close_file();

					count += tBlkPair->_block->_size;

					//最后将缓存清空
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，orders数据收盘作业失败", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<OrdDtlBlockPair>(tBlkPair);
		}

		//转移实时queue数据
		{
			OrdQueBlockPair *tBlkPair = getOrdQueBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "开始转移%s的queue数据", fullcode.c_str());
				BoostUniqueLock lock(tBlkPair->_mutex);

				std::stringstream ss;
				ss << _base_dir << "his/queue/" << ct->getExchg() << "/" << tBlkPair->_block->_date << "/";
				std::string path = ss.str();
				_sink->outputWriterLog(LL_INFO, path.c_str());
				BoostFile::create_directories(ss.str().c_str());
				std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

				bool bNew = false;
				if (!BoostFile::exists(filename.c_str()))
					bNew = true;

				_sink->outputWriterLog(LL_INFO, "打开数据存储块: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//先压缩数据
					std::string cmp_data = WTSCmpHelper::compress_data(tBlkPair->_block->_queues, sizeof(WTSOrdQueStruct)*tBlkPair->_block->_size);

					BlockHeaderV2 header;
					strcpy(header._blk_flag, BLK_FLAG);
					header._type = BT_HIS_OrdQueue;
					header._version = BLOCK_VERSION_CMP;
					header._size = cmp_data.size();
					f.write_file(&header, sizeof(header));

					f.write_file(cmp_data.c_str(), cmp_data.size());
					f.close_file();

					count += tBlkPair->_block->_size;

					//最后将缓存清空
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "历史数据文件%s打开失败，queue数据收盘作业失败", filename.c_str());
				}


			}

			if (tBlkPair)
				releaseBlock<OrdQueBlockPair>(tBlkPair);
		}

		//转移历史K线
		if(_db_conn)
			count += dump_hisdata_to_db(ct);
		else
			count += dump_hisdata_to_file(ct);

		_sink->outputWriterLog(LL_INFO, "合约%s[%s]已完成收盘, 本次作业共处理数据%u条", ct->getCode(), ct->getExchg(), count);
	}
}