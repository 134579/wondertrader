#include "WtHfExeUnit.h"
#include <math.h>

#include "../Share/TimeUtils.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"

extern const char* FACT_NAME;


WtHfExeUnit::WtHfExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _sticky(0)
	, _cancel_cnt(0)
	, _channel_ready(false)
	, _last_fire_time(0)
	, _exec_begin_time(UINT64_MAX)
	, _target_pos(0)
{
}


WtHfExeUnit::~WtHfExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtHfExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtHfExeUnit::getName()
{
	return "WtHfExeUnit";
}

void WtHfExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_sticky = cfg->getUInt32("sticky");
	_exec_secs = cfg->getUInt32("execsecs");
	_exec_tail = cfg->getUInt32("tailsecs");

	ctx->writeLog("ִ�е�Ԫ WtHfExeUnit[%s] ��ʼ����ɣ�������ʱ %u �룬ִ��ʱ�� %u �룬��βʱ�� %u ��", stdCode, _sticky, _exec_secs, _exec_tail);
}

void WtHfExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t leftover, double price, bool isCanceled)
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

void WtHfExeUnit::on_channel_ready()
{
	_channel_ready = true;
	int32_t undone = _ctx->getUndoneQty(_code.c_str());
	if (undone != 0 && _orders.empty())
	{
		//��˵����δ��ɵ����ڼ��֮�У��ȳ���
		_ctx->writeLog("%s�в��ڹ����е�δ��ɵ� %d �֣�ȫ������", _code.c_str(), undone);

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

void WtHfExeUnit::on_channel_lost()
{

}

void WtHfExeUnit::on_tick(WTSTickData* newTick)
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
		int32_t newVol = _target_pos;
		const char* stdCode = _code.c_str();

		int32_t undone = _ctx->getUndoneQty(stdCode);
		int32_t realPos = _ctx->getPosition(stdCode);

		if (newVol != undone + realPos)
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
				if (now - firetime > _sticky * 1000)
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

void WtHfExeUnit::on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price)
{
	//���ô�����������ontick�ﴥ����
	//_ctx->writeLog("%s��Լ%s%u�֣����´���ִ���߼�", stdCode, isBuy?"����":"����", vol);
	//doCalculate();
}

void WtHfExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtHfExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;

	int32_t newVol = _target_pos;
	const char* stdCode = _code.c_str();

	int32_t undone = _ctx->getUndoneQty(stdCode);
	int32_t realPos = _ctx->getPosition(stdCode);

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
	if (_exec_begin_time != UINT64_MAX && ((now - _exec_begin_time) >= (_exec_secs - _exec_tail) * 1000))
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
	int32_t curPos = realPos + undone;
	if (curPos == newVol)
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

	if(bNeedShowHand)
	{
		int32_t qty = newVol - realPos;
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

	const char* code = _code.c_str();
	int32_t signal = 0;
	double price = curTick->price();
	//���㲿��
	double alpha_bias = on_tickalpha_sum(curTick);

	//���ۼ۸�������¼�
	if ((alpha_bias) >= 2)
	{
		//�����ź�
		signal = 2;

	}
	else if ((alpha_bias) <= -2)
	{
		//�����ź�
		signal = -2;
	}

	if ((alpha_bias) < 2 && (alpha_bias) >= 1)
	{
		//�����ź�

		signal = 1;
	}
	else if ((alpha_bias) > -2 && (alpha_bias) <= -1)
	{
		//�����ź�
		signal = -1;
	}
	if ((alpha_bias) > -1 && (alpha_bias) < 1)
	{
		signal = 0;
	}

	int32_t diffQty = newVol - curPos;

	if (diffQty > 0)
	{//�����źţ��ҵ�ǰ��λС�ڵ���0
		double targetPx = curTick->bidprice(0) + _comm_info->getPriceTick() * abs(signal);
		auto ids = _ctx->buy(code, targetPx, 1);

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
	}
	else if (diffQty < 0)
	{//�����źţ��ҵ�ǰ��λ���ڵ���0
		double targetPx = curTick->askprice(0) - _comm_info->getPriceTick()* abs(signal);
		auto ids = _ctx->sell(code, targetPx, 1);

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
	}

	/*
	if (abs(signal) == 2) //  +2tick ׷��
	{
		if (signal > 0 && diffQty > 0)	
		{//�����źţ��ҵ�ǰ��λС�ڵ���0
			//���¼�+2���µ�
			double targetPx = curTick->askprice(0) + _comm_info->getPriceTick() * abs(signal);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (signal < 0 && diffQty < 0)
		{//�����źţ��ҵ�ǰ��λ���ڵ���0
			//���¼�-2���µ�
			double targetPx = curTick->bidprice(0) - _comm_info->getPriceTick()* abs(signal);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	else if (abs(signal) == 1)  //����ּ�
	{
		if (signal > 0 && diffQty > 0)
		{//�����źţ��ҵ�ǰ��λС�ڵ���0
			//���¼�+2���µ�
			double targetPx = curTick->askprice(0);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (signal < 0 && diffQty < 0)
		{//�����źţ��ҵ�ǰ��λ���ڵ���0
			//���¼�-2���µ�
			double targetPx = curTick->bidprice(0);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	else if (abs(signal) == 0)  //�����ż۵ȴ�
	{
		if (diffQty > 0)
		{//�����źţ��ҵ�ǰ��λС�ڵ���0
			//���¼�+2���µ�
			double targetPx = curTick->bidprice(0);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (diffQty < 0)
		{//�����źţ��ҵ�ǰ��λ���ڵ���0
			//���¼�-2���µ�
			double targetPx = curTick->askprice(0);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	*/

	curTick->release();
}

void WtHfExeUnit::set_position(const char* stdCode, int32_t newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	int32_t diff = newVol - _target_pos;
	if (diff == 0)
		return;

	_show_hand = false;

	_target_pos = newVol;

	_exec_begin_time = _ctx->getCurTime();

	_fire_span = (uint32_t)round((_exec_secs - _exec_tail)*1000.0 / abs(diff));

	doCalculate();
}

//**********************************************************************************************************************************
double WtHfExeUnit::on_tick_bias(WTSTickData* curTick)
{
	//ÿ��30s ����һ��tickbiasֵ
	uint64_t now = (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime();
	if (now - _last_tickbias_cal_time <= 30 * 1000)
	{
		if (curTick->price() == curTick->bidprice(0)) // ����
		{
			_total_s = _total_s + -1 * curTick->volumn();
		}
		if (curTick->price() == curTick->askprice(0)) // ����
		{
			_total_s = _total_s + 1 * curTick->volumn();
		}
		if (_total_s > 0)
		{

			_alpha_tickbias = sqrt(_total_s);
		}
		if (_total_s < 0)
		{

			_alpha_tickbias = -sqrt(-_total_s);
		}

		return _old_alpha_tickbias;
	}

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_tickbias = 0.1 * _alpha_tickbias / curTick->volumn();
	}
	else
	{
		_alpha_tickbias = 0;
	}
	_total_s = 0;
	_last_tickbias_cal_time = now;
	_old_alpha_tickbias = _alpha_tickbias;

	return _alpha_tickbias;
}

double WtHfExeUnit::on_trade_bias(WTSTickData* curTick)
{
	double bidsz_tick = 0.0;
	double asksz_tick = 0.0;
	double _alpha_tradebias = 0.0;

	bidsz_tick = curTick->volumn() * (curTick->askprice(0) - curTick->price()) / (curTick->askprice(0) - curTick->bidprice(0));
	asksz_tick = curTick->volumn() * (curTick->price() - curTick->bidprice(0)) / (curTick->askprice(0) - curTick->bidprice(0));
	_alpha_tradebias = asksz_tick - bidsz_tick;


	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_tradebias = 0.1 * _alpha_tradebias / curTick->volumn();
	}
	else
	{
		_alpha_tradebias = 0;
	}

	return _alpha_tradebias;
}


