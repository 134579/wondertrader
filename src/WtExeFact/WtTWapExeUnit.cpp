#include "WtTWapExeUnit.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"

#include <math.h>


extern const char* FACT_NAME;


WtTWapExeUnit::WtTWapExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _sticky(0)
	, _cancel_cnt(0)
	, _channel_ready(false)
	, _last_fire_time(0)
	, _exec_begin_time(UINT64_MAX)
	, _target_pos(0)
	, _exeSecondTime(60)
	, _twap_times(10)
{
}


WtTWapExeUnit::~WtTWapExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtTWapExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtTWapExeUnit::getName()
{
	return "WtTWapExeUnit";
}

void WtTWapExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_sticky = cfg->getUInt32("sticky");
	_exec_secs = cfg->getUInt32("execsecs");
	_exec_tail = cfg->getUInt32("tailsecs");

	ctx->writeLog("ִ�е�Ԫ WtTWapExeUnit[%s] ��ʼ����ɣ�������ʱ %u �룬ִ��ʱ�� %u �룬��βʱ�� %u ��", stdCode, _sticky, _exec_secs, _exec_tail);
}

void WtTWapExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	if (isCanceled || leftover == 0)
	{
		StdLocker<StdRecurMutex> lock(_mtx_ords);
		_orders.erase(it);
		if (_cancel_cnt > 0)
			_cancel_cnt--;

		_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
	}

	//����г�����Ҳ�������¼���
	if (isCanceled)
	{
		_ctx->writeLog("%s�Ķ���%u�ѳ��������´���ִ���߼�", stdCode, localid);
		doCalculate();
	}
}

void WtTWapExeUnit::on_channel_ready()
{
	_channel_ready = true;
	double undone = _ctx->getUndoneQty(_code.c_str());
	if (undone != 0 && _orders.empty())
	{
		//��˵����δ��ɵ����ڼ��֮�У��ȳ���
		_ctx->writeLog("%s�в��ڹ����е�δ��ɵ� %.0f �֣�ȫ������", _code.c_str(), undone);

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		for (auto localid : ids)
		{
			_orders[localid] = _ctx->getCurTime();
		}
		_cancel_cnt += ids.size();

		_ctx->writeLog("%s cancelcnt -> %u", __FUNCTION__, _cancel_cnt);
	}


	doCalculate();
}

void WtTWapExeUnit::on_channel_lost()
{

}

void WtTWapExeUnit::on_tick(WTSTickData* newTick)
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

	if (isFirstTick)	//����ǵ�һ��tick������Ŀ���λ�����������µ�
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();
		uint64_t    _fire_span = _exeSecondTime * 1000 / _twap_times;

		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);
		uint64_t total_ExeSecond = _exeSecondTime;
		uint64_t now = _ctx->getCurTime();
		if (newVol != undone + realPos) // ��λ�仯Ҫ����
		{
			doCalculate();
		}
	}
	else
	{
		uint64_t now = _ctx->getCurTime();
		if (_sticky != 0 && !_orders.empty())
		{
			
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto v : _orders)
			{
				uint32_t localid = v.first;
				uint64_t firetime = v.second;
				if (now - firetime > _sticky * 1000) //Ҫ�����ع�
				{
					if (_ctx->cancel(localid))
					{
						_cancel_cnt++;
						_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
					}
				}
			}
		}
		
		if(now - _last_fire_time >= _fire_span)
		{
			doCalculate();
		}
	}
}

void WtTWapExeUnit::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	//���ô�����������ontick�ﴥ����
	//_ctx->writeLog("%s��Լ%s%u�֣����´���ִ���߼�", stdCode, isBuy?"����":"����", vol);
	//doCalculate();
}

void WtTWapExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtTWapExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;
	double newVol = 0;
	if (abs(_target_pos) >= _twap_times) {
		double newVol = _target_pos / _twap_times;
	}
	if (abs(_target_pos) <_twap_times  && _target_pos != 0)
	{
		newVol = 1;
	}
	if (_target_pos ==0 )
	{
		newVol = 0;
	} //figure out min order newVol to place 
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);

	//����з���δ��ɵ�����ֱ�ӳ���
	//���Ŀ���λΪ0���ҵ�ǰ�ֲ�Ϊ0������ȫ���ҵ�
	if (newVol * undone < 0)
	{
		bool isBuy = (undone > 0);
		//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		_cancel_cnt += ids.size();
		_ctx->writeLog("%s@%d cancelcnt -> %u", __FUNCTION__, __LINE__, _cancel_cnt);
		return;
	}
	else if (newVol == 0 && undone != 0)
	{
		//���Ŀ���λΪ0����δ��ɲ�Ϊ0
		//��ô��Ŀǰ��λΪ0������ Ŀǰ��λ��δ�������������ͬ
		//����ҲҪȫ������
		if (realPos == 0 || (realPos * undone > 0))
		{
			bool isBuy = (undone > 0);
			//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
			OrderIDs ids = _ctx->cancel(stdCode, isBuy);
			_cancel_cnt += ids.size();
			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			return;
		}
	}

	bool bNeedShowHand = false;
	uint64_t now = _ctx->getCurTime();
	//����Ѿ�����β��ʱ�䣬����Ҫһ���Է���ȥ
	if (_exec_begin_time != UINT64_MAX && ((now - _exec_begin_time) >= _exeSecondTime * 1000)) //����60s ִ��ʱ��
		bNeedShowHand = true;

	if (bNeedShowHand && !_show_hand && undone != 0)
	{
		_ctx->writeLog("�ѵ�ִ��ʱ��ĩβ�׶Σ�ȫ���������ж���");
		OrderIDs ids = _ctx->cancel(stdCode, undone > 0);
		_cancel_cnt += ids.size();
		_ctx->writeLog("%s@%d cancelcnt -> %u", __FUNCTION__, __LINE__, _cancel_cnt);
		_show_hand = true;
		return;
	}

	//�������ͬ��ģ����������
	double curPos = realPos + undone;
	if (curPos == _target_pos) //�µ���Ŀ���λ
		return;

	if (_last_tick == NULL)
	{
		//grabLastTick���Զ��������ü���������Ҫ��retain
		_last_tick = _ctx->grabLastTick(stdCode);
	}

	if (_last_tick == NULL)
	{
		_ctx->writeLog("%sû������tick���ݣ��˳�ִ���߼�", _code.c_str());
		return;
	}

	if (!_channel_ready)
		return;

	_last_tick->retain();
	WTSTickData* curTick = _last_tick;

	if(bNeedShowHand) //  last showhand time
	{
		double qty = _target_pos - realPos; // showhand qty,  place  in one time
		double targetPx = 0;
		if(qty > 0)
			targetPx = curTick->askprice(0) + _comm_info->getPriceTick() * 3;
		else
			targetPx = curTick->bidprice(0) - _comm_info->getPriceTick() * 3;
		OrderIDs ids;
		if(qty > 0)
			ids = _ctx->buy(_code.c_str(), targetPx, abs(qty));
		else
			ids = _ctx->sell(_code.c_str(), targetPx, abs(qty));

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
		return;
	}
	// ------------- TWAP place order--------------------------------------
	double diffQty = _target_pos - curPos;
	const char* code = _code.c_str();
	if (diffQty > 0) {
		//�����źţ��ҵ�ǰ��λС�ڵ���0
		 //���¼�+2���µ�
		double targetPx = curTick->bidprice(0);
		auto ids = _ctx->buy(code, targetPx, abs(newVol)); //  twap place order

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;

	}
	if (diffQty < 0) {
		//�����źţ�����λΪ��������
		//���¼�+2���µ�
		double targetPx = curTick->askprice(0);
		auto ids = _ctx->sell(code, targetPx, abs(newVol)); //  twap place order

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;

	}

	curTick->release();
}

void WtTWapExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	double diff = newVol - _target_pos;
	if (diff == 0)
		return;

	_show_hand = false;

	_target_pos = newVol;

	_exec_begin_time = _ctx->getCurTime();

	_fire_span = (uint32_t)round((_exec_secs - _exec_tail)*1000.0 / abs(diff));

	doCalculate();
}
