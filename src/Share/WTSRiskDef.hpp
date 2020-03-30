#pragma once
#include "WTSObject.hpp"

NS_OTP_BEGIN
typedef struct _TradeStatInfo
{
	char		_code[MAX_INSTRUMENT_LENGTH];
	//��ƽͳ��
	uint32_t	l_openvol;	//���տ������
	uint32_t	l_closevol;	//����ƽ�����
	uint32_t	l_closetvol;//����ƽ������
	uint32_t	s_openvol;	//���տ��ղ���
	uint32_t	s_closevol;	//����ƽ�ղ���
	uint32_t	s_closetvol;//����ƽ��ղ���

	//�ҵ�ͳ��
	uint32_t	b_orders;	//ί�����
	uint32_t	b_ordqty;	//ί������
	uint32_t	s_orders;	//ί������
	uint32_t	s_ordqty;	//ί������

	//����ͳ��
	uint32_t	b_cancels;	//�������
	uint32_t	b_canclqty;	//��������
	uint32_t	s_cancels;	//��������
	uint32_t	s_canclqty;	//��������

	//��ͳ��
	uint32_t	b_wrongs;	//������
	uint32_t	b_wrongqty;	//������
	uint32_t	s_wrongs;	//������
	uint32_t	s_wrongqty;	//������

	_TradeStatInfo()
	{
		memset(this, 0, sizeof(_TradeStatInfo));
	}
} TradeStatInfo;

class WTSTradeStateInfo : public WTSObject
{
protected:
	WTSTradeStateInfo(){}

public:
	static WTSTradeStateInfo* create(const char* code)
	{
		WTSTradeStateInfo* pRet = new WTSTradeStateInfo();
		strcpy(pRet->_trd_stat_info._code, code);

		return pRet;
	}

	TradeStatInfo&	statInfo(){ return _trd_stat_info; }
	const TradeStatInfo& statInfo() const{ return _trd_stat_info; }

	const char* code() const{ return _trd_stat_info._code; }

	uint32_t open_volumn_long() const{ return _trd_stat_info.l_openvol; }
	uint32_t close_volumn_long() const{ return _trd_stat_info.l_closevol; }
	uint32_t closet_volumn_long() const{ return _trd_stat_info.l_closetvol; }
	uint32_t open_volumn_short() const{ return _trd_stat_info.s_openvol; }
	uint32_t close_volumn_short() const{ return _trd_stat_info.s_closevol; }
	uint32_t closet_volumn_short() const{ return _trd_stat_info.s_closetvol; }

	uint32_t orders_buy() const{ return _trd_stat_info.b_orders; }
	uint32_t ordqty_buy() const{ return _trd_stat_info.b_ordqty; }
	uint32_t orders_sell() const{ return _trd_stat_info.s_orders; }
	uint32_t ordqty_sell() const{ return _trd_stat_info.s_ordqty; }

	uint32_t cancels_buy() const{ return _trd_stat_info.b_cancels; }
	uint32_t cancelqty_buy() const{ return _trd_stat_info.b_canclqty; }
	uint32_t cancels_sell() const{ return _trd_stat_info.s_cancels; }
	uint32_t cancelqty_sell() const{ return _trd_stat_info.s_canclqty; }

	uint32_t total_cancels() const{ return _trd_stat_info.b_cancels + _trd_stat_info.s_cancels; }

private:
	TradeStatInfo	_trd_stat_info;
};

//����ʽ�����
typedef struct _WTSFundStruct
{
	double		_predynbal;		//�ڳ���̬Ȩ��
	double		_prebalance;	//�ڳ���̬Ȩ��
	double		_balance;		//��̬Ȩ��
	double		_profit;		//ƽ��ӯ��
	double		_dynprofit;		//����ӯ��
	double		_fees;			//Ӷ��
	uint32_t	_last_date;		//�ϴν��㽻����

	double		_max_dyn_bal;	//�������ֵ
	uint32_t	_max_time;		//���ڸߵ����ʱ��
	double		_min_dyn_bal;	//������С��ֵ
	uint32_t	_min_time;		//���ڵ͵����ʱ��

	typedef struct _DynBalPair
	{
		uint32_t	_date;
		double		_dyn_balance;

		_DynBalPair()
		{
			memset(this, 0, sizeof(_DynBalPair));
		}
	} DynBalPair;

	DynBalPair	_max_md_dyn_bal;	//���̬��ֵ
	DynBalPair	_min_md_dyn_bal;	//��С��̬��ֵ

	_WTSFundStruct()
	{
		memset(this, 0, sizeof(_WTSFundStruct));
		_max_dyn_bal = DBL_MAX;
		_min_dyn_bal = DBL_MAX;
	}
} WTSFundStruct;


class WTSPortFundInfo : public WTSObject
{
protected:
	WTSPortFundInfo(){}

public:
	static WTSPortFundInfo* create()
	{
		WTSPortFundInfo* pRet = new WTSPortFundInfo();
		return pRet;
	}

	WTSFundStruct&	fundInfo(){ return _fund_info; }
	const WTSFundStruct& fundInfo() const{ return _fund_info; }

	double predynbalance() const{ return _fund_info._predynbal; }
	double balance() const{ return _fund_info._balance; }
	double profit() const{ return _fund_info._profit; }
	double dynprofit() const{ return _fund_info._dynprofit; }
	double fees() const{ return _fund_info._fees; }

	double max_dyn_balance() const{ return _fund_info._max_dyn_bal; }
	double min_dyn_balance() const{ return _fund_info._min_dyn_bal; }

	double max_md_dyn_balance() const{ return _fund_info._max_md_dyn_bal._dyn_balance; }
	double min_md_dyn_balance() const{ return _fund_info._min_md_dyn_bal._dyn_balance; }

	uint32_t max_dynbal_time() const{ return _fund_info._max_time; }
	uint32_t min_dynbal_time() const{ return _fund_info._min_time; }

	uint32_t last_settle_date() const{ return _fund_info._last_date; }

	uint32_t max_md_dynbal_date() const{ return _fund_info._max_md_dyn_bal._date; }
	uint32_t min_md_dynbal_date() const{ return _fund_info._min_md_dyn_bal._date; }


private:
	WTSFundStruct	_fund_info;
};

NS_OTP_END