/*!
 * \file IDataReader.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>

#include "../Share/WTSMarcos.h"
#include "../Share/WTSTypes.h"

NS_OTP_BEGIN
class WTSHisTickData;
class WTSKlineData;
class WTSKlineSlice;
class WTSTickSlice;
struct WTSBarStruct;
class WTSVariant;
class IBaseDataMgr;
class IHotMgr;

/*
 *	@brief ���ݶ�ȡģ��ص��ӿ�
 *	@details ��Ҫ�������ݶ�ȡģ�������ģ��ص�
 */
class IDataReaderSink
{
public:
	/*
	 *	@brief	K�߱պ��¼��ص�
	 *	
	 *	@param stdCode	��׼Ʒ�ִ��룬��SSE.600000��SHFE.au.2005
	 *	@param period	K������
	 *	@param newBar	�պϵ�K�߽ṹָ��
	 */
	virtual void on_bar(const char* stdCode, WTSKlinePeriod period, WTSBarStruct* newBar) = 0;

	/*
	 *	@brief	���л����K��ȫ�����µ��¼��ص�
	 *
	 *	@param updateTime	K�߸���ʱ�䣬��ȷ�����ӣ���202004101500
	 */
	virtual void on_all_bar_updated(uint32_t updateTime) = 0;

	/*
	 *	@brief	��ȡ�������ݹ���ӿ�ָ��
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() = 0;

	 /*
	  *	@brief	��ȡ�����л��������ӿ�ָ��
	  */
	virtual IHotMgr*		get_hot_mgr() = 0;

	/*
	 *	@brief	��ȡ��ǰ���ڣ���ʽͼ20100410
	 */
	virtual uint32_t	get_date() = 0;

	/*
	 *	@brief	��ȡ��ǰ1�����ߵ�ʱ��
	 *	@details ����ķ�����ʱ���Ǵ������1������ʱ�䣬��������9:00:32�룬��ʵ�¼�Ϊ0900�����Ƕ�Ӧ��1������ʱ��Ϊ0901
	 */
	virtual uint32_t	get_min_time() = 0;

	/*
	 *	@brief	��ȡ��ǰ����������ȷ�����룬��37,500
	 */
	virtual uint32_t	get_secs() = 0;

	/*
	 *	@brief	������ݶ�ȡģ�����־
	 */
	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) = 0;
};

/*
 *	@brief	���ݶ�ȡ�ӿ�
 *
 *	�����ģ���ṩ��������(tick��K��)��ȡ�ӿ�
 */
class IDataReader
{
public:
	IDataReader(){}
	virtual ~IDataReader(){}

public:
	/*
	 *	@brief ��ʼ�����ݶ�ȡģ��
	 *
	 *	@param cfg	ģ��������
	 *	@param sink	ģ��ص��ӿ�
	 */
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink){ _sink = sink; }

	/*
	 *	@brief	�����߱պ��¼�����ӿ�
	 *	
	 *	@param uDate	�պϵķ��������ڣ���20200410�����ﲻ�ǽ�����
	 *	@param uTime	�պϵķ����ߵķ���ʱ�䣬��1115
	 *	@param endTDate	����պϵķ������ǽ��������һ�������ߣ���endTDateΪ��ǰ�����գ���20200410���������Ϊ0
	 */
	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) = 0;

	/*
	 *	@brief	��ȡtick���ݣ�������һ���洢������
	 *	@details	����ֵWTSHisTickData����һ��WTSTickStruct�����У���ȡ������Ҫ���Ƶ���������
	 *	
	 *	@param stdCode	��׼Ʒ�ִ��룬��SSE.600000��SHFE.au.2005
	 *	@param count	Ҫ��ȡ��tick����
	 *	@param etime	����ʱ�䣬��ȷ�����룬��ʽ��yyyyMMddhhmmssmmm�����Ҫ��ȡ�����һ����etimeΪ0��Ĭ��Ϊ0
	 *	@param bOnlyValid	�Ƿ�ֻ����Чtick(�ɽ�����Ϊ0)��Ĭ��Ϊfalse
	 */
	virtual WTSHisTickData* readTicks(const char* stdCode, uint32_t count, uint64_t etime = 0, bool bOnlyValid = false) = 0;

	/*
	 *	@brief ��ȡK�����У�������һ���洢������
	 *	@details	���ص�WTSKlineData������WTSBarStruct�����У���ȡ������Ҫ���Ƶ���������
	 *	
	 *	@param	stdCode	��׼Ʒ�ִ��룬��SSE.600000��SHFE.au.2005
	 *	@param	period	K������
	 *	@param	count	Ҫ��ȡ��K������
	 *	@param	etime	����ʱ�䣬������������ݣ����ʽΪyyyyMMdd������Ƿ����ߣ���ΪyyyyMMddhhmm
	 */
	virtual WTSKlineData*	readBars(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief	��ȡtick������Ƭ
	 *	@details ��Ƭ���Ḵ�����ݣ�ֻ�ѻ����е�����ָ�봫�ݳ��������Խ�����Ƭ
	 *
	 *	@param stdCode	��׼Ʒ�ִ��룬��SSE.600000��SHFE.au.2005
	 *	@param count	Ҫ��ȡ��tick����
	 *	@param etime	����ʱ�䣬��ȷ�����룬��ʽ��yyyyMMddhhmmssmmm�����Ҫ��ȡ�����һ����etimeΪ0��Ĭ��Ϊ0
	 */
	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief ��ȡK�����У�������һ���洢������
	 *	@details	��Ƭ���Ḵ�����ݣ�ֻ�ѻ����е�����ָ�봫�ݳ��������Խ�����Ƭ
	 *
	 *	@param	stdCode	��׼Ʒ�ִ��룬��SSE.600000��SHFE.au.2005
	 *	@param	period	K������
	 *	@param	count	Ҫ��ȡ��K������
	 *	@param	etime	����ʱ�䣬������������ݣ����ʽΪyyyyMMdd������Ƿ����ߣ���ΪyyyyMMddhhmm
	 */
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

protected:
	IDataReaderSink* _sink;
};

//�������ݴ洢����
typedef IDataReader* (*FuncCreateDataReader)();
//ɾ�����ݴ洢����
typedef void(*FuncDeleteDataReader)(IDataReader* store);

NS_OTP_END