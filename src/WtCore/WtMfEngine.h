#pragma once
#include "WtEngine.h"
#include "IMfStraCtx.h"
#include "WtExecuter.h"

#include <unordered_map>
#include <memory>

NS_OTP_BEGIN

typedef enum tagTaskPeriodType
{
	TPT_None,		//���ظ�
	TPT_Daily = 8,	//ÿ��������
	TPT_Weekly,		//ÿ�ܣ������ڼ��յĻ�Ҫ˳��
	TPT_Monthly,	//ÿ�£������ڼ���˳��
	TPT_Yearly		//ÿ�꣬�����ڼ���˳��
}TaskPeriodType;

typedef struct _TaskInfo
{
	uint32_t	_id;
	char		_name[16];		//������
	char		_trdtpl[16];	//������ģ��
	uint32_t	_day;			//���ڣ��������ڱ仯��ÿ��Ϊ0��ÿ��Ϊ0~6����Ӧ���յ�������ÿ��Ϊ1~31��ÿ��Ϊ0101~1231
	uint32_t	_time;			//ʱ�䣬��ȷ������
	bool		_strict_time;	//�Ƿ����ϸ�ʱ�䣬�ϸ�ʱ�伴ֻ��ʱ����ȲŻ�ִ�У������ϸ�ʱ�䣬����ڵ��ڴ���ʱ�䶼��ִ��

	uint64_t	_last_exe_time;	//�ϴ�ִ��ʱ�䣬��ҪΪ�˷�ֹ�ظ�ִ��

	TaskPeriodType	_period;	//��������
} TaskInfo;

typedef std::shared_ptr<TaskInfo> TaskInfoPtr;

typedef boost::shared_ptr<IMfStraCtx> MfContextPtr;
class WtMfRtTicker;


class WtMfEngine : public WtEngine
{
public:
	WtMfEngine();
	~WtMfEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine�ӿ�
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr) override;

	virtual void run(bool bAsync = false) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_push_quote(WTSTickData* newTick, bool isHot) override;

	virtual void on_init() override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

public:
	//uint32_t	register_task(const char* name, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA");
	void			addContext(MfContextPtr ctx, uint32_t date, uint32_t time, TaskPeriodType period, bool bStrict = true, const char* trdtpl = "CHINA");

	MfContextPtr	getContext(uint32_t id);

	inline void addExecuter(WtExecuterPtr& executer)
	{
		_executers.push_back(executer);
		executer->setEngine(this);
	}

	void	on_minute_end(uint32_t uDate, uint32_t uTime);

private:
	std::unordered_map<uint32_t, TaskInfoPtr>	_tasks;

	typedef std::unordered_map<uint32_t, MfContextPtr> ContextMap;
	ContextMap		_ctx_map;

	typedef std::vector<WtExecuterPtr> ExecuterList;
	ExecuterList	_executers;

	bool	_terminated;

	WtMfRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;
};

NS_OTP_END
