#include "WtHftStraDemo.h"
#include "../Includes/IHftStraCtx.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/TimeUtils.hpp"

extern const char* FACT_NAME;

WtHftStraDemo::WtHftStraDemo(const char* id)
	: HftStrategy(id)
	, _last_tick(NULL)
	, _terminated(false)
	, _last_entry_time(0)
	, _thrd(NULL)
	, _channel_ready(false)
	, _last_calc_time(0)
{
}


WtHftStraDemo::~WtHftStraDemo()
{
	if (_last_tick)
		_last_tick->release();

	_terminated = true;
	if (_thrd)
		_thrd->join();
}

const char* WtHftStraDemo::getName()
{
	return "HftDemoStrategy";
}

const char* WtHftStraDemo::getFactName()
{
	return FACT_NAME;
}

bool WtHftStraDemo::init(WTSVariant* cfg)
{
	//������ʾһ���ⲿ��������Ļ�ȡ
	_code = cfg->getCString("code");
	_count = cfg->getUInt32("count");
	_secs = cfg->getUInt32("second");
	_offset = cfg->getUInt32("offset");

	return true;
}

void WtHftStraDemo::on_entrust(uint32_t localid, bool bSuccess, const char* message)
{

}

void WtHftStraDemo::on_init(IHftStraCtx* ctx)
{
	WTSTickSlice* ticks = ctx->stra_get_ticks(_code.c_str(), _count);
	if (ticks)
		ticks->release();

	WTSKlineSlice* kline = ctx->stra_get_bars(_code.c_str(), "m1", 30);
	if (kline)
		kline->release();

	ctx->sub_ticks(_code.c_str());

	_ctx = ctx;
}

void WtHftStraDemo::on_tick(IHftStraCtx* ctx, const char* code, WTSTickData* newTick)
{	
	if (_code.compare(code) != 0)
		return;

	if (!_orders.empty())
	{
		//���������Ϊ�գ�����붩�������߼�
		return;
	}

	if (!_channel_ready)
		return;

	WTSTickData* curTick = ctx->stra_get_last_tick(code);
	if (curTick)
		curTick->release();

	uint32_t curMin = newTick->actiontime() / 100000;	//actiontime�Ǵ�����ģ�Ҫȡ�÷��ӣ�����Ҫ����10w
	if (curMin > _last_calc_time)
	{//���spread�ϴμ����ʱ��С�ڵ�ǰ���ӣ�������spread
		WTSKlineSlice* kline = ctx->stra_get_bars(code, "m5", 30);
		if (kline)
			kline->release();

		//���������Ժ󣬸��¼���ʱ��
		_last_calc_time = curMin;
	}

	//30���ڲ��ظ�����
	uint64_t now = (uint64_t)ctx->stra_get_date()*1000000000 + (uint64_t)ctx->stra_get_time()*100000 + ctx->stra_get_secs();
	if(now - _last_entry_time <= 30*1000)
	{
		return;
	}

	int32_t signal = 0;
	double price = newTick->price();
	//���㲿��
	double pxInThry = (newTick->bidprice(0)*newTick->askqty(0) + newTick->askprice(0)*newTick->bidqty(0)) / (newTick->bidqty(0) + newTick->askqty(0));

	//���ۼ۸�������¼�
	if (pxInThry > price)
	{
		//�����ź�
		signal = 1;
	}
	else if (pxInThry < price)
	{
		//�����ź�
		signal = -1;
	}

	if (signal != 0)
	{
		double curPos = ctx->stra_get_position(code);
		WTSCommodityInfo* cInfo = ctx->stra_get_comminfo(code);

		if(signal > 0  && curPos <= 0)
		{//�����źţ��ҵ�ǰ��λС�ڵ���0
			//���¼�+2���µ�
			double targetPx = price + cInfo->getPriceTick() * _offset;
			auto ids = ctx->stra_buy(code, targetPx, 1);

			_mtx_ords.lock();
			for( auto localid : ids)
			{
				_orders.insert(localid);
			}
			_mtx_ords.unlock();
			_last_entry_time = now;
		}
		else if (signal < 0 && curPos >= 0)
		{//�����źţ��ҵ�ǰ��λ���ڵ���0
			//���¼�-2���µ�
			double targetPx = price - cInfo->getPriceTick()*_offset;
			auto ids = ctx->stra_sell(code, targetPx, 1);

			_mtx_ords.lock();
			for (auto localid : ids)
			{
				_orders.insert(localid);
			}
			_mtx_ords.unlock();
			_last_entry_time = now;
		}

		if (_secs > 0 && _thrd == NULL)	//��������˳�ʱʱ�䣬����������ֳ�
		{
			_thrd.reset(new std::thread(&WtHftStraDemo::check_orders, this));
		}
	}
}

void WtHftStraDemo::check_orders()
{
	while (!_terminated)
	{
		if (!_orders.empty())
		{
			uint64_t now = TimeUtils::getLocalTimeNow();
			if (now - _last_entry_time >= _secs * 1000)	//�������һ��ʱ��û�гɽ��꣬����
			{
				_mtx_ords.lock();
				for( auto localid : _orders)
				{
					_ctx->stra_cancel(localid);
				}
				_mtx_ords.unlock();
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void WtHftStraDemo::on_bar(IHftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	
}

void WtHftStraDemo::on_schedule(IHftStraCtx* ctx, uint32_t uDate, uint32_t uTime)
{

}

void WtHftStraDemo::on_trade(IHftStraCtx* ctx, const char* stdCode, bool isBuy, double qty, double price)
{
	
}

void WtHftStraDemo::on_position(IHftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	
}

void WtHftStraDemo::on_order(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	//��������ҷ���ȥ�Ķ������ҾͲ�����
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	//����ѳ�������ʣ������Ϊ0���������ԭ�е�id��¼
	if(isCanceled || leftQty == 0)
	{
		_mtx_ords.lock();
		_orders.erase(it);
		_mtx_ords.unlock();
	}
}


void WtHftStraDemo::on_channel_ready(IHftStraCtx* ctx)
{
	_channel_ready = true;
}

void WtHftStraDemo::on_channel_lost(IHftStraCtx* ctx)
{
	_channel_ready = false;
}