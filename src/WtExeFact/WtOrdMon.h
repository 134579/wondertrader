#pragma once
#include <unordered_map>
#include <stdint.h>
#include <functional>

#include "../Share/StdUtils.hpp"

typedef std::function<void(uint32_t)> EnumOrderCallback;

/*
 *	����������
 */
class WtOrdMon
{
public:
	/*
	 *	��Ӷ���
	 *	
	 */
	void push_order(const uint32_t* ids, uint32_t cnt, uint64_t curTime);

	void erase_order(uint32_t localid);

	/*
	 *	����Ƿ��ж���
	 *	@localid	�����ţ�Ϊ0ʱ����Ƿ������ⶩ������Ϊ0ʱ����Ƿ���ָ������
	 */
	inline bool has_order(uint32_t localid = 0)
	{
		if (localid == 0)
			return !_orders.empty();

		auto it = _orders.find(localid);
		if (it == _orders.end())
			return false;

		return true;
	}

	void check_orders(uint32_t expiresecs, uint64_t curTime, EnumOrderCallback callback);

private:
	typedef std::unordered_map<uint32_t, uint64_t> IDMap;
	IDMap			_orders;
	StdRecurMutex	_mtx_ords;
};