double WtHfExeUnit::on_size_bias(WTSTickData* curTick)
{
	double _alpha_sizebias = 0.0;

	if (curTick->askqty(0) != 0)
	{
		_alpha_sizebias = log(curTick->bidqty(0) / curTick->askqty(0));
	}
	else
	{
		_alpha_sizebias = 0.0;
	}

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_sizebias = 0.2 * _alpha_sizebias / curTick->volumn();
	}
	else
	{
		_alpha_sizebias = 0;
	}

	return _alpha_sizebias;
}


double WtHfExeUnit::on_book_bias(WTSTickData* curTick)
{
	double _alpha_bookbias = 0.0;
	double  ask_level = 0.0;
	double  bid_level = 0.0;


	ask_level = (curTick->askprice(0) - curTick->price()) / 2;
	bid_level = (curTick->bidprice(0) - curTick->price()) / 2;

	_alpha_bookbias = ask_level + bid_level;

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_bookbias = 0.5 * _alpha_bookbias;
	}
	else
	{
		_alpha_bookbias = 0;
	}

	return _alpha_bookbias;
}


double WtHfExeUnit::on_tickalpha_sum(WTSTickData* curTick)
{
	double _alpha_Sum = 0.0;
	double alpha_1 = 0.0;
	double alpha_2 = 0.0;
	double alpha_3 = 0.0;
	double alpha_4 = 0.0;

	alpha_1 = on_book_bias(curTick);
	alpha_2 = on_size_bias(curTick);
	alpha_3 = on_trade_bias(curTick);
	alpha_4 = on_tick_bias(curTick);

	_alpha_Sum = alpha_1 + alpha_2 + alpha_3 + alpha_4;

	if (abs(_alpha_Sum) >= 10)
	{
		if (_alpha_Sum > 0)
		{
			return 2; // ׷����alpha��ǿʱ
		}
		if (_alpha_Sum < 0)
		{
			return -2; // ׷������alpha��ǿʱ
		}
	}
	else if (abs(_alpha_Sum) >= 5 && abs(_alpha_Sum) < 10)
	{
		if (_alpha_Sum > 0)
		{
			return 1; // ���ּ۹ҵ���alpha��ǿʱ
		}
		if (_alpha_Sum < 0)
		{
			return -1; // ���ּ۹ҵ�����alpha��ǿʱ
		}
	}
	else if (abs(_alpha_Sum) < 5)
	{
		if (_alpha_Sum > 0)
		{
			return 0; // ���ż۹ҵ���alpha����������ʱ
		}
		if (_alpha_Sum < 0)
		{
			return 0; // ���ż۹ҵ�����alpha����������ʱ
		}
	}

	return 0;
}
//**************************************************************************************
