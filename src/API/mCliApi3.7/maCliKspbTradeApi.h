#if !defined(__MA_CLI_KSPB_TRADE_API_H__)
#define __MA_CLI_KSPB_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliKspbTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliKspbTradeSpi : virtual public CCliTradeSpi
{
public:
  // ȷ�ϻر�
  virtual int OnRtnOrderConfirm(CRtnKspbRpcOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // �ɽ��ر�
  virtual int OnRtnOrderFill(CRtnKspbRpcOrderFillField *p_pRtnOrderFillField) {return 0;}
  // �û���½
  virtual int OnRspRpcLogin(CFirstSetField *p_pFirstSetField, CRspKspbRpcLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ί��
  virtual int OnRspRpcOrder(CFirstSetField *p_pFirstSetField, CRspKspbRpcOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ί�г���
  virtual int OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspKspbRpcOrderCancelField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɷݲ�ѯ
  virtual int OnRspRpcQryStkDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryStkDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʽ��ѯ
  virtual int OnRspRpcQryFund(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryFundField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ί�в�ѯ
  virtual int OnRspRpcQryOrderDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryOrderDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɽ���ѯ
  virtual int OnRspRpcQryMatchDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryMatchDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��Ϣ��ѯ
  virtual int OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, CRspKspbRpcStkInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɶ���ѯ
  virtual int OnRspRpcQryTrdacct(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryTrdacctField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ϣ��ѯ
  virtual int OnRspRpcQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankTransBasicInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��֤ת��
  virtual int OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankStkTransField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ת����ˮ��ѯ
  virtual int OnRspRpcQryBankStkTrans(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankStkTransInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷί�в�ѯ
  virtual int OnRspRpcQryHistoryOrderDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryHistoryOrderDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷ�ɽ���ѯ
  virtual int OnRspRpcQryHistoryMatchDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryHistoryMatchDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʽ�ɷ���ˮ��ѯ
  virtual int OnRspRpcQryStatement(CFirstSetField *p_pFirstSetField, CRspKspbRpcStatementField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����ѯ
  virtual int OnRspRpcQryDeliOrder(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryDeliOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliKspbTradeApi : virtual public CCliTradeApi
{
public:
  // Ĭ�Ϲ��캯��
  CCliKspbTradeApi(void);

  // ��������
  virtual ~CCliKspbTradeApi(void);

public:
  // �첽�ص�����
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // �����ص�����
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);
  
  // �û���½
  int ReqRpcLogin(CReqKspbRpcLoginField *p_pReqField, LONGLONG p_llRequestId);
  // ����ί��
  int ReqRpcOrder(CReqKspbRpcOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ί�г���
  int ReqRpcOrderCancel(CReqKspbRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // �ɷݲ�ѯ
  int ReqRpcQryStkDetail(CReqKspbRpcQryStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // �ʽ��ѯ
  int ReqRpcQryFund(CReqKspbRpcQryFundField *p_pReqField, LONGLONG p_llRequestId);
  // ί�в�ѯ
  int ReqRpcQryOrderDetail(CReqKspbRpcQryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // �ɽ���ѯ
  int ReqRpcQryMatchDetail(CReqKspbRpcQryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ��Ϣ��ѯ
  int ReqRpcStkInfo(CReqKspbRpcStkInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �ɶ���ѯ
  int ReqRpcQryTrdacct(CReqKspbRpcQryTrdacctField *p_pReqField, LONGLONG p_llRequestId);
  // ������Ϣ��ѯ
  int ReqRpcQryBankTransBasicInfo(CReqKspbRpcBankTransBasicInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��֤ת��
  int ReqRpcBankStkTrans(CReqKspbRpcBankStkTransField *p_pReqField, LONGLONG p_llRequestId);
  // ת����ˮ��ѯ
  int ReqRpcQryBankStkTrans(CReqKspbRpcBankStkTransInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷί�в�ѯ
  int ReqRpcQryHistoryOrderDetail(CReqKspbRpcQryHistoryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷ�ɽ���ѯ
  int ReqRpcQryHistoryMatchDetail(CReqKspbRpcQryHistoryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // �ʽ�ɷ���ˮ��ѯ
  int ReqRpcQryStatement(CReqKspbRpcStatementField *p_pReqField, LONGLONG p_llRequestId);
  // �����ѯ
  int ReqRpcQryDeliOrder(CReqKspbRpcQryDeliOrderField *p_pReqField, LONGLONG p_llRequestId);

private:
  // �û���½
  void OnRspRpcLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ί��
  void OnRspRpcOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ί�г���
  void OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɷݲ�ѯ
  void OnRspRpcQryStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʽ��ѯ
  void OnRspRpcQryFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ί�в�ѯ
  void OnRspRpcQryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɽ���ѯ
  void OnRspRpcQryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��Ϣ��ѯ
  void OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɶ���ѯ
  void OnRspRpcQryTrdacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ϣ��ѯ
  void OnRspRpcQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��֤ת��
  void OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ת����ˮ��ѯ
  void OnRspRpcQryBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷί�в�ѯ
  void OnRspRpcQryHistoryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷ�ɽ���ѯ
  void OnRspRpcQryHistoryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʽ�ɷ���ˮ��ѯ
  void OnRspRpcQryStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����ѯ
  void OnRspRpcQryDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_KSPB_TRADE_API_H__