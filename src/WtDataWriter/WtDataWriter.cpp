#include "WtDataWriter.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IniHelper.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../WTSTools/WTSCmpHelper.hpp"


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

	_sink->outputWriterLog(LL_INFO, "��ʼԤ����ʵʱ���ݻ����ļ�����");
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
	_sink->outputWriterLog(LL_INFO, "Ԥ����%��Ʒ�ֵ�ʵʱ���ݻ����ļ�%u������ʱ%s΢��", codecnt, cnt, WTSLogger::fmtInt64(ticker.micro_seconds()));
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
	//����û�������
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

	//���øú���֮ǰ��Ӧ���Ѿ�������д����
	RTBlockHeader* tBlock = (RTBlockHeader*)mfPtr->addr();
	if (tBlock->_capacity >= nCount)
		return mfPtr->addr();

	const char* filename = mfPtr->filename();
	uint64_t uSize = sizeof(HeaderType) + sizeof(T)*nCount;
	try
	{
		std::filebuf fbuf;
		fbuf.open(filename, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		//����û�������
		fbuf.pubseekoff(uSize - 1, std::ios_base::beg);
		fbuf.sputc(0);
		fbuf.close();
	}
	catch(std::exception& ex)
	{
		_sink->outputWriterLog(LL_ERROR, "%s[%u]��ʵʱ�����ļ����ݳ����쳣: %s", ex.what());
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
	//����û�������
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

			//�ٸ���״̬����
			if (!_sink->canSessionReceive(commInfo->getSession()))
				break;

			//�ȸ��»���
			if (!updateCache(ct, curTick, bNeedSlice))
				break;

			//д��tick����
			pipeToTicks(ct, curTick);

			//д��K�߻���
			pipeToKlines(ct, curTick);

			_sink->broadcastTick(curTick);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTick->exchg()]++;
			if (_tcnt_map[curTick->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "���յ�������%s��tick����%I64d��", curTick->exchg(), _tcnt_map[curTick->exchg()]);
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

			//�ٸ���״̬����
			if (_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdQueBlockPair* pBlockPair = getOrdQueBlock(ct, curOrdQue->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//�ȼ������������������Ҫ��
			RTOrdQueBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdQueBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdQueStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_queues[blk->_size], &curOrdQue->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdQue->exchg()]++;
			if (_tcnt_map[curOrdQue->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "���յ�������%s��ί�ж�������%I64d��", curOrdQue->exchg(), _tcnt_map[curOrdQue->exchg()]);
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

				{
					BoostUniqueLock lck(_task_mtx);
					TaskInfo& curTask = _tasks.front();
					curTask();
					_tasks.pop();
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

			//�ٸ���״̬����
			if (_sink->canSessionReceive(commInfo->getSession()))
				break;

			OrdDtlBlockPair* pBlockPair = getOrdDtlBlock(ct, curOrdDtl->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//�ȼ������������������Ҫ��
			RTOrdDtlBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTOrdDtlBlock*)resizeRTBlock<RTDayBlockHeader, WTSOrdDtlStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_details[blk->_size], &curOrdDtl->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curOrdDtl->exchg()]++;
			if (_tcnt_map[curOrdDtl->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "���յ�������%s�����ί������%I64d��", curOrdDtl->exchg(), _tcnt_map[curOrdDtl->exchg()]);
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

			//�ٸ���״̬����
			if (_sink->canSessionReceive(commInfo->getSession()))
				break;

			TransBlockPair* pBlockPair = getTransBlock(ct, curTrans->tradingdate());
			if (pBlockPair == NULL)
				break;

			BoostUniqueLock lock(pBlockPair->_mutex);

			//�ȼ������������������Ҫ��
			RTTransBlock* blk = pBlockPair->_block;
			if (blk->_size >= blk->_capacity)
			{
				pBlockPair->_file->sync();
				pBlockPair->_block = (RTTransBlock*)resizeRTBlock<RTDayBlockHeader, WTSTransStruct>(pBlockPair->_file, blk->_capacity + TICK_SIZE_STEP);
				blk = pBlockPair->_block;
			}

			memcpy(&blk->_trans[blk->_size], &curTrans->getTransStruct(), sizeof(WTSTransStruct));
			blk->_size += 1;

			//TODO: Ҫ�㲥��
			//g_udpCaster.broadcast(curTrans);

			static std::unordered_map<std::string, uint64_t> _tcnt_map;
			_tcnt_map[curTrans->exchg()]++;
			if (_tcnt_map[curTrans->exchg()] % _log_group_size == 0)
			{
				_sink->outputWriterLog(LL_INFO, "���յ�������%s����ʳɽ�����%I64d��", curTrans->exchg(), _tcnt_map[curTrans->exchg()]);
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

	//�ȼ������������������Ҫ��
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

			_sink->outputWriterLog(LL_INFO, "�����ļ�%s�����ڣ����ڳ�ʼ������", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s��ʼӳ��", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "�ļ�%sӳ��ʧ��", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdQueBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%sӳ��ɹ�", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orderqueue����ģ��%s����%u�����ڵ�ǰ����%u�����³�ʼ��", path.c_str(), pBlock->_block->_date, curDate);
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
			//��黺���ļ��Ƿ������⣬Ҫ�Զ��ָ�
			do
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdQueStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdQueStruct));
					//�ļ���С��ƥ�䣬һ������Ϊcapacity���ˣ�����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s��Ʊ%u��ί�ж��л����ļ����޸�", ct->getCode(), curDate);
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

			_sink->outputWriterLog(LL_INFO, "�����ļ�%s�����ڣ����ڳ�ʼ������", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s��ʼӳ��", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "�ļ�%sӳ��ʧ��", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTOrdDtlBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%sӳ��ɹ�", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "orders����ģ��%s����%u�����ڵ�ǰ����%u�����³�ʼ��", path.c_str(), pBlock->_block->_date, curDate);
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
			//��黺���ļ��Ƿ������⣬Ҫ�Զ��ָ�
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSOrdDtlStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSOrdDtlStruct));
					//�ļ���С��ƥ�䣬һ������Ϊcapacity���ˣ�����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s��Ʊ%u����ʳɽ������ļ����޸�", ct->getCode(), curDate);
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

			_sink->outputWriterLog(LL_INFO, "�����ļ�%s�����ڣ����ڳ�ʼ������", path.c_str());

			uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * TICK_SIZE_STEP;

			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s��ʼӳ��", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if (!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "�ļ�%sӳ��ʧ��", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTransBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%sӳ��ɹ�", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "trans����ģ��%s����%u�����ڵ�ǰ����%u�����³�ʼ��", path.c_str(), pBlock->_block->_date, curDate);
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
			//��黺���ļ��Ƿ������⣬Ҫ�Զ��ָ�
			for (;;)
			{
				uint64_t uSize = sizeof(RTDayBlockHeader) + sizeof(WTSTransStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTDayBlockHeader)) / sizeof(WTSTransStruct));
					//�ļ���С��ƥ�䣬һ������Ϊcapacity���ˣ�����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s��Ʊ%u����ʳɽ������ļ����޸�", ct->getCode(), curDate);
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

			_sink->outputWriterLog(LL_INFO, "�����ļ�%s�����ڣ����ڳ�ʼ������", path.c_str());

			uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * TICK_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(path.c_str());
			bf.truncate_file((uint32_t)uSize);
			bf.close_file();

			isNew = true;
		}

		//_sink->outputWriterLog(LL_INFO, "%s��ʼӳ��", path.c_str());
		pBlock->_file.reset(new BoostMappingFile);
		if(!pBlock->_file->map(path.c_str()))
		{
			_sink->outputWriterLog(LL_INFO, "�ļ�%sӳ��ʧ��", path.c_str());
			pBlock->_file.reset();
			return NULL;
		}
		pBlock->_block = (RTTickBlock*)pBlock->_file->addr();

		//_sink->outputWriterLog(LL_INFO, "%sӳ��ɹ�", path.c_str());

		if (!isNew &&  pBlock->_block->_date != curDate)
		{
			_sink->outputWriterLog(LL_INFO, "tick����ģ��%s����%u�����ڵ�ǰ����%u�����³�ʼ��", path.c_str(), pBlock->_block->_date, curDate);
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
			//��黺���ļ��Ƿ������⣬Ҫ�Զ��ָ�
			for (;;)
			{
				uint64_t uSize = sizeof(RTTickBlock) + sizeof(WTSTickStruct) * pBlock->_block->_capacity;
				uint64_t oldSize = pBlock->_file->size();
				if (oldSize != uSize)
				{
					uint32_t oldCnt = (uint32_t)((oldSize - sizeof(RTTickBlock)) / sizeof(WTSTickStruct));
					//�ļ���С��ƥ�䣬һ������Ϊcapacity���ˣ�����ʵ��û����
					//������һ�����ݼ���
					pBlock->_block->_capacity = oldCnt;
					pBlock->_block->_size = oldCnt;

					_sink->outputWriterLog(LL_WARN, "%s��Լ%u��tick�����ļ����޸�", ct->getCode(), curDate);
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

	//������Ϊ0��Ҫר�Ŵ�������091500000�����tickҪ����0915��
	//�����С�ڽ�����Ҫ����С�ڽ�����һ���ӣ���Ϊ�������г�������ʱ��ļ۸��������113000500
	//����ͬʱ����������or	
	if (sInfo->isLastOfSection(curTime))
	{
		minutes--;
	}

	//����1������
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

		//ƴ��1������
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

			_sink->outputWriterLog(LL_INFO, "�����ļ�%s�����ڣ����ڳ�ʼ������", path.c_str());

			uint64_t uSize = sizeof(RTKlineBlock) + sizeof(WTSBarStruct) * KLINE_SIZE_STEP;
			/*
			std::filebuf fbuf;
			fbuf.open(path, std::ios_base::in | std::ios_base::out
				| std::ios_base::trunc | std::ios_base::binary);

			//��λ�������ڶ����ַ���Ȼ��д��һ��0�������ļ���С�͹̶�Ϊָ����С��
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
			_sink->outputWriterLog(LL_INFO, "�����ļ�%sӳ��ʧ��", path.c_str());
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
		_sink->outputWriterLog(LL_ERROR, "tick���ݻ�����δ��ʼ��");
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
			_sink->outputWriterLog(LL_INFO, "%s�������ع�Ϊ%u��С", key.c_str(), _tick_cache_block->_capacity);
		}
	}
	else
	{
		idx = _tick_cache_idx[key];
	}


	TickCacheItem& item = _tick_cache_block->_ticks[idx];
	if (curTick->tradingdate() < item._date)
	{
		_sink->outputWriterLog(LL_INFO, "%s��������%uС�ڻ��潻����%u", curTick->tradingdate(), item._date);
		return false;
	}

	WTSTickStruct& newTick = curTick->getTickStruct();

	if (curTick->tradingdate() > item._date)
	{
		//�����ݽ����մ��������ݣ�����Ϊ����һ�������
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

		_sink->outputWriterLog(LL_INFO, "�½�����%u�ĵ�һ������,%s.%s,%u,%f,%d,%d", newTick.trading_date, curTick->exchg(), curTick->code(), curTick->volumn(), curTick->turnover(), curTick->openinterest(), curTick->additional());
	}
	else
	{
		//�����������������ڴ����������������
		//���߻������ʱ����ڵ������������ʱ�䣬���ݾͲ���Ҫ����
		WTSSessionInfo* sInfo = _bd_mgr->getSessionByCode(curTick->code(), curTick->exchg());
		uint32_t tdate = sInfo->getOffsetDate(curTick->actiondate(), curTick->actiontime() / 100000);
		if (tdate > curTick->tradingdate())
		{
			_sink->outputWriterLog(LL_ERROR, "��Լ%s.%s����tick����(ʱ��%u.%u)�쳣������", curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime());
			return false;
		}
		else if (curTick->totalvolumn() < item._tick.total_volumn)
		{
			_sink->outputWriterLog(LL_ERROR, "��Լ%s.%s����tick����(ʱ��%u.%u���ܳɽ�%uС�ڻ����ܳɽ�%u)�쳣������", 
				curTick->exchg(), curTick->code(), curTick->actiondate(), curTick->actiontime(), curTick->totalvolumn(), item._tick.total_volumn);
			return false;
		}

		//ʱ�����ͬ�����ǳɽ������ڵ���ԭ���ģ��������һ����֣����������Ĵ���ʽ����ʱ���+500����
		if(newTick.action_date == item._tick.action_date && newTick.action_time == item._tick.action_time && newTick.total_volumn >= item._tick.volumn)
		{
			newTick.action_time += 500;
		}

		//�����Ҫ���費��ҪԤ������
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
				_sink->outputWriterLog(LL_INFO, "tick���� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<TickBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_trans_blocks.begin(); it != _rt_trans_blocks.end(); it++)
		{
			const std::string& key = it->first;
			TransBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "trans���� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<TransBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_orddtl_blocks.begin(); it != _rt_orddtl_blocks.end(); it++)
		{
			const std::string& key = it->first;
			OrdDtlBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "order���� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<OrdDtlBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_ordque_blocks.begin(); it != _rt_ordque_blocks.end(); it++)
		{
			const std::string& key = it->first;
			OrdQueBlockPair* tBlk = &it->second;
			if (tBlk->_lasttime != 0 && (now - tBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "queue���� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<OrdQueBlockPair>(tBlk);
			}
		}

		for (auto it = _rt_min1_blocks.begin(); it != _rt_min1_blocks.end(); it++)
		{
			const std::string& key = it->first;
			KBlockPair* kBlk = &it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "1���ӻ��� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<KBlockPair>(kBlk);
			}
		}

		for (auto it = _rt_min5_blocks.begin(); it != _rt_min5_blocks.end(); it++)
		{
			const std::string& key = it->first;
			KBlockPair* kBlk = &it->second;
			if (kBlk->_lasttime != 0 && (now - kBlk->_lasttime > expire_secs))
			{
				_sink->outputWriterLog(LL_INFO, "5���ӻ��� %s ӳ�䳬ʱ���Զ��ر�", key.c_str());
				releaseBlock<KBlockPair>(kBlk);
			}
		}
	}
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
			//������
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
					_sink->outputWriterLog(LL_WARN, "��Լ%s[%s]�ѹ��ڣ����漴������", ay[1].c_str(), ay[0].c_str());

					//ɾ���Ѿ����ں�Լ��ʵʱtick�ļ�
					std::string path = StrUtil::printf("%srt/ticks/%s/%s.dmb", _base_dir.c_str(), ay[0].c_str(), ay[1].c_str());
					BoostFile::delete_file(path.c_str());
				}
			}

			//��������Լ������ͬ��˵���к�Լ�����ˣ����ų���
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

				//�����滻
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
				
				_sink->outputWriterLog(LL_INFO, "���黺������ɹ���������%u�����ں�Լ�Ļ���", diff);
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

			//�����յ����߿�����ص�һ�������ļ�
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
			//���ָ����MARK.��ͷ��˵���Ǳ��ָ�Ҫдһ�����
			std::string filename = _base_dir + MARKER_FILE;
			std::string sid = fullcode.substr(5);
			uint32_t curDate = TimeUtils::getCurDate();
			//IniFile::WriteConfigInt("markers", sid.c_str(), curDate, filename.c_str());
			IniHelper iniHelper;
			iniHelper.load(filename.c_str());
			iniHelper.writeInt("markers", sid.c_str(), curDate);
			iniHelper.save();
			_sink->outputWriterLog(LL_INFO, "����ʱ��ģ��[%s]������ҵ����Ѹ��£�%u", sid.c_str(), curDate);
		}

		auto pos = fullcode.find(".");
		std::string exchg = fullcode.substr(0, pos);
		std::string code = fullcode.substr(pos + 1);
		WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if(ct == NULL)
			continue;

		std::string key = StrUtil::printf("%s.%s", ct->getExchg(), ct->getCode());

		uint32_t count = 0;

		uint32_t uCurDate = 0;
		//��һ�����ӻ����ж�ȡ����tick�����µ���ʷ����
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

			uCurDate = ts.trading_date;

			std::stringstream ss;
			ss << _base_dir << "his/day/" << ct->getExchg() << "/";
			std::string path = ss.str();
			BoostFile::create_directories(ss.str().c_str());
			std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

			bool bNew = false;
			if (!BoostFile::exists(filename.c_str()))
				bNew = true;

			//_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());

			BoostFile f;
			if(f.create_or_open_file(filename.c_str()))
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
					//���߱���Ҫ���һ��
					std::string content;
					BoostFile::read_file_contents(filename.c_str(), content);
					uint64_t flength = content.size();
					
					HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
					if (strcmp(kBlock->_blk_flag, BLK_FLAG) != 0)
					{
						_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%sͷ���쳣������������ؽ�", filename.c_str());
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
						if (kBlock->_version == BLOCK_VERSION_RAW)	//����ϵ��ļ��Ƿ�ѹ���汾����ֱ�Ӱ�K�߶���������ȥ
						{
							uint32_t barcnt = (uint32_t)(flength - BLOCK_HEADER_SIZE) / sizeof(WTSBarStruct);
							bars.resize(barcnt);
							memcpy(bars.data(), kBlock->_bars, (uint32_t)(flength - BLOCK_HEADER_SIZE));
						}
						else if (kBlock->_version == BLOCK_VERSION_CMP)	//ѹ���汾
						{
							//���ھ���Ҫ�����ݽ�ѹ�Ժ����������ȥ
							HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.data();
							std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
							uint32_t barcnt = rawData.size() / sizeof(WTSBarStruct);
							bars.resize(barcnt);
							memcpy(bars.data(), rawData.data(), rawData.size());
						}

						//��ʼ�Ƚ�K��ʱ���ǩ����ҪΪ�˷�ֹ�����ظ�д
						if (!bars.empty())
						{
							WTSBarStruct& oldBS = bars.at(bars.size() - 1);	//��ȡ�����һ��K��

							if (oldBS.date == bs.date && memcmp(&oldBS, &bs, sizeof(WTSBarStruct)) != 0)
							{
								//������ͬ�����ݲ�ͬ���������µ��滻���һ��
								oldBS = bs;
							}
							else if (oldBS.date < bs.date)	//�ϵ�K������С���µģ���ֱ��׷�ӵ�����
							{
								bars.push_back(bs);
							}
						}

						//����ϵ��ļ��Ѿ���ѹ���汾�������������ݴ�С����100���������ѹ��
						bool bNeedCompress = false;
						if (kBlock->_version == BLOCK_VERSION_CMP || bars.size() > 100)
						{
							bNeedCompress = true;
						}

						if (bNeedCompress)
						{
							std::string cmpData = WTSCmpHelper::compress_data(bars.data(), bars.size()*sizeof(WTSBarStruct));
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
							f.write_file(bars.data(), bars.size()*sizeof(WTSBarStruct));
							count++;
						}
					}
				}
				
				f.close_file();
			}
			else
			{
				_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�day����������ҵʧ��", filename.c_str());
			}
		}

		uint32_t uDate = _sink->getTradingDate(key.c_str());
		//�ڶ�����ת��ʵʱtick����
		{
			TickBlockPair *tBlkPair = getTickBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "��ʼת�ƺ�Լ%s��tick����", fullcode.c_str());
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

				_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//��ѹ������
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

					//��󽫻������
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�tick����������ҵʧ��", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<TickBlockPair>(tBlkPair);
		}

		//��������ת��ʵʱtrans����
		{
			TransBlockPair *tBlkPair = getTransBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "��ʼת��%s��trans����", fullcode.c_str());
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

				_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//��ѹ������
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

					//��󽫻������
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�trans����������ҵʧ��", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<TransBlockPair>(tBlkPair);
		}

		//���Ĳ���ת��ʵʱorder����
		{
			OrdDtlBlockPair *tBlkPair = getOrdDtlBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "��ʼת��%s��order����", fullcode.c_str());
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

				_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//��ѹ������
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

					//��󽫻������
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�orders����������ҵʧ��", filename.c_str());
				}
			}

			if (tBlkPair)
				releaseBlock<OrdDtlBlockPair>(tBlkPair);
		}

		//���岽��ת��ʵʱqueue����
		{
			OrdQueBlockPair *tBlkPair = getOrdQueBlock(ct, uDate, false);
			if (tBlkPair != NULL && tBlkPair->_block->_size > 0)
			{
				_sink->outputWriterLog(LL_INFO, "��ʼת��%s��queue����", fullcode.c_str());
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

				_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());
				BoostFile f;
				if (f.create_new_file(filename.c_str()))
				{
					//��ѹ������
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

					//��󽫻������
					//memset(tBlkPair->_block->_ticks, 0, sizeof(WTSTickStruct)*tBlkPair->_block->_size);
					tBlkPair->_block->_size = 0;
				}
				else
				{
					_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�queue����������ҵʧ��", filename.c_str());
				}


			}

			if (tBlkPair)
				releaseBlock<OrdQueBlockPair>(tBlkPair);
		}

		//��������ת��ʵʱ1������
		KBlockPair* kBlkPair = getKlineBlock(ct, KP_Minute1, false);
		if (kBlkPair != NULL  && kBlkPair->_block->_size>0)
		{
			uint32_t size = kBlkPair->_block->_size;
			_sink->outputWriterLog(LL_INFO, "��ʼת�ƺ�Լ%s��1��������", fullcode.c_str());
			BoostUniqueLock lock(kBlkPair->_mutex);

			std::stringstream ss;
			ss << _base_dir << "his/min1/" << ct->getExchg() << "/";
			BoostFile::create_directories(ss.str().c_str());
			std::string path = ss.str();
			BoostFile::create_directories(ss.str().c_str());
			std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

			bool bNew = false;
			if (!BoostFile::exists(filename.c_str()))
				bNew = true;

			_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());

			BoostFile f;
			if (f.create_or_open_file(filename.c_str()))
			{
				std::string newData;
				if(!bNew)
				{
					std::string content;
					BoostFile::read_file_contents(filename.c_str(), content);
					HisKlineBlock* kBlock = (HisKlineBlock*)content.data();
					if(kBlock->_version == BLOCK_VERSION_RAW)
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

				//��󽫻������
				//memset(kBlkPair->_block->_bars, 0, sizeof(WTSBarStruct)*kBlkPair->_block->_size);
				kBlkPair->_block->_size = 0;
			}
			else
			{
				_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�min1����������ҵʧ��", filename.c_str());
			}
		}

		if (kBlkPair)
			releaseBlock(kBlkPair);

		//���Ĳ���ת��ʵʱ5������
		kBlkPair = getKlineBlock(ct, KP_Minute5, false);
		if (kBlkPair != NULL  && kBlkPair->_block->_size>0)
		{
			uint32_t size = kBlkPair->_block->_size;
			_sink->outputWriterLog(LL_INFO, "��ʼת�ƺ�Լ%s��5��������", fullcode.c_str());
			BoostUniqueLock lock(kBlkPair->_mutex);

			std::stringstream ss;
			ss << _base_dir << "his/min5/" << ct->getExchg() << "/";
			BoostFile::create_directories(ss.str().c_str());
			std::string path = ss.str();
			BoostFile::create_directories(ss.str().c_str());
			std::string filename = StrUtil::printf("%s%s.dsb", path.c_str(), code.c_str());

			bool bNew = false;
			if (!BoostFile::exists(filename.c_str()))
				bNew = true;

			_sink->outputWriterLog(LL_INFO, "�����ݴ洢��: %s", filename.c_str());

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

				//��󽫻������
				kBlkPair->_block->_size = 0;
			}
			else
			{
				_sink->outputWriterLog(LL_ERROR, "��ʷ�����ļ�%s��ʧ�ܣ�min5����������ҵʧ��", filename.c_str());
			}
		}

		if (kBlkPair)
			releaseBlock(kBlkPair);

		_sink->outputWriterLog(LL_INFO, "��Լ%s[%s]���������, ������ҵ����������%u��", ct->getCode(), ct->getExchg(), count);
	}
}