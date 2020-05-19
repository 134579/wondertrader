#pragma once

#include "WTSMarcos.h"
#include <stdlib.h>
#include <vector>
#include <atomic>

NS_OTP_BEGIN
template<std::size_t ObjSize, std::size_t BlkCap>
class WTSMemCache
{
public:
	inline bool	enlarge()
	{
		bool bEnlarge = false;
		while (!std::atomic_compare_exchange_weak(&_enlarging, &bEnlarge, true));

		//����б��������󻺴��ˣ���ǰ�߳̾Ͳ�Ҫ���ˣ��ȱ����������˷��ؼ���
		if(bEnlarge)
		{
			printf("some other thread is enlarging the cache, wait until its done...\r\n");
			while (_enlarging.load());
			return true;
		}

		_obj_size = ObjSize;
		_blk_size = ObjSize;
		_blk_size *= BlkCap;
		char* addr = (char*)::malloc(_blk_size);
		if (addr == NULL)
			throw std::exception("malloc memory failed");

		_cache_blocks.resize(_cache_blocks.size() + 1);	//���������һ��
		CacheBlock& items = _cache_blocks[_cache_blocks.size()-1];
		items.resize(BlkCap);	//��ʼ���»���������

		//��������
		for (std::size_t i = 0; i < BlkCap; i++)
		{
			items[i]._data = (addr + i*ObjSize);
			if (i < BlkCap - 1)
				items[i]._next = &items[i + 1];
		}

		//�ڴ��ַ����
		_addrs.push_back(addr);
		
		//���ͷ��ָ��Ϊ�գ���δ��ʼ�����������������
		//�����·���ĸ�ͷָ��
		if (_head.load() == NULL)
		{
			CacheItem* head = NULL;
			while (!std::atomic_compare_exchange_weak(&_head, &head, &items[0]));
		}
		
		//�滻β��ָ�룬���ԭβ��ָ�벻Ϊ�գ���ָ���·�����׵�ַ
		{
			CacheItem* tail = NULL;
			while (!std::atomic_compare_exchange_weak(&_tail, &tail, &items[BlkCap - 1]));

			if (tail)
				tail->_next = &items[0];
		}

		while (!std::atomic_compare_exchange_weak(&_enlarging, &bEnlarge, false));
		return true;
	}

	inline bool is_empty() const
	{
		return !_cache_blocks.empty();
	}

	inline bool is_valid() const
	{
		while (_enlarging.load());
		return _head.load() != NULL;
	}

	inline void* gain_one()
	{
		if (_head.load() == NULL)
			throw std::exception("run out of cache");

		CacheItem* head = NULL;
		while (!std::atomic_compare_exchange_weak(&_head, &head, (*_head)._next));

		head->_next = NULL;
		head->_used = true;

		CacheItem* tail = _tail.load();
		if (tail == head)
		{
			while (!std::atomic_compare_exchange_weak(&_tail, &tail, (CacheItem*)NULL));
		}

		return head->_data;
	}

	inline bool	free_one(void* ptr)
	{
		//�ȶ�λ�ڴ�λ��
		bool bHit = false;
		std::size_t offset = 0;
		std::size_t blk_idx = 0;
		
		auto cnt = _addrs.size();
		for (auto i = 0; i < cnt; i++)
		{
			char* addr = _addrs[i];
			offset = (char*)ptr - addr;
			if (offset >= 0 && offset < _blk_size)
			{
				bHit = true;
				break;
			}

			//������ܱ������С������˵���ͷ������⣬����false
			if (bHit && offset % _obj_size != 0)
				return false;

			blk_idx++;
		}	

		if (!bHit)
			return false;

		//���ݵ�ַƫ��ȷ����������
		std::size_t idx = offset / _obj_size;
		//����������δʹ�õģ������޸���
		CacheItem* item = &_cache_blocks[blk_idx][idx];
		if (!item->_used)
			return false;

		//�ͷŵĻ���Ҫ�ŵ�����β��������nextΪNULL
		//�ͷ��Ժ���δʹ��״̬�����Ա��ҲҪ��
		item->_next = NULL;
		item->_used = false;

		//�Ȱ�����β���޸ĵ�
		CacheItem* tail = NULL;
		while (!std::atomic_compare_exchange_weak(&_tail, &tail, item));
		if (tail)
			tail->_next = item;

		//���ͷ��Ϊ�գ��ٸ�ͷ����ֵ
		if (_head.load() == NULL)
		{
			//_head = item;
			CacheItem* head = NULL;
			while (!std::atomic_compare_exchange_weak(&_head, &head, item));
		}

		_usage.fetch_sub(1);

		return true;
	}

	WTSMemCache() :_head(NULL), _tail(NULL), _usage(0), _max_usage(0), _enlarging(false){}
	~WTSMemCache()
	{
		for(char*& addr : _addrs)
			::free(addr);
	}

private:
	typedef struct _CacheItem
	{
		void*		_data;
		_CacheItem*	_next;
		bool		_used;

		_CacheItem() :_data(NULL), _next(NULL), _used(false){}
	} CacheItem;

	typedef std::vector<CacheItem> CacheBlock;
	std::vector<CacheBlock>	_cache_blocks;	//������б�
	std::atomic<CacheItem*>	_head;		//ͷ��ָ��
	std::atomic<CacheItem*>	_tail;		//β��ָ��
	std::vector<char*>		_addrs;		//�ڴ���б�
	std::size_t				_blk_size;	//���С
	std::size_t				_obj_size;	//�����С
	std::atomic<uint32_t>	_usage;		//��ʹ��
	uint32_t				_max_usage;	//�������
	std::atomic<bool>		_enlarging;
};


namespace allocator
{
	template<std::size_t ObjSize, std::size_t ObjCap>
	static WTSMemCache<ObjSize, ObjCap>& getMemCache(bool bNeedInit = true)
	{
		static WTSMemCache<ObjSize, ObjCap> inst;
		if ((!inst.is_empty() || !inst.is_valid()) && bNeedInit)
		{
			inst.enlarge();
		}

		return inst;
	}

	template<class T>
	static T* allocate()
	{
		auto& cache = getMemCache<sizeof(T), 128>(true);
		T* ret = (T*)cache.gain_one();
		new(ret)T();
		return ret;
	}

	template<class T>
	static void deallocate(T* obj)
	{
		if (obj == NULL)
			return;

		obj->~T();
		auto& cache = getMemCache<sizeof(T), 128>(false);
		cache.free_one(obj);
	}

};
NS_OTP_END