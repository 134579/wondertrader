#include "WtSimpExeUnit.h"

#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"

extern const char* FACT_NAME;


WtSimpExeUnit::WtSimpExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _use_opposite(false)
	, _price_offset(0)
	, _expire_secs(0)
	, _cancel_cnt(0)
	, _target_pos(0)
{
}


WtSimpExeUnit::~WtSimpExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtSimpExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtSimpExeUnit::getName()
{
	return "WtSimpExeUnit";
}

void WtSimpExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_price_offset = cfg->getInt32("offset");
	_expire_secs = cfg->getUInt32("expire");
	_use_opposite = cfg->getBoolean("opposite");

	ctx->writeLog("ִ�е�Ԫ %s ��ʼ����ɣ�ί�м� %s �� %d ����������ʱ %u ��", stdCode, _use_opposite?"���ּ�":"���¼�", _price_offset, _expire_secs);
}

void WtSimpExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t leftover, double price, bool isCanceled)
{
	{
		std::unique_lock<std::mutex> lck(_mtx_ords);
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		if (isCanceled || leftover == 0)
		{
			_orders.erase(it);
			if (_cancel_cnt > 0)
				_cancel_cnt--;

			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		}
	}

	//����г�����Ҳ�������¼���
	if (isCanceled)
	{
		_ctx->writeLog("%s�Ķ���%u�ѳ��������´���ִ���߼�", stdCode, localid);
		doCalculate();
	}
}

void WtSimpExeUnit::on_channel_ready()
{
	int32_t undone = _ctx->getUndoneQty(_code.c_str());
	if(undone != 0 && _orders.empty())
	{
		//��˵����δ��ɵ����ڼ��֮�У��ȳ���
		_ctx->writeLog("%s�в��ڹ����е�δ��ɵ� %d �֣�ȫ������", _code.c_str(), undone);

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		for(auto localid : ids)
		{
			_orders.insert(localid);
		}
		_cancel_cnt += ids.size();

		_ctx->writeLog("%s cancelcnt -> %u", __FUNCTION__, _cancel_cnt);
	}


	doCalculate();
}

void WtSimpExeUnit::on_channel_lost()
{
	
}

void WtSimpExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	bool isFirstTick = false;
	//���ԭ����tick��Ϊ�գ���Ҫ�ͷŵ�
	if (_last_tick)
		_last_tick->release();
	else
		isFirstTick = true;

	//�µ�tick���ݣ�Ҫ����
	_last_tick = newTick;
	_last_tick->retain();

	/*
	 *	������Կ���һ��
	 *	���д����һ�ζ���ȥ�ĵ��Ӳ����ﵽĿ���λ
	 *	��ô���µ��������ݽ�����ʱ������ٴδ��������߼�
	 */

	if(isFirstTick)	//����ǵ�һ��tick������Ŀ���λ�����������µ�
	{
		int32_t newVol = _target_pos;
		const char* stdCode = _code.c_str();

		int32_t undone = _ctx->getUndoneQty(stdCode);
		int32_t realPos = _ctx->getPosition(stdCode);

		if (newVol != undone + realPos)
		{
			doCalculate();
		}
	}
	else if(_expire_secs != 0 && !_orders.empty())
	{
		uint64_t now = _ctx->getCurTime();
		if (now - _last_entry_time < _expire_secs * 1000 || _cancel_cnt!=0)
			return;

		_mtx_ords.lock();
		for (auto localid : _orders)
		{
			if(_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			}
		}
		_mtx_ords.unlock();
	}
}

void WtSimpExeUnit::on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price)
{
	//���ô�����������ontick�ﴥ����
	//_ctx->writeLog("%s��Լ%s%u�֣����´���ִ���߼�", stdCode, isBuy?"����":"����", vol);
	//doCalculate();
}

void WtSimpExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//��������ҷ���ȥ�Ķ������ҾͲ�����
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		_orders.erase(it);

		doCalculate();
	}
}

void WtSimpExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;

	int32_t newVol = _target_pos;
	const char* stdCode = _code.c_str();

	int32_t undone = _ctx->getUndoneQty(stdCode);
	int32_t realPos = _ctx->getPosition(stdCode);

	//����з���δ��ɵ�����ֱ�ӳ���
	//���Ŀ���λΪ0���ҵ�ǰ�ֲ�Ϊ0������ȫ���ҵ�
	if (newVol * undone < 0 )
	{
		bool isBuy = (undone > 0);
		//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_cancel_cnt += ids.size();
		_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		return;
	}
	else if(newVol == 0 && undone != 0)
	{
		//���Ŀ���λΪ0����δ��ɲ�Ϊ0
		//��ô��Ŀǰ��λΪ0������ Ŀǰ��λ��δ�������������ͬ
		//����ҲҪȫ������
		if (realPos == 0 || (realPos * undone > 0))
		{
			bool isBuy = (undone > 0);
			//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
			OrderIDs ids = _ctx->cancel(stdCode, isBuy);
			for (auto localid : ids)
			{
				_orders.insert(localid);
			}
			_cancel_cnt += ids.size();
			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			return;
		}
	}

	//�������ͬ��ģ����������
	int32_t curPos = realPos + undone;
	if (curPos == newVol)
		return;

	if(_last_tick == NULL)
	{
		//grabLastTick���Զ��������ü���������Ҫ��retain
		_last_tick = _ctx->grabLastTick(stdCode);
	}

	if (_last_tick == NULL)
	{
		_ctx->writeLog("%sû������tick���ݣ��˳�ִ���߼�", _code.c_str());
		return;
	}

	/*
	*	����һ���򵥵�ִ��ģ��
	*	һ���Լ�����ǵ�ͣ�۷���
	*/

	double buyPx = _last_tick->price() + _comm_info->getPriceTick() * _price_offset;
	double sellPx = _last_tick->price() - _comm_info->getPriceTick() * _price_offset;

	if (newVol > curPos)
	{
		OrderIDs ids = _ctx->buy(stdCode, buyPx, newVol - curPos);

		_mtx_ords.lock();
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_mtx_ords.unlock();
		_last_entry_time = _ctx->getCurTime();
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, curPos - newVol);

		_mtx_ords.lock();
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_mtx_ords.unlock();
		_last_entry_time = _ctx->getCurTime();
	}
}

void WtSimpExeUnit::set_position(const char* stdCode, int32_t newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	if (_target_pos != newVol)
	{
		_target_pos = newVol;
	}

	doCalculate();
}
