#include "ExpSelMocker.h"
#include "WtBtRunner.h"

extern WtBtRunner& getRunner();


ExpSelMocker::ExpSelMocker(HisDataReplayer* replayer, const char* name)
	: SelMocker(replayer, name)
{
}


ExpSelMocker::~ExpSelMocker()
{
}

void ExpSelMocker::on_init()
{
	SelMocker::on_init();

	//���ⲿ�ص�
	getRunner().ctx_on_init(_context_id, ET_SEL);
}

void ExpSelMocker::on_session_begin()
{
	SelMocker::on_session_begin();
}

void ExpSelMocker::on_session_end()
{
	SelMocker::on_session_end();
}

void ExpSelMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	SelMocker::on_tick_updated(stdCode, newTick);

	//���ⲿ�ص�
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_SEL);
}

void ExpSelMocker::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	SelMocker::on_bar_close(stdCode, period, newBar);
	//Ҫ���ⲿ�ص�
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, ET_SEL);
}

void ExpSelMocker::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	SelMocker::on_strategy_schedule(curDate, curTime);

	//���ⲿ�ص�
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_SEL);
}
