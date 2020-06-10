#include "PySelContext.h"
#include "WtRtRunner.h"

extern WtRtRunner& getRunner();


PySelContext::PySelContext(WtSelEngine* env, const char* name)
	: SelStraBaseCtx(env, name)
{
}


PySelContext::~PySelContext()
{
}

void PySelContext::on_init()
{
	SelStraBaseCtx::on_init();

	//���ⲿ�ص�
	getRunner().ctx_on_init(_context_id, false);
}

void PySelContext::on_session_begin()
{
	SelStraBaseCtx::on_session_begin();
}

void PySelContext::on_session_end()
{
	SelStraBaseCtx::on_session_end();
}

void PySelContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	getRunner().ctx_on_calc(_context_id, false);
}

void PySelContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, false);
}

void PySelContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, false);
}
