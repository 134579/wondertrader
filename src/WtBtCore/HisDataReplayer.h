/*!
 * \file HisDataReplayer.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "DataDefine.h"
#include "../WtDataWriter/MysqlDB.hpp"

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSTypes.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_OTP_BEGIN
class WTSTickData;
class WTSVariant;
class WTSKlineSlice;
class WTSTickSlice;
class WTSOrdDtlSlice;
class WTSOrdQueSlice;
class WTSTransSlice;
class WTSSessionInfo;
class WTSCommodityInfo;

class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;
NS_OTP_END

typedef std::shared_ptr<MysqlDb>	MysqlDbPtr;

USING_NS_OTP;

class IDataSink
{
public:
	virtual void	handle_tick(const char* stdCode, WTSTickData* curTick) = 0;
	virtual void	handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue) {};
	virtual void	handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl) {};
	virtual void	handle_transaction(const char* stdCode, WTSTransData* curTrans) {};
	virtual void	handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual void	handle_schedule(uint32_t uDate, uint32_t uTime) = 0;

	virtual void	handle_init() = 0;
	virtual void	handle_session_begin(uint32_t curTDate) = 0;
	virtual void	handle_session_end(uint32_t curTDate) = 0;
	virtual void	handle_replay_done() {}
};

class HisDataReplayer
{

private:
	template <typename T>
	class HftDataList
	{
	public:
		std::string		_code;
		uint32_t		_date;
		uint32_t		_cursor;
		uint32_t		_count;

		std::vector<T> _items;

		HftDataList() :_cursor(UINT_MAX), _count(0), _date(0){}
	};

	typedef std::unordered_map<std::string, HftDataList<WTSTickStruct>>		TickCache;
	typedef std::unordered_map<std::string, HftDataList<WTSOrdDtlStruct>>	OrdDtlCache;
	typedef std::unordered_map<std::string, HftDataList<WTSOrdQueStruct>>	OrdQueCache;
	typedef std::unordered_map<std::string, HftDataList<WTSTransStruct>>	TransCache;


	typedef struct _BarsList
	{
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_cursor;
		uint32_t		_count;
		uint32_t		_times;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_cursor(UINT_MAX), _count(0), _times(1){}
	} BarsList;

	typedef std::unordered_map<std::string, BarsList>	BarsCache;

	typedef enum tagTaskPeriodType
	{
		TPT_None,		//���ظ�
		TPT_Minute = 4,	//����������
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
		char		_session[16];	//����ʱ��ģ��
		uint32_t	_day;			//���ڣ��������ڱ仯��ÿ��Ϊ0��ÿ��Ϊ0~6����Ӧ���յ�������ÿ��Ϊ1~31��ÿ��Ϊ0101~1231
		uint32_t	_time;			//ʱ�䣬��ȷ������
		bool		_strict_time;	//�Ƿ����ϸ�ʱ�䣬�ϸ�ʱ�伴ֻ��ʱ����ȲŻ�ִ�У������ϸ�ʱ�䣬����ڵ��ڴ���ʱ�䶼��ִ��

		uint64_t	_last_exe_time;	//�ϴ�ִ��ʱ�䣬��ҪΪ�˷�ֹ�ظ�ִ��

		TaskPeriodType	_period;	//��������
	} TaskInfo;

	typedef std::shared_ptr<TaskInfo> TaskInfoPtr;



public:
	HisDataReplayer();
	~HisDataReplayer();

private:
	/*
	 *	���Զ��������ļ�������ʷ����
	 */
	bool		cacheRawBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	��csv�ļ�������ʷ����
	 */
	bool		cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	�����ݿ⻺����ʷ����
	 */
	bool		cacheRawBarsFromDB(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	���Զ��������ļ�������ʷtick����
	 */
	bool		cacheRawTicksFromBin(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	��csv�ļ�������ʷtick����
	 */
	bool		cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate);

	void		onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0);

	void		loadFees(const char* filename);

	void		replayHftDatas(uint64_t stime, uint64_t etime);

	uint64_t	replayHftDatasByDay(uint32_t curTDate);

	void		replayUnbars(uint64_t stime, uint64_t etime, uint32_t endTDate = 0);

	inline bool		checkTicks(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderDetails(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderQueues(const char* stdCode, uint32_t uDate);

	inline bool		checkTransactions(const char* stdCode, uint32_t uDate);

	void		checkUnbars();

	bool		loadStkAdjFactors(const char* adjfile);

	bool		loadStkAdjFactorsFromDB();

	void		initDB();

	bool		checkAllTicks(uint32_t uDate);

	inline	uint64_t	getNextTickTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdQueTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdDtlTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextTransTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);

public:
	bool init(WTSVariant* cfg);

	void run();

	void register_sink(IDataSink* listener){ _listener = listener; }

	void register_task(uint32_t taskid, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	WTSKlineSlice* get_kline_slice(const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, bool isMain = false);

	WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSTickData* get_last_tick(const char* stdCode);

	uint32_t get_date() const{ return _cur_date; }
	uint32_t get_min_time() const{ return _cur_time; }
	uint32_t get_raw_time() const{ return _cur_time; }
	uint32_t get_secs() const{ return _cur_secs; }
	uint32_t get_trading_date() const{ return _cur_tdate; }

	double calc_fee(const char* stdCode, double price, double qty, uint32_t offset);
	WTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	WTSCommodityInfo*	get_commodity_info(const char* stdCode);
	double get_cur_price(const char* stdCode);

	void sub_tick(uint32_t sid, const char* stdCode);
	void sub_order_queue(uint32_t sid, const char* stdCode);
	void sub_order_detail(uint32_t sid, const char* stdCode);
	void sub_transaction(uint32_t sid, const char* stdCode);

	bool	is_tick_enabled() const{ return _tick_enabled; }

private:
	IDataSink*		_listener;

	TickCache		_ticks_cache;	//tick����
	OrdDtlCache		_orddtl_cache;	//order detail����
	OrdQueCache		_ordque_cache;	//order queue����
	TransCache		_trans_cache;	//transaction����

	BarsCache		_bars_cache;	//K�߻���
	BarsCache		_unbars_cache;	//δ���ĵ�K�߻���

	TaskInfoPtr		_task;

	std::string		_main_key;
	std::string		_min_period;	//��СK�����ڣ������Ҫ����δ����Ʒ�ֵ��źŴ�����
	bool			_tick_enabled;
	std::map<std::string, WTSTickStruct>	_day_cache;	//ÿ��Tick����,��tick�ط�δ����ʱ�����õ��û���
	std::map<std::string, std::string>		_ticker_keys;

	uint32_t		_cur_date;
	uint32_t		_cur_time;
	uint32_t		_cur_secs;
	uint32_t		_cur_tdate;
	uint32_t		_closed_tdate;
	uint32_t		_opened_tdate;

	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;

	std::string		_base_dir;
	std::string		_mode;
	uint64_t		_begin_time;
	uint64_t		_end_time;

	//////////////////////////////////////////////////////////////////////////
	//������ģ��
	typedef struct _FeeItem
	{
		double	_open;
		double	_close;
		double	_close_today;
		bool	_by_volume;

		_FeeItem()
		{
			memset(this, 0, sizeof(_FeeItem));
		}
	} FeeItem;
	typedef std::unordered_map<std::string, FeeItem>	FeeMap;
	FeeMap		_fee_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_map<std::string, double> PriceMap;
	PriceMap		_price_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_set<uint32_t> SIDSet;
	typedef std::unordered_map<std::string, SIDSet>	StraSubMap;
	StraSubMap		_tick_sub_map;		//tick���ݶ��ı�
	StraSubMap		_ordque_sub_map;	//orderqueue���ݶ��ı�
	StraSubMap		_orddtl_sub_map;	//orderdetail���ݶ��ı�
	StraSubMap		_trans_sub_map;		//transaction���ݶ��ı�

	//��Ȩ����
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef std::unordered_map<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	inline const AdjFactorList& getAdjFactors(const char* code, const char* exchg)
	{
		char key[20] = { 0 };
		sprintf(key, "%s.%s", exchg, code);
		return _adj_factors[key];
	}

	typedef struct _DBConfig
	{
		bool	_active;
		char	_host[64];
		int32_t	_port;
		char	_dbname[32];
		char	_user[32];
		char	_pass[32];

		_DBConfig() { memset(this, 0, sizeof(_DBConfig)); }
	} DBConfig;

	DBConfig	_db_conf;
	MysqlDbPtr	_db_conn;
};

