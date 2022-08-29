#if !defined(__MA_CLI_FS2_TRADE_API_H__)
#define __MA_CLI_FS2_TRADE_API_H__

#include "maCliFs2Api.h"
#include "maCliFs2TradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFs2TradeSpi : virtual public CCliFs2Spi
{
public:
  // ȷ�ϻر�
  virtual int OnRtnOrderConfirm(CRtnFsOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // �ɽ��ر�
  virtual int OnRtnOrderFill(CRtnFsOrderFillField *p_pRtnOrderFillField) {return 0;}
  // ������Ϣ��ѯ��Ӧ
  virtual int OnRspQryOrgInfo(CFirstSetField *p_pFirstSetField, CRspFsQryOrgInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��Ϣ��ѯ��Ӧ
  virtual int OnRspQryTrdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryTrdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ֵ���Ϣ��ѯ��Ӧ
  virtual int OnRspQryDictInfo(CFirstSetField *p_pFirstSetField, CRspFsQryDictInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ϵͳ״̬��ѯ��Ӧ
  virtual int OnRspQrySysStat(CFirstSetField *p_pFirstSetField, CRspFsQrySysStatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��֤ת����Ӧ
  virtual int OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, CRspFsBankStkTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ȯ�̷�������������Ӧ
  virtual int OnRspQryInitiatorOrderBankBal(CFirstSetField *p_pFirstSetField, CRspFsQryInitiatorOrderBankBalField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ѯ��֤ҵ����ˮ��Ӧ
  virtual int OnRspQryBankStkDetail(CFirstSetField *p_pFirstSetField, CRspFsQryBankStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ����ֽ�ת��Ӧ
  virtual int OnRspCustCashTransfer(CFirstSetField *p_pFirstSetField, CRspFsCustCashTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �˻��ʽ�鼯��Ӧ
  virtual int OnRspQryAcctConcentration(CFirstSetField *p_pFirstSetField, CRspFsQryAcctConcentrationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ѯת�������ʻ���Ӧ
  virtual int OnRspQryTransferAcct(CFirstSetField *p_pFirstSetField, CRspFsQryTransferAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻��ʽ𻮲���Ӧ
  virtual int OnRspQryFortuneAcctTransfer(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻�������ˮ��ѯ��Ӧ
  virtual int OnRspQryFortuneAcctTransferDetail(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctTransferDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ���¼��Ӧ
  virtual int OnRspQryCustLogin(CFirstSetField *p_pFirstSetField, CRspFsQryCustLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʲ��˻���ѯ��Ӧ
  virtual int OnRspQryAssetsAcct(CFirstSetField *p_pFirstSetField, CRspFsQryAssetsAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻���ѯ��Ӧ
  virtual int OnRspQryFortuneAcct(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɶ���ѯ��Ӧ
  virtual int OnRspQrySecu(CFirstSetField *p_pFirstSetField, CRspFsQrySecuField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɶ�����Э���ѯ��Ӧ
  virtual int OnRspQrySecuTransferAgreement(CFirstSetField *p_pFirstSetField, CRspFsQrySecuTransferAgreementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʵ���ƥ������Ӧ
  virtual int OnRspPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ��ʵ��Բ�ѯ��Ӧ
  virtual int OnRspQryCustPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsQryCustPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʒ�ʵ��Բ�ѯ��Ӧ
  virtual int OnRspQryProdPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsQryProdPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʽ��ѯ��Ӧ
  virtual int OnRspQryFund(CFirstSetField *p_pFirstSetField, CRspFsQryFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ʲ���ѯ��Ӧ
  virtual int OnRspQryTotalAssets(CFirstSetField *p_pFirstSetField, CRspFsQryTotalAssetsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɷ��ʲ���ѯ��Ӧ
  virtual int OnRspQryShareAssets(CFirstSetField *p_pFirstSetField, CRspFsQryShareAssetsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ί�в�ѯ��Ӧ
  virtual int OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ճɽ���ѯ��Ӧ
  virtual int OnRspQryCurrDayMatch(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ճɽ�ͳ����Ӧ
  virtual int OnRspQryCurrDayMatchCount(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayMatchCountField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ɽ�������������Ӧ
  virtual int OnRspQryMaxLimitTransferCount(CFirstSetField *p_pFirstSetField, CRspFsQryMaxLimitTransferCountField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��������ί���걨��Ӧ
  virtual int OnRspTrdTransferOrder(CFirstSetField *p_pFirstSetField, CRspFsTrdTransferOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ����ί�г�����Ӧ
  virtual int OnRspTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, CRspFsTrdTransferPurchaseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɳ���ί�в�ѯ��Ӧ
  virtual int OnRspQryAvlTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, CRspFsQryAvlTrdTransferPurchaseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��������������Ӧ
  virtual int OnRspTrdQuantityCancel(CFirstSetField *p_pFirstSetField, CRspFsTrdQuantityCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���óɱ���Ӧ
  virtual int OnRspResetCost(CFirstSetField *p_pFirstSetField, CRspFsResetCostField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���гֲ���ϸ��ѯ��Ӧ
  virtual int OnRspQryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, CRspFsQryBondsPreIssuePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ��Ӧ
  virtual int OnRspQryCustBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, CRspFsQryCustBondsPreIssueClosePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���в�Ʒ��Ϣ��ѯ��Ӧ
  virtual int OnRspQryBondsPreProdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryBondsPreProdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���Ų�ѯ��Ӧ
  virtual int OnRspQryNewShareDistribution(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareDistributionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���ǩ��ѯ��Ӧ
  virtual int OnRspQryNewShareWinning(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareWinningField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹��깺��ֵ��Ȳ�ѯ��Ӧ
  virtual int OnRspQryNewShareSubscriptionLimit(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareSubscriptionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���Ϣ��ѯ��Ӧ
  virtual int OnRspQryNewShareInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�Ʒ����Ϣ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ����������Ϣ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseProductInfoConversionRate(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductInfoConversionRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ���ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseProduct(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Լչ����Ӧ
  virtual int OnRspQuoteRepurchaseContflag(CFirstSetField *p_pFirstSetField, CRspFsQuoteRepurchaseContflagField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ�ʽ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchasePledgeFund(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchasePledgeFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�ԤԼ��ˮ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseReservationDetail(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseReservationDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�ԤԼ��Ӧ
  virtual int OnRspQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, CRspFsQuoteRepurchaseReservationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�����ǰ���غ�Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseValPreRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValPreRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���չ��ȡ����Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseValFlagCancel(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValFlagCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseAutoValFlagCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAutoValFlagContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseAvlCancelStaTransferPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlCancelStaTransferPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���չ�ں�Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseValFlagCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValFlagContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQryQuoteRepurchaseAvlAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlAdvanceEndPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩֱ�ӻ�����Ӧ
  virtual int OnRspFinancingRightDirectRepayment(CFirstSetField *p_pFirstSetField, CRspFsFinancingRightDirectRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ��Լ��ѯ��Ӧ
  virtual int OnRspQryFinancingRightcontract(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightcontractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ�ɻ���ծ��ѯ��Ӧ
  virtual int OnRspQryFinancingRightCanrePayDebt(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightCanrePayDebtField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ������ϸ��ѯ��Ӧ
  virtual int OnRspQryFinancingRightPayDetail(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightPayDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ�ɶ������Ϣ��ѯ��Ӧ
  virtual int OnRspQryNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingTrdacctsConferenceInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱͶƱ�����ѯ��Ӧ
  virtual int OnRspQryNetworkVotingResult(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingResultField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ�鰸��Ϣ��ѯ��Ӧ
  virtual int OnRspQryNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingMotionInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ��Ʒ��Ϣ��ѯ��Ӧ
  virtual int OnRspQryNetworkVotingProdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingProdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�۲���Ϣ��ѯ��Ӧ
  virtual int OnRspQryHkCuaccDifferencet(CFirstSetField *p_pFirstSetField, CRspFsQryHkCuaccDifferencetField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ������Ϣ��ѯ��Ӧ
  virtual int OnRspQryHkRationInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkRationInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�г�״̬��Ϣ��ѯ��Ӧ
  virtual int OnRspQryHkShareMarketInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareMarketInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ���֤ȯ��Ϣ��ѯ��Ӧ
  virtual int OnRspQryHkShareCommTrdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareCommTrdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ����������ѯ��Ӧ
  virtual int OnRspQryHkShareTransferDate(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareTransferDateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�ʽ��ѯ��Ӧ
  virtual int OnRspQryHkShareFund(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ڽ���������Ϣ��ѯ��Ӧ
  virtual int OnRspQryBlockTradingMkt(CFirstSetField *p_pFirstSetField, CRspFsQryBlockTradingMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �̺������ѯ��Ӧ
  virtual int OnRspQrylosingMktSecondboard(CFirstSetField *p_pFirstSetField, CRspFsQrylosingMktSecondboardField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����Ͷ���߿ɽ���֤ȯ��ѯ��Ӧ
  virtual int OnRspQryRestrictedInvestorValTransferTrd(CFirstSetField *p_pFirstSetField, CRspFsQryRestrictedInvestorValTransferTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ת�ֲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQryStkTranLayeringInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranLayeringInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��תѯ��ȷ����Ϣ��ѯ��Ӧ
  virtual int OnRspQryStkTranInquiryConfirmInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranInquiryConfirmInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ת����ҵ����Ϣ��ѯ��Ӧ
  virtual int OnRspQryStkTranIssuingBusinessInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranIssuingBusinessInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ��Ӧ
  virtual int OnRspQryStkPledgeContract(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ��Ӧ
  virtual int OnRspQryStkPledgeContractDetail(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeContractDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ��Ӧ
  virtual int OnRspQryStkPledgeInitTrdReferAmt(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeInitTrdReferAmtField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ��Ӧ
  virtual int OnRspQryStkPledgeQuotaInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeQuotaInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ���֤ȯ��ѯ��Ӧ
  virtual int OnRspQryStkPledgeTrd(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ��Ӧ
  virtual int OnRspQryLenderStkPledgeQuota(CFirstSetField *p_pFirstSetField, CRspFsQryLenderStkPledgeQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ��Ӧ
  virtual int OnRspQryszBondPledgeRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryszBondPledgeRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ��Ӧ
  virtual int OnRspQryBondPledgeRepurchaseTrd(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeRepurchaseTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع������ѯ�걨��Ӧ
  virtual int OnRspshContRepurchaseMarket(CFirstSetField *p_pFirstSetField, CRspFsshContRepurchaseMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع��ǹ������������ѯ��Ӧ
  virtual int OnRspQryshContRepurchaseClsMarket(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseClsMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع��������������ѯ��Ӧ
  virtual int OnRspQryshContRepurchaseOpenMarket(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseOpenMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع�δ����Э���ѯ��Ӧ
  virtual int OnRspQryshContRepurchaseUnsettledBill(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseUnsettledBillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع���Լ��ѯ��Ӧ
  virtual int OnRspQryshContRepurchaseContract(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ծȯЭ��ع����ؽ���ѯ��Ӧ
  virtual int OnRspQryszBondContRepurchaseQuota(CFirstSetField *p_pFirstSetField, CRspFsQryszBondContRepurchaseQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��Ѻ�ɷ���ϸ��ѯ��Ӧ
  virtual int OnRspQryBondPledgeStkDetail(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��Ѻʽ�ع���Լ��ѯ��Ӧ
  virtual int OnRspQryBondPledgeStkRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeStkRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�����Ϲ����볷����Ӧ
  virtual int OnRspETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, CRspFsETFOfflineApplicationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF����ί����Ӧ
  virtual int OnRspETFOrder(CFirstSetField *p_pFirstSetField, CRspFsETFOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�����Ϲ������ѯ��Ӧ
  virtual int OnRspQryETFOfflineApplication(CFirstSetField *p_pFirstSetField, CRspFsQryETFOfflineApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�ɷֹɳֲֲ�ѯ��Ӧ
  virtual int OnRspQryETFCotPosition(CFirstSetField *p_pFirstSetField, CRspFsQryETFCotPositionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF��Ϣ��ѯ��Ӧ
  virtual int OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, CRspFsQryEtfInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQryshLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspFsQryshLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQryszLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspFsQryszLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ȩ��Ա��Ϣ��ѯ��Ӧ
  virtual int OnRspQrySecurInfo(CFirstSetField *p_pFirstSetField, CRspFsQrySecurInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����˰������Ϣά����Ӧ
  virtual int OnRspQryTaxInfoMaintenance(CFirstSetField *p_pFirstSetField, CRspFsQryTaxInfoMaintenanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ҪԼ�չ�ת����ϵ���ѯ��Ӧ
  virtual int OnRspQryTenderSgConverRelat(CFirstSetField *p_pFirstSetField, CRspFsQryTenderSgConverRelatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ����ֹɷݶ�Ȳ�ѯ��Ӧ
  virtual int OnRspQryshReduceStkQuota(CFirstSetField *p_pFirstSetField, CRspFsQryshReduceStkQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ڼ��ֹɷݶ�Ȳ�ѯ��Ӧ
  virtual int OnRspQryszReduceStkQuota(CFirstSetField *p_pFirstSetField, CRspFsQryszReduceStkQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // Ȩ֤��Ϣ��ѯ��Ӧ
  virtual int OnRspQryWarrantInfo(CFirstSetField *p_pFirstSetField, CRspFsQryWarrantInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����ع�������Ϣ��ѯ��Ӧ
  virtual int OnRspQryThreeRePurchaseInfo(CFirstSetField *p_pFirstSetField, CRspFsQryThreeRePurchaseInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����ع���Ѻ���ѯ��Ӧ
  virtual int OnRspQryThreeRePurchasepledge(CFirstSetField *p_pFirstSetField, CRspFsQryThreeRePurchasepledgeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع�ת���ɽ���Ϣ���ѯ��Ӧ
  virtual int OnRspQryszThreeRePurchaseMatchInfo(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseMatchInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع���Լ�ܱ��ѯ��Ӧ
  virtual int OnRspQryszThreeRePurchaseContTotal(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseContTotalField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع���Լ��ϸ��ѯ��Ӧ
  virtual int OnRspQryszThreeRePurchaseContDetail(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseContDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ��Ӧ
  virtual int OnRspQryHStkFullCirAcctInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHStkFullCirAcctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ��Ӧ
  virtual int OnRspQryHStkForTrdIdCorrespond(CFirstSetField *p_pFirstSetField, CRspFsQryHStkForTrdIdCorrespondField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ѯ�۽��׷ǹ������������ѯ��Ӧ
  virtual int OnRspQryInquireTransferCloseMarketInfo(CFirstSetField *p_pFirstSetField, CRspFsQryInquireTransferCloseMarketInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷ�ɽ���ѯ��Ӧ
  virtual int OnRspQryHisFill(CFirstSetField *p_pFirstSetField, CRspFsQryHisFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷί�в�ѯ��Ӧ
  virtual int OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, CRspFsQryHisOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷ�����ѯ��Ӧ
  virtual int OnRspQryHisDeliOrder(CFirstSetField *p_pFirstSetField, CRspFsQryHisDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliFs2TradeApi : virtual public CCliFs2Api
{
public:
  // Ĭ�Ϲ��캯��
  CCliFs2TradeApi(void);

  // ��������
  virtual ~CCliFs2TradeApi(void);

  // ������Ϣ��ѯ����
  virtual int ReqQryOrgInfo(CReqFsQryOrgInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ��Ϣ��ѯ����
  virtual int ReqQryTrdInfo(CReqFsQryTrdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �ֵ���Ϣ��ѯ����
  virtual int ReqQryDictInfo(CReqFsQryDictInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ϵͳ״̬��ѯ����
  virtual int ReqQrySysStat(CReqFsQrySysStatField *p_pReqField, LONGLONG p_llRequestId);
  // ��֤ת������
  virtual int ReqBankStkTransfer(CReqFsBankStkTransferField *p_pReqField, LONGLONG p_llRequestId);
  // ȯ�̷���������������
  virtual int ReqQryInitiatorOrderBankBal(CReqFsQryInitiatorOrderBankBalField *p_pReqField, LONGLONG p_llRequestId);
  // ��ѯ��֤ҵ����ˮ����
  virtual int ReqQryBankStkDetail(CReqFsQryBankStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ����ֽ�ת����
  virtual int ReqCustCashTransfer(CReqFsCustCashTransferField *p_pReqField, LONGLONG p_llRequestId);
  // �˻��ʽ�鼯����
  virtual int ReqQryAcctConcentration(CReqFsQryAcctConcentrationField *p_pReqField, LONGLONG p_llRequestId);
  // ��ѯת�������ʻ�����
  virtual int ReqQryTransferAcct(CReqFsQryTransferAcctField *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻��ʽ𻮲�����
  virtual int ReqQryFortuneAcctTransfer(CReqFsQryFortuneAcctTransferField *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻�������ˮ��ѯ����
  virtual int ReqQryFortuneAcctTransferDetail(CReqFsQryFortuneAcctTransferDetailField *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ���¼����
  virtual int ReqQryCustLogin(CReqFsQryCustLoginField *p_pReqField, LONGLONG p_llRequestId);
  // �ʲ��˻���ѯ����
  virtual int ReqQryAssetsAcct(CReqFsQryAssetsAcctField *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻���ѯ����
  virtual int ReqQryFortuneAcct(CReqFsQryFortuneAcctField *p_pReqField, LONGLONG p_llRequestId);
  // �ɶ���ѯ����
  virtual int ReqQrySecu(CReqFsQrySecuField *p_pReqField, LONGLONG p_llRequestId);
  // �ɶ�����Э���ѯ����
  virtual int ReqQrySecuTransferAgreement(CReqFsQrySecuTransferAgreementField *p_pReqField, LONGLONG p_llRequestId);
  // �ʵ���ƥ��������
  virtual int ReqPropriatenceCheck(CReqFsPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ��ʵ��Բ�ѯ����
  virtual int ReqQryCustPropriatenceCheck(CReqFsQryCustPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʒ�ʵ��Բ�ѯ����
  virtual int ReqQryProdPropriatenceCheck(CReqFsQryProdPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // �ʽ��ѯ����
  virtual int ReqQryFund(CReqFsQryFundField *p_pReqField, LONGLONG p_llRequestId);
  // ���ʲ���ѯ����
  virtual int ReqQryTotalAssets(CReqFsQryTotalAssetsField *p_pReqField, LONGLONG p_llRequestId);
  // �ɷ��ʲ���ѯ����
  virtual int ReqQryShareAssets(CReqFsQryShareAssetsField *p_pReqField, LONGLONG p_llRequestId);
  // ����ί�в�ѯ����
  virtual int ReqQryCurrDayOrder(CReqFsQryCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ���ճɽ���ѯ����
  virtual int ReqQryCurrDayMatch(CReqFsQryCurrDayMatchField *p_pReqField, LONGLONG p_llRequestId);
  // ���ճɽ�ͳ������
  virtual int ReqQryCurrDayMatchCount(CReqFsQryCurrDayMatchCountField *p_pReqField, LONGLONG p_llRequestId);
  // ���ɽ���������������
  virtual int ReqQryMaxLimitTransferCount(CReqFsQryMaxLimitTransferCountField *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ��������ί���걨����
  virtual int ReqTrdTransferOrder(CReqFsTrdTransferOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ����ί�г�������
  virtual int ReqTrdTransferPurchaseCancel(CReqFsTrdTransferPurchaseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // �ɳ���ί�в�ѯ����
  virtual int ReqQryAvlTrdTransferPurchaseCancel(CReqFsQryAvlTrdTransferPurchaseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ����������������
  virtual int ReqTrdQuantityCancel(CReqFsTrdQuantityCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ���óɱ�����
  virtual int ReqResetCost(CReqFsResetCostField *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���гֲ���ϸ��ѯ����
  virtual int ReqQryBondsPreIssuePosi(CReqFsQryBondsPreIssuePosiField *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ����
  virtual int ReqQryCustBondsPreIssueClosePosi(CReqFsQryCustBondsPreIssueClosePosiField *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���в�Ʒ��Ϣ��ѯ����
  virtual int ReqQryBondsPreProdInfo(CReqFsQryBondsPreProdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �¹���Ų�ѯ����
  virtual int ReqQryNewShareDistribution(CReqFsQryNewShareDistributionField *p_pReqField, LONGLONG p_llRequestId);
  // �¹���ǩ��ѯ����
  virtual int ReqQryNewShareWinning(CReqFsQryNewShareWinningField *p_pReqField, LONGLONG p_llRequestId);
  // �¹��깺��ֵ��Ȳ�ѯ����
  virtual int ReqQryNewShareSubscriptionLimit(CReqFsQryNewShareSubscriptionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // �¹���Ϣ��ѯ����
  virtual int ReqQryNewShareInfo(CReqFsQryNewShareInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�Ʒ����Ϣ��ѯ����
  virtual int ReqQryQuoteRepurchaseProductInfo(CReqFsQryQuoteRepurchaseProductInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ����������Ϣ��ѯ����
  virtual int ReqQryQuoteRepurchaseProductInfoConversionRate(CReqFsQryQuoteRepurchaseProductInfoConversionRateField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ���ѯ����
  virtual int ReqQryQuoteRepurchaseProduct(CReqFsQryQuoteRepurchaseProductField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Լչ������
  virtual int ReqQuoteRepurchaseContflag(CReqFsQuoteRepurchaseContflagField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ�ʽ��ѯ����
  virtual int ReqQryQuoteRepurchasePledgeFund(CReqFsQryQuoteRepurchasePledgeFundField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�ԤԼ��ˮ��ѯ����
  virtual int ReqQryQuoteRepurchaseReservationDetail(CReqFsQryQuoteRepurchaseReservationDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�ԤԼ����
  virtual int ReqQuoteRepurchaseReservation(CReqFsQuoteRepurchaseReservationField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�����ǰ���غ�Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseValPreRepurchaseCont(CReqFsQryQuoteRepurchaseValPreRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���չ��ȡ����Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseValFlagCancel(CReqFsQryQuoteRepurchaseValFlagCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseAutoValFlagCont(CReqFsQryQuoteRepurchaseAutoValFlagContField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CReqFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseAvlCancelStaTransferPreCont(CReqFsQryQuoteRepurchaseAvlCancelStaTransferPreContField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���չ�ں�Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseValFlagCont(CReqFsQryQuoteRepurchaseValFlagContField *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ����
  virtual int ReqQryQuoteRepurchaseAvlAdvanceEndPreCont(CReqFsQryQuoteRepurchaseAvlAdvanceEndPreContField *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩֱ�ӻ�������
  virtual int ReqFinancingRightDirectRepayment(CReqFsFinancingRightDirectRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ��Լ��ѯ����
  virtual int ReqQryFinancingRightcontract(CReqFsQryFinancingRightcontractField *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ�ɻ���ծ��ѯ����
  virtual int ReqQryFinancingRightCanrePayDebt(CReqFsQryFinancingRightCanrePayDebtField *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ������ϸ��ѯ����
  virtual int ReqQryFinancingRightPayDetail(CReqFsQryFinancingRightPayDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ�ɶ������Ϣ��ѯ����
  virtual int ReqQryNetworkVotingTrdacctsConferenceInfo(CReqFsQryNetworkVotingTrdacctsConferenceInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱͶƱ�����ѯ����
  virtual int ReqQryNetworkVotingResult(CReqFsQryNetworkVotingResultField *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ�鰸��Ϣ��ѯ����
  virtual int ReqQryNetworkVotingMotionInfo(CReqFsQryNetworkVotingMotionInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ��Ʒ��Ϣ��ѯ����
  virtual int ReqQryNetworkVotingProdInfo(CReqFsQryNetworkVotingProdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�۲���Ϣ��ѯ����
  virtual int ReqQryHkCuaccDifferencet(CReqFsQryHkCuaccDifferencetField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ������Ϣ��ѯ����
  virtual int ReqQryHkRationInfo(CReqFsQryHkRationInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�г�״̬��Ϣ��ѯ����
  virtual int ReqQryHkShareMarketInfo(CReqFsQryHkShareMarketInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ���֤ȯ��Ϣ��ѯ����
  virtual int ReqQryHkShareCommTrdInfo(CReqFsQryHkShareCommTrdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ����������ѯ����
  virtual int ReqQryHkShareTransferDate(CReqFsQryHkShareTransferDateField *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�ʽ��ѯ����
  virtual int ReqQryHkShareFund(CReqFsQryHkShareFundField *p_pReqField, LONGLONG p_llRequestId);
  // ���ڽ���������Ϣ��ѯ����
  virtual int ReqQryBlockTradingMkt(CReqFsQryBlockTradingMktField *p_pReqField, LONGLONG p_llRequestId);
  // �̺������ѯ����
  virtual int ReqQrylosingMktSecondboard(CReqFsQrylosingMktSecondboardField *p_pReqField, LONGLONG p_llRequestId);
  // ����Ͷ���߿ɽ���֤ȯ��ѯ����
  virtual int ReqQryRestrictedInvestorValTransferTrd(CReqFsQryRestrictedInvestorValTransferTrdField *p_pReqField, LONGLONG p_llRequestId);
  // ��ת�ֲ���Ϣ��ѯ����
  virtual int ReqQryStkTranLayeringInfo(CReqFsQryStkTranLayeringInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��תѯ��ȷ����Ϣ��ѯ����
  virtual int ReqQryStkTranInquiryConfirmInfo(CReqFsQryStkTranInquiryConfirmInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��ת����ҵ����Ϣ��ѯ����
  virtual int ReqQryStkTranIssuingBusinessInfo(CReqFsQryStkTranIssuingBusinessInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ����
  virtual int ReqQryStkPledgeContract(CReqFsQryStkPledgeContractField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ����
  virtual int ReqQryStkPledgeContractDetail(CReqFsQryStkPledgeContractDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ����
  virtual int ReqQryStkPledgeInitTrdReferAmt(CReqFsQryStkPledgeInitTrdReferAmtField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ����
  virtual int ReqQryStkPledgeQuotaInfo(CReqFsQryStkPledgeQuotaInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ���֤ȯ��ѯ����
  virtual int ReqQryStkPledgeTrd(CReqFsQryStkPledgeTrdField *p_pReqField, LONGLONG p_llRequestId);
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ����
  virtual int ReqQryLenderStkPledgeQuota(CReqFsQryLenderStkPledgeQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ����
  virtual int ReqQryszBondPledgeRepurchaseCont(CReqFsQryszBondPledgeRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ����
  virtual int ReqQryBondPledgeRepurchaseTrd(CReqFsQryBondPledgeRepurchaseTrdField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع������ѯ�걨����
  virtual int ReqshContRepurchaseMarket(CReqFsshContRepurchaseMarketField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع��ǹ������������ѯ����
  virtual int ReqQryshContRepurchaseClsMarket(CReqFsQryshContRepurchaseClsMarketField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع��������������ѯ����
  virtual int ReqQryshContRepurchaseOpenMarket(CReqFsQryshContRepurchaseOpenMarketField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع�δ����Э���ѯ����
  virtual int ReqQryshContRepurchaseUnsettledBill(CReqFsQryshContRepurchaseUnsettledBillField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع���Լ��ѯ����
  virtual int ReqQryshContRepurchaseContract(CReqFsQryshContRepurchaseContractField *p_pReqField, LONGLONG p_llRequestId);
  // ����ծȯЭ��ع����ؽ���ѯ����
  virtual int ReqQryszBondContRepurchaseQuota(CReqFsQryszBondContRepurchaseQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��Ѻ�ɷ���ϸ��ѯ����
  virtual int ReqQryBondPledgeStkDetail(CReqFsQryBondPledgeStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��Ѻʽ�ع���Լ��ѯ����
  virtual int ReqQryBondPledgeStkRepurchaseCont(CReqFsQryBondPledgeStkRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // ETF�����Ϲ����볷������
  virtual int ReqETFOfflineApplicationCancel(CReqFsETFOfflineApplicationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ETF����ί������
  virtual int ReqETFOrder(CReqFsETFOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ETF�����Ϲ������ѯ����
  virtual int ReqQryETFOfflineApplication(CReqFsQryETFOfflineApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // ETF�ɷֹɳֲֲ�ѯ����
  virtual int ReqQryETFCotPosition(CReqFsQryETFCotPositionField *p_pReqField, LONGLONG p_llRequestId);
  // ETF��Ϣ��ѯ����
  virtual int ReqQryEtfInfo(CReqFsQryEtfInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ����
  virtual int ReqQryshLOFFundSplitMerge(CReqFsQryshLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // ����LOF����ֲ�ϲ���Ϣ��ѯ����
  virtual int ReqQryszLOFFundSplitMerge(CReqFsQryszLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // ��Ȩ��Ա��Ϣ��ѯ����
  virtual int ReqQrySecurInfo(CReqFsQrySecurInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ����˰������Ϣά������
  virtual int ReqQryTaxInfoMaintenance(CReqFsQryTaxInfoMaintenanceField *p_pReqField, LONGLONG p_llRequestId);
  // ҪԼ�չ�ת����ϵ���ѯ����
  virtual int ReqQryTenderSgConverRelat(CReqFsQryTenderSgConverRelatField *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ����ֹɷݶ�Ȳ�ѯ����
  virtual int ReqQryshReduceStkQuota(CReqFsQryshReduceStkQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // ���ڼ��ֹɷݶ�Ȳ�ѯ����
  virtual int ReqQryszReduceStkQuota(CReqFsQryszReduceStkQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // Ȩ֤��Ϣ��ѯ����
  virtual int ReqQryWarrantInfo(CReqFsQryWarrantInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �����ع�������Ϣ��ѯ����
  virtual int ReqQryThreeRePurchaseInfo(CReqFsQryThreeRePurchaseInfoField *p_pReqField, LONGLONG p_llRequestId);
  // �����ع���Ѻ���ѯ����
  virtual int ReqQryThreeRePurchasepledge(CReqFsQryThreeRePurchasepledgeField *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع�ת���ɽ���Ϣ���ѯ����
  virtual int ReqQryszThreeRePurchaseMatchInfo(CReqFsQryszThreeRePurchaseMatchInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع���Լ�ܱ��ѯ����
  virtual int ReqQryszThreeRePurchaseContTotal(CReqFsQryszThreeRePurchaseContTotalField *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع���Լ��ϸ��ѯ����
  virtual int ReqQryszThreeRePurchaseContDetail(CReqFsQryszThreeRePurchaseContDetailField *p_pReqField, LONGLONG p_llRequestId);
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ����
  virtual int ReqQryHStkFullCirAcctInfo(CReqFsQryHStkFullCirAcctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ����
  virtual int ReqQryHStkForTrdIdCorrespond(CReqFsQryHStkForTrdIdCorrespondField *p_pReqField, LONGLONG p_llRequestId);
  // ѯ�۽��׷ǹ������������ѯ����
  virtual int ReqQryInquireTransferCloseMarketInfo(CReqFsQryInquireTransferCloseMarketInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷ�ɽ���ѯ����
  virtual int ReqQryHisFill(CReqFsQryHisFillField *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷί�в�ѯ����
  virtual int ReqQryHisOrder(CReqFsQryHisOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷ�����ѯ����
  virtual int ReqQryHisDeliOrder(CReqFsQryHisDeliOrderField *p_pReqField, LONGLONG p_llRequestId);

public:
  // �첽�ص�����
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // �����ص�����
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // ������Ϣ��ѯ��Ӧ
  void OnRspQryOrgInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��Ϣ��ѯ��Ӧ
  void OnRspQryTrdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ֵ���Ϣ��ѯ��Ӧ
  void OnRspQryDictInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ϵͳ״̬��ѯ��Ӧ
  void OnRspQrySysStat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��֤ת����Ӧ
  void OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ȯ�̷�������������Ӧ
  void OnRspQryInitiatorOrderBankBal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ѯ��֤ҵ����ˮ��Ӧ
  void OnRspQryBankStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ����ֽ�ת��Ӧ
  void OnRspCustCashTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �˻��ʽ�鼯��Ӧ
  void OnRspQryAcctConcentration(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ѯת�������ʻ���Ӧ
  void OnRspQryTransferAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻��ʽ𻮲���Ӧ
  void OnRspQryFortuneAcctTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻�������ˮ��ѯ��Ӧ
  void OnRspQryFortuneAcctTransferDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ���¼��Ӧ
  void OnRspQryCustLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʲ��˻���ѯ��Ӧ
  void OnRspQryAssetsAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻���ѯ��Ӧ
  void OnRspQryFortuneAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɶ���ѯ��Ӧ
  void OnRspQrySecu(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɶ�����Э���ѯ��Ӧ
  void OnRspQrySecuTransferAgreement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʵ���ƥ������Ӧ
  void OnRspPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ��ʵ��Բ�ѯ��Ӧ
  void OnRspQryCustPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʒ�ʵ��Բ�ѯ��Ӧ
  void OnRspQryProdPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʽ��ѯ��Ӧ
  void OnRspQryFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ʲ���ѯ��Ӧ
  void OnRspQryTotalAssets(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɷ��ʲ���ѯ��Ӧ
  void OnRspQryShareAssets(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ί�в�ѯ��Ӧ
  void OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ճɽ���ѯ��Ӧ
  void OnRspQryCurrDayMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ճɽ�ͳ����Ӧ
  void OnRspQryCurrDayMatchCount(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ɽ�������������Ӧ
  void OnRspQryMaxLimitTransferCount(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��������ί���걨��Ӧ
  void OnRspTrdTransferOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ����ί�г�����Ӧ
  void OnRspTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɳ���ί�в�ѯ��Ӧ
  void OnRspQryAvlTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��������������Ӧ
  void OnRspTrdQuantityCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���óɱ���Ӧ
  void OnRspResetCost(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���гֲ���ϸ��ѯ��Ӧ
  void OnRspQryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ��Ӧ
  void OnRspQryCustBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���в�Ʒ��Ϣ��ѯ��Ӧ
  void OnRspQryBondsPreProdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���Ų�ѯ��Ӧ
  void OnRspQryNewShareDistribution(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���ǩ��ѯ��Ӧ
  void OnRspQryNewShareWinning(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹��깺��ֵ��Ȳ�ѯ��Ӧ
  void OnRspQryNewShareSubscriptionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���Ϣ��ѯ��Ӧ
  void OnRspQryNewShareInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�Ʒ����Ϣ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ����������Ϣ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseProductInfoConversionRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ���ѯ��Ӧ
  void OnRspQryQuoteRepurchaseProduct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Լչ����Ӧ
  void OnRspQuoteRepurchaseContflag(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ�ʽ��ѯ��Ӧ
  void OnRspQryQuoteRepurchasePledgeFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�ԤԼ��ˮ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseReservationDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�ԤԼ��Ӧ
  void OnRspQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�����ǰ���غ�Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseValPreRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���չ��ȡ����Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseValFlagCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseAutoValFlagCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseAvlCancelStaTransferPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���չ�ں�Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseValFlagCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  void OnRspQryQuoteRepurchaseAvlAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩֱ�ӻ�����Ӧ
  void OnRspFinancingRightDirectRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ��Լ��ѯ��Ӧ
  void OnRspQryFinancingRightcontract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ�ɻ���ծ��ѯ��Ӧ
  void OnRspQryFinancingRightCanrePayDebt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ������ϸ��ѯ��Ӧ
  void OnRspQryFinancingRightPayDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ�ɶ������Ϣ��ѯ��Ӧ
  void OnRspQryNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱͶƱ�����ѯ��Ӧ
  void OnRspQryNetworkVotingResult(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ�鰸��Ϣ��ѯ��Ӧ
  void OnRspQryNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ��Ʒ��Ϣ��ѯ��Ӧ
  void OnRspQryNetworkVotingProdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�۲���Ϣ��ѯ��Ӧ
  void OnRspQryHkCuaccDifferencet(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ������Ϣ��ѯ��Ӧ
  void OnRspQryHkRationInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�г�״̬��Ϣ��ѯ��Ӧ
  void OnRspQryHkShareMarketInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ���֤ȯ��Ϣ��ѯ��Ӧ
  void OnRspQryHkShareCommTrdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ����������ѯ��Ӧ
  void OnRspQryHkShareTransferDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�ʽ��ѯ��Ӧ
  void OnRspQryHkShareFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ڽ���������Ϣ��ѯ��Ӧ
  void OnRspQryBlockTradingMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �̺������ѯ��Ӧ
  void OnRspQrylosingMktSecondboard(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����Ͷ���߿ɽ���֤ȯ��ѯ��Ӧ
  void OnRspQryRestrictedInvestorValTransferTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ת�ֲ���Ϣ��ѯ��Ӧ
  void OnRspQryStkTranLayeringInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��תѯ��ȷ����Ϣ��ѯ��Ӧ
  void OnRspQryStkTranInquiryConfirmInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ת����ҵ����Ϣ��ѯ��Ӧ
  void OnRspQryStkTranIssuingBusinessInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ��Ӧ
  void OnRspQryStkPledgeContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ��Ӧ
  void OnRspQryStkPledgeContractDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ��Ӧ
  void OnRspQryStkPledgeInitTrdReferAmt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ��Ӧ
  void OnRspQryStkPledgeQuotaInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ���֤ȯ��ѯ��Ӧ
  void OnRspQryStkPledgeTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ��Ӧ
  void OnRspQryLenderStkPledgeQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ��Ӧ
  void OnRspQryszBondPledgeRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ��Ӧ
  void OnRspQryBondPledgeRepurchaseTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع������ѯ�걨��Ӧ
  void OnRspshContRepurchaseMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع��ǹ������������ѯ��Ӧ
  void OnRspQryshContRepurchaseClsMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع��������������ѯ��Ӧ
  void OnRspQryshContRepurchaseOpenMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع�δ����Э���ѯ��Ӧ
  void OnRspQryshContRepurchaseUnsettledBill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع���Լ��ѯ��Ӧ
  void OnRspQryshContRepurchaseContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ծȯЭ��ع����ؽ���ѯ��Ӧ
  void OnRspQryszBondContRepurchaseQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��Ѻ�ɷ���ϸ��ѯ��Ӧ
  void OnRspQryBondPledgeStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��Ѻʽ�ع���Լ��ѯ��Ӧ
  void OnRspQryBondPledgeStkRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�����Ϲ����볷����Ӧ
  void OnRspETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF����ί����Ӧ
  void OnRspETFOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�����Ϲ������ѯ��Ӧ
  void OnRspQryETFOfflineApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�ɷֹɳֲֲ�ѯ��Ӧ
  void OnRspQryETFCotPosition(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF��Ϣ��ѯ��Ӧ
  void OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  void OnRspQryshLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  void OnRspQryszLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ȩ��Ա��Ϣ��ѯ��Ӧ
  void OnRspQrySecurInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����˰������Ϣά����Ӧ
  void OnRspQryTaxInfoMaintenance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ҪԼ�չ�ת����ϵ���ѯ��Ӧ
  void OnRspQryTenderSgConverRelat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ����ֹɷݶ�Ȳ�ѯ��Ӧ
  void OnRspQryshReduceStkQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ڼ��ֹɷݶ�Ȳ�ѯ��Ӧ
  void OnRspQryszReduceStkQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // Ȩ֤��Ϣ��ѯ��Ӧ
  void OnRspQryWarrantInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����ع�������Ϣ��ѯ��Ӧ
  void OnRspQryThreeRePurchaseInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����ع���Ѻ���ѯ��Ӧ
  void OnRspQryThreeRePurchasepledge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع�ת���ɽ���Ϣ���ѯ��Ӧ
  void OnRspQryszThreeRePurchaseMatchInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع���Լ�ܱ��ѯ��Ӧ
  void OnRspQryszThreeRePurchaseContTotal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع���Լ��ϸ��ѯ��Ӧ
  void OnRspQryszThreeRePurchaseContDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ��Ӧ
  void OnRspQryHStkFullCirAcctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ��Ӧ
  void OnRspQryHStkForTrdIdCorrespond(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ѯ�۽��׷ǹ������������ѯ��Ӧ
  void OnRspQryInquireTransferCloseMarketInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷ�ɽ���ѯ��Ӧ
  void OnRspQryHisFill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷί�в�ѯ��Ӧ
  void OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷ�����ѯ��Ӧ
  void OnRspQryHisDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FS2_TRADE_API_H__