/*!
 * \file ParserUT.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <map>
#include <string>

#if _WIN32
#ifndef WINAPI
#define WINAPI __stdcall
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#endif

#include "../Share/IParserApi.h"
#include "./UTApi/XYZQ_UT_API.h"


NS_OTP_BEGIN
class WTSTickData;
NS_OTP_END

USING_NS_OTP;

class ParserUT : public IParserApi, public xyzq_ut_spi
{
public:
	ParserUT();
	virtual ~ParserUT();

public:
	enum LoginStatus
	{
		LS_NOTLOGIN,
		LS_LOGINING,
		LS_LOGINED
	};

//IQuoteParser �ӿ�
public:
	virtual bool init(WTSParams* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerListener(IParserApiListener* listener) override;

//////////////////////////////////////////////////////////////////////////
//xyzq_ut_spi �ӿ�
public:
	//������Ϣ��Ӧ
	virtual void rsp_error(error_msg *arg) override;
	//״̬��Ӧ
	virtual void rsp_status_changed(int status) override;
	//����Ʒ����������
	virtual void rsp_market_data(ut_data *arg) override;
	//���Ʒ����������
	virtual void rsp_market_data_ex(ut_data_list *arg) override;
	//����Ʒ����ʳɽ�����
	virtual void rsp_transactions(ut_data *arg) override;
	//����Ʒ�����ί������
	virtual void rsp_orders(ut_data *arg) override;
	//����Ʒ��ί�ж�������
	virtual void rsp_order_list(ut_order_queue_list *arg) override;
	//��ѯ������Ϣ��Ӧ
	virtual void rsp_base_info(rsp_base_info_s *arg) override;

private:
	/*
	 *	���͵�¼����
	 */
	void DoLogin();
	/*
	 *	����Ʒ������
	 */
	void SubscribeMDs(bool isAll = false);
	/*
	 *	��������Ϣ
	 */
	bool IsErrorRspInfo(error_msg *pRspInfo);

	WTSTickData*	makeTickData(ut_data* pMktData);

private:
	LoginStatus			m_loginState;
	xyzq_ut_api*		m_pUserAPI;

	std::string			m_strFrontAddr;
	std::string			m_strUserNo;
	std::string			m_strUserID;
	std::string			m_strPassword;

	bool				m_bHasOrder;
	bool				m_bHasOrdQue;
	bool				m_bHasTrans;
	bool				m_bLogined;

	CodeSet				m_filterSubs;
	CodeSet				m_ignoreCodes;

	IParserApiListener*	m_parserSink;
	IBaseDataMgr*		m_pBaseDataMgr;
};

