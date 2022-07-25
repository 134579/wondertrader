#if !defined(__MA_CLI_KSPB_TRADE_API_STRUCT_H__)
#define __MA_CLI_KSPB_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10101530:�û���½--------------------------
struct MATRADEAPI CReqKspbRpcLoginField
{
  char          chInputtype;                // ��½����
  char          szInputid[64 + 1];          // ��½��ʶ
  char          szProcName[128 + 1];        // ��������������
  char          szCert[4096 + 1];           // ֤��
  char          chCerttype;                 // ֤������
  char          chUserrole;                 // �û���ɫ
  char          szRandcode[32 + 1];         // �����
  char          szSignedrandcode[4096 + 1]; // ǩ��������
  char          szEtokenpin[33 + 1];        // ��̬����pin��
  char          szDynpwd[33 + 1];           // ��̬����
  char          szLoginsite[64 + 1];        // ί��վ��
  char          szLoginip[64 + 1];          // ��¼����IP
  char          szMac[32 + 1];              // ��¼����MAC��ַ
  char          szCpusn[32 + 1];            // ��¼����cpu���к�
  char          szHddsn[32 + 1];            // ��¼����Ӳ�����к�
  char          chCheckauthflag;            // ��ȫ��֤����־
};
struct MATRADEAPI CRspKspbRpcLoginField
{
  char          chCustprop;             // �ͻ�����
  char          chMarket;               // �����г�
  char          szSecuid[10 + 1];       // �ɶ�����
  char          szName[16 + 1];         // �ɶ�����
  LONGLONG      llFundid;               // ȱʡ�ʽ��ʻ�
  LONGLONG      llCustid;               // �ͻ�����
  char          szCustname[16 + 1];     // �ͻ�����
  char          szOrgid[4 + 1];         // ��������
  char          szBankcode[4 + 1];      // ���д���
  char          szIdentitysign[64 + 1]; // ����ǩ��
  char          chTimeoutflag;          // ��ʱ����
  char          chAuthlevel;            // ��֤��ʽ/����
  int           iPwderrtimes;           // ��½�������
  char          chSingleflag;           // �ͻ���־
  char          chCheckpwdflag;         // ������Ч��־
  char          szCustcert[128 + 1];    // �ͻ�֤��
  char          szTokenlen[8 + 1];      // ��¼ʱ����Ķ�̬���Ƴ���
  char          szLastlogindate[8 + 1]; // �����¼����
  char          szLastlogintime[8 + 1]; // �����¼ʱ��
  char          szLastloginip[64 + 1];  // �����¼IP
  char          szLastloginmac[32 + 1]; // �����¼MAC
  char          chInputtype;            // ��½����
  char          szInputid[64 + 1];      // ��½��ʶ
  char          szTokenenddate[8 + 1];  // �ͻ���̬���ƽ�������
  char          chBindflag;             // Ӳ������Ϣ��ʶ
};

//-------------------------------10101573:����ί��--------------------------
struct MATRADEAPI CReqKspbRpcOrderField
{
  char          chMarket;             // �����г�
  char          szSecuid[10 + 1];     // �ɶ�����
  LONGLONG      llFundid;             // �ʽ��˻�
  char          szStkcode[8 + 1];     // ֤ȯ����
  char          szBsflag[2 + 1];      // �������
  char          szPrice[21 + 1];      // �۸�
  int           iQty;                 // ����
  int           iOrdergroup;          // ί������
  char          szBankcode[4 + 1];    // �ⲿ����
  char          chCreditid;           // ���ò�Ʒ��ʶ
  char          chCreditflag;         // ����ί������
  char          szRemark[64 + 1];     // ��ע��Ϣ
  char          szTargetseat[6 + 1];  // �Է�ϯλ
  int           iPromiseno;           // Լ����
  int           iRisksno;             // ���յ�����ˮ��
  char          chAutoflag;           // �Զ�չ�ڱ�־
  int           iEnddate;             // չ����ֹ����
  char          szLinkman[12 + 1];    // ��ϵ��
  char          szLinkway[30 + 1];    // ��ϵ��ʽ
  char          chLinkmarket;         // �����г�
  char          szLinksecuid[10 + 1]; // �����ɶ�
  char          szSorttype[3 + 1];    // Ʒ�����
  int           iMergematchcode;      // �ϲ�����ĳ�ʼ���׵ĳɽ����
  int           iMergematchdate;      // �ϲ�����ĳ�ʼ���׵ĳɽ�����
  char          szOldorderid[24 + 1]; // ԭ��ͬ���
  char          szProdcode[12 + 1];   // ��Ʒ����
  char          chPricetype;          // ��������
  char          chBlackflag;          // �Ƿ������������֤ȯ
  char          chDzsaletype;         // ��������
  LONGLONG      llRisksignsno;        // ���ս�ʾǩ����ˮ��
  int           iCuacctSn;            // �˻����
};
struct MATRADEAPI CRspKspbRpcOrderField
{
  int           iOrdersno;         // ί�����
  char          szOrderid[10 + 1]; // ��ͬ���
  int           iOrdergroup;       // ί������
};

//-------------------------------10101574:ί�г���--------------------------
struct MATRADEAPI CReqKspbRpcOrderCancelField
{
  int           iOrderdate;        // ί������
  LONGLONG      llFundid;          // �ʽ��ʻ�
  int           iOrdersno;         // ί�����
  char          szBsflag[2 + 1];   // �������
  int           iCuacctSn;         // �˻����
};
struct MATRADEAPI CRspKspbRpcOrderCancelField
{
  char          szMsgok[32 + 1];   // �ɹ���Ϣ
  char          chCancelStatus;    // �ڲ�������־
  int           iOrdersno;         // ����ί�����
};

//-------------------------------10101679:�ɷݲ�ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryStkDetailField
{
  char          chMarket;          // �����г�
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szStkcode[8 + 1];  // ֤ȯ����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
};
struct MATRADEAPI CRspKspbRpcQryStkDetailField
{
  char          szPoststr[32 + 1];     // ��λ��
  LONGLONG      llCustid;              // �ͻ�����
  LONGLONG      llFundid;              // �ʽ��˻�
  char          chMarket;              // �����г�
  char          szSecuid[10 + 1];      // �ɶ�����
  char          szStkname[8 + 1];      // ֤ȯ����
  char          szStkcode[8 + 1];      // ֤ȯ����
  char          szOrgid[4 + 1];        // ��������
  char          chMoneytype;           // ����
  int           iStkbal;               // �ɷ����
  int           iStkavl;               // �ɷݿ���
  char          szBuycost[21 + 1];     // ��ǰ�ɱ�
  char          szCostprice[21 + 1];   // �ɱ��۸�
  char          szMktval[21 + 1];      // ��ֵ
  char          szIncome[21 + 1];      // ӯ��
  char          szProincome[21 + 1];   // �ο�ӯ��
  char          chMtkcalflag;          // ��ֵ�����ʶ
  int           iStkqty;               // ��ǰӵ����
  char          szLastprice[21 + 1];   // ���¼۸�
  char          chStktype;             // ֤ȯ����
  char          szProfitcost[21 + 1];  // �ο��ɱ�
  char          szProfitprice[21 + 1]; // �ο��ɱ���
  LONGLONG      llStkbuy;              // �ɷ�����ⶳ
  LONGLONG      llStksale;             // �ɷ���������
  LONGLONG      llStkdiff;             // ����������
  LONGLONG      llStkfrz;              // �ɷݶ���
  LONGLONG      llStktrdfrz;           // ���������
  LONGLONG      llStktrdunfrz;         // ��������
  LONGLONG      llStkbuysale;          // �ɷ�ʵʱ�������
  LONGLONG      llStkuncomebuy;        // ��;����
  LONGLONG      llStkuncomesale;       // ��;����
  char          szCostpriceEx[21 + 1]; // �ɱ��۸�
};

//-------------------------------10101521:�ʽ��ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryFundField
{
  LONGLONG      llFundid;         // �ʽ��˺�
  char          chMoneytype;      // ����
};
struct MATRADEAPI CRspKspbRpcQryFundField
{
  LONGLONG      llCustid;              // �ͻ�����
  LONGLONG      llFundid;              // �ʽ��˻�
  char          szOrgid[4 + 1];        // ��������
  char          chMoneytype;           // ����
  char          szFundbal[21 + 1];     // �ʽ����
  char          szFundavl[21 + 1];     // �ʽ���ý��
  char          szMarketvalue[21 + 1]; // �ʲ���ֵ
  char          szFund[21 + 1];        // �ʽ��ʲ�
  char          szStkvalue[21 + 1];    // ��ֵ
  int           iFundseq;              // ���ʽ��־
  char          szFundloan[21 + 1];    // �����ܽ��
  char          szFundbuy[21 + 1];     // ���붳��
  char          szFundsale[21 + 1];    // �����ⶳ
  char          szFundfrz[21 + 1];     // �����ܽ��
  char          szFundlastbal[21 + 1]; // �������
};

//-------------------------------10101684:ί�в�ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryOrderDetailField
{
  char          chMarket;          // �����г�
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szStkcode[8 + 1];  // ֤ȯ����
  int           iOrdersno;         // ί�����
  int           iOrdergroup;       // ί������
  char          szBankcode[4 + 1]; // �ⲿ����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
  char          szExtsno[32 + 1];  // �ⲿ��ˮ��
  char          chQryoperway;      // ί������
  int           iCuacctSn;         // �˻����
};
struct MATRADEAPI CRspKspbRpcQryOrderDetailField
{
  char          szPoststr[32 + 1];     // ��λ��
  int           iOrderdate;            // ί������
  int           iOrdersno;             // ί�����
  int           iOrdergroup;           // ί������
  LONGLONG      llCustid;              // �ͻ�����
  char          szCustname[16 + 1];    // �ͻ�����
  LONGLONG      llFundid;              // �ʽ��˻�
  char          chMoneytype;           // ����
  char          szOrgid[4 + 1];        // ��������
  char          szSecuid[10 + 1];      // �ɶ�����
  char          szBsflag[2 + 1];       // �������
  char          szOrderid[10 + 1];     // �걨��ͬ���
  int           iReporttime;           // ����ʱ��
  int           iOpertime;             // ί��ʱ��
  char          chMarket;              // �����г�
  char          szStkcode[8 + 1];      // ֤ȯ����
  char          szStkname[8 + 1];      // ֤ȯ����
  char          szProdcode[12 + 1];    // ��Ʒ����
  char          szProdname[64 + 1];    // ��Ʒ����
  char          szOrderprice[21 + 1];  // ί�м۸�
  int           iOrderqty;             // ί������
  char          szOrderfrzamt[21 + 1]; // ������
  int           iMatchqty;             // �ɽ�����
  char          szMatchamt[21 + 1];    // �ɽ����
  int           iCancelqty;            // ��������
  char          chOrderstatus;         // ί��״̬
  char          szSeat[6 + 1];         // ����ϯλ
  char          chCancelflag;          // ������ʶ
  int           iOperdate;             // ��������
  char          szBondintr[21 + 1];    // ծȯӦ����Ϣ
  char          chOperway;             // ί������
  char          szRemark[64 + 1];      // ��ע��Ϣ
};

//-------------------------------10101686:�ɽ���ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryMatchDetailField
{
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          chMarket;          // �����г�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szStkcode[8 + 1];  // ֤ȯ����
  int           iOrdersno;         // ί�����
  char          szBankcode[4 + 1]; // �ⲿ����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
  char          chQryoperway;      // ί������
  int           iCuacctSn;         // �˻����
};
struct MATRADEAPI CRspKspbRpcQryMatchDetailField
{
  char          szPoststr[32 + 1];    // ��λ��
  int           iTrddate;             // �ɽ�����
  char          szSecuid[10 + 1];     // �ɶ�����
  char          szBsflag[2 + 1];      // �������
  int           iOrdersno;            // ί�����
  char          szOrderid[10 + 1];    // �걨��ͬ���
  char          chMarket;             // �����г�
  char          szStkcode[8 + 1];     // ֤ȯ����
  char          szStkname[8 + 1];     // ֤ȯ����
  char          szProdcode[12 + 1];   // ��Ʒ����
  char          szProdname[64 + 1];   // ��Ʒ����
  int           iMatchtime;           // �ɽ�ʱ��
  char          szMatchcode[20 + 1];  // �ɽ����
  char          szMatchprice[21 + 1]; // �ɽ��۸�
  int           iMatchqty;            // �ɽ�����
  char          szMatchamt[21 + 1];   // �ɽ����
  char          chMatchtype;          // �ɽ�����
  int           iOrderqty;            // ί������
  char          szOrderprice[21 + 1]; // ί�м۸�
  char          szBondintr[21 + 1];   // ծȯӦ����Ϣ
};

//-------------------------------10101527:֤ȯ��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqKspbRpcStkInfoField
{
  char          chMarket;          // �����г�
  char          chStklevel;        // ֤ȯ����
  char          szStkcode[8 + 1];  // ֤ȯ����
  char          szPoststr[32 + 1]; // ��λ��
  int           iRowcount;         // ��ѯ����
  char          chStktype;         // ֤ȯ���
};
struct MATRADEAPI CRspKspbRpcStkInfoField
{
  char          szPoststr[32 + 1];      // ��λ��
  char          chMarket;               // �����г�
  char          chMoneytype;            // ����
  char          szStkname[8 + 1];       // ֤ȯ����
  char          szStkcode[8 + 1];       // ֤ȯ����
  char          chStktype;              // ֤ȯ���
  int           iPriceunit;             // ��λ
  char          szMaxrisevalue[21 + 1]; // ��ͣ�۸�
  char          szMaxdownvalue[21 + 1]; // ��ͣ�۸�
  char          chStopflag;             // ͣ�Ʊ�־
  char          chMtkcalflag;           // ��ֵ�����ʶ
  char          szBondintr[21 + 1];     // ծȯӦ����Ϣ
  int           iMaxqty;                // �������
  int           iMinqty;                // �������
  int           iBuyunit;               // ������С��λ
  int           iSaleunit;              // ������С��λ
  char          chStkstatus;            // ֤ȯ״̬
  char          chStklevel;             // ֤ȯ����
  char          chTrdid;                // ��������
  int           iQuitdate;              // ����֤ȯ���׽�ֹ����
  char          szFixprice[21 + 1];     // ϵͳ����
  char          chPriceflag;            // ί�м۸��־
  char          szMemotext[128 + 1];    // ����֤ȯ��ʾ��Ϣ
};

//-------------------------------10101676:�ɶ���ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryTrdacctField
{
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          chMarket;          // �����г�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
};
struct MATRADEAPI CRspKspbRpcQryTrdacctField
{
  char          szPoststr[32 + 1]; // ��λ��
  LONGLONG      llCustid;          // �ͻ�����
  char          chMarket;          // �����г�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szName[16 + 1];    // �ɶ�����
  int           iSecuseq;          // �ɶ����
  char          chRegflag;         // ָ������״̬
};

//-------------------------------10101500:������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqKspbRpcBankTransBasicInfoField
{
  char          chCurrency;        // ���Ҵ���
  char          szBankCode[4+1];   // ���д���
};
struct MATRADEAPI CRspKspbRpcBankTransBasicInfoField
{
  char          szBankCode[4+1];   // ���д���
  char          chSourceType;      // ����
  char          chBankTranType;    // ת�ʷ�ʽ
  char          chFundPwdFlag;     // �ʽ�����У��
  char          chBankPwdFlag;     // ��������У��
  char          chCheckBankId;     // �����ʺ�У��
  char          chCheckIdNo;       // ֤��У���־
  int           iIntOrg;           // ��������
  char          chCurrency;        // ���Ҵ���
  char          chStatus;          // ����״̬
};

//-------------------------------10101501:��֤ת��--------------------------
struct MATRADEAPI CReqKspbRpcBankStkTransField
{
  LONGLONG      llCuacctCode;      // �ʽ��˺�
  char          chCurrency;        // ���Ҵ���
  char          szFundPwd[32+1];   // �ʽ�����
  char          szBankCode[4+1];   // ���д���
  char          szBankPwd[32+1];   // ��������
  char          chBankTranType;    // ת������
  char          szTransAmt[21+1];  // ת�ʽ��
  char          chPwdFlag;         // �ʽ�����У���־
};
struct MATRADEAPI CRspKspbRpcBankStkTransField
{
  int           iSNo;              // ί�����
  int           iSysErrorId;       // �������
  char          szErrorMsg[64+1];  // ������Ϣ
};

//-------------------------------10101503:ת����ˮ��ѯ--------------------------
struct MATRADEAPI CReqKspbRpcBankStkTransInfoField
{
  LONGLONG      llCuacctCode;       // �ʽ��˺�
  char          chCurrency;         // ���Ҵ���
  char          szSNo[32 + 1];      // ת�����
};
struct MATRADEAPI CRspKspbRpcBankStkTransInfoField
{
  int           iOperDate;           // ת������
  int           iOperTime;           // ת��ʱ��
  LONGLONG      llCuacctCode;        // �ʽ��˺�
  char          chCurrency;          // ���Ҵ���
  LONGLONG      llCustCode;          // �ͻ�����
  char          szBankCode[4 + 1];   // ���д���
  char          chBankTranId;        // ҵ������
  int           iSNo;                // ��ͬ���
  LONGLONG      llFundEffect;        // ί�н��
  LONGLONG      llFundBal;           // ���   
  char          szRemark[32 + 1];    // ��ע��Ϣ
  char          chStatus;            // ������
  char          chSourceType;        // ������
  char          szBankMsgId[16 + 1]; // �ⲿ��Ϣ����
  char          szBankMsg[64 + 1];   // �ⲿ��Ϣ����
  char          szErrorMsg[64 + 1];  // ϵͳ������Ϣ
  int           iSysErrId;           // ϵͳ�������
  char          szFundEffect[23 + 1];// ί�н��
  char          szFundBal[23 + 1];   // ���
};

//-------------------------------10101509:��ʷί�в�ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryHistoryOrderDetailField
{
  int           iStrdate;          // ��ʼ����
  int           iEnddate;          // ��ֹ����
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          chMarket;          // �����г�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szStkcode[8 + 1];  // ֤ȯ����
  int           iOrdersno;         // ί�����
  int           iOrdergroup;       // ί������
  char          szBankcode[4 + 1]; // �ⲿ����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
  char          szExtsno[32 + 1];  // �ⲿ��ˮ��
  char          chQryoperway;      // ί������
};
struct MATRADEAPI CRspKspbRpcQryHistoryOrderDetailField
{
  char          szPoststr[32 + 1];     // ��λ��
  int           iOrderdate;            // ί������
  LONGLONG      llCustid;              // �ͻ�����
  char          szCustname[16 + 1];    // �ͻ�����
  LONGLONG      llFundid;              // �ʽ��˻�
  char          chMoneytype;           // ����
  char          szOrgid[4 + 1];        // ��������
  char          szSecuid[10 + 1];      // �ɶ�����
  char          szBsflag[2 + 1];       // �������
  char          szOrderid[10 + 1];     // �걨��ͬ���
  int           iOrdergroup;           // ί������
  int           iReporttime;           // ����ʱ��
  int           iOpertime;             // ί��ʱ��
  char          chMarket;              // �����г�
  char          szStkcode[8 + 1];      // ֤ȯ����
  char          szStkname[8 + 1];      // ֤ȯ����
  char          szProdcode[12 + 1];    // ��Ʒ����
  char          szOrderprice[21 + 1];  // ί�м۸�
  int           iOrderqty;             // ί������
  char          szOrderfrzamt[21 + 1]; // ������
  int           iMatchqty;             // �ɽ�����
  char          szMatchamt[21 + 1];    // �ɽ����
  int           iCancelqty;            // ��������
  char          chCancelflag;          // ������־
  char          chOrderstatus;         // ί��״̬
  char          szSeat[6 + 1];         // ����ϯλ
  char          szMatchprice[21 + 1];  // �ɽ��۸�
  int           iOperdate;             // ��������
  char          szBondintr[21 + 1];    // ծȯӦ����Ϣ
  char          chOperway;             // ί������
  char          szRemark[64 + 1];      // ��ע��Ϣ
};

//-------------------------------10101510:��ʷ�ɽ���ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryHistoryMatchDetailField
{
  int           iStrdate;          // ��ʼ����
  int           iEnddate;          // ��ֹ����
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          chMarket;          // �����г�
  char          szSecuid[10 + 1];  // �ɶ�����
  char          szStkcode[8 + 1];  // ֤ȯ����
  char          szBankcode[4 + 1]; // �ⲿ����
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
};
struct MATRADEAPI CRspKspbRpcQryHistoryMatchDetailField
{
  char          szPoststr[32 + 1];    // ��λ��
  int           iBizdate;             // ��������
  int           iCleardate;           // �ɽ�����
  char          szSecuid[10 + 1];     // �ɶ�����
  char          szBsflag[2 + 1];      // �������
  char          szOrderid[10 + 1];    // �걨��ͬ���
  int           iOrdersno;            // ί�����
  char          chMarket;             // �����г�
  char          szStkcode[8 + 1];     // ֤ȯ����
  char          szStkname[8 + 1];     // ֤ȯ����
  char          szProdcode[12 + 1];   // ��Ʒ����
  int           iMatchtime;           // �ɽ�ʱ��
  char          szMatchcode[20 + 1];  // �ɽ����
  char          szMatchprice[21 + 1]; // �ɽ��۸�
  int           iMatchqty;            // �ɽ�����
  char          szMatchamt[21 + 1];   // �ɽ����
  int           iOrderqty;            // ί������
  char          szOrderprice[21 + 1]; // ί�м۸�
  int           iStkbal;              // �ɷݱ������
  char          szFeeJsxf[21 + 1];    // ��������
  char          szFeeSxf[21 + 1];     // ������
  char          szFeeYhs[21 + 1];     // ӡ��˰
  char          szFeeGhf[21 + 1];     // ������
  char          szFeeQsf[21 + 1];     // �����
  char          szFeeJygf[21 + 1];    // ���׹��
  char          szFeeJsf[21 + 1];     // ���ַ�
  char          szFeeZgf[21 + 1];     // ֤�ܷ�
  char          szFeeQtf[21 + 1];     // ������
  char          szFeefront[21 + 1];   // ǰ̨����
  char          szFundeffect[21 + 1]; // �ʽ�����
  char          szFundbal[21 + 1];    // ʹ�ý��
  char          szBondintr[21 + 1];   // ծȯӦ����Ϣ
};

//-------------------------------10101506:�ʽ�ɷ���ˮ��ѯ--------------------------
struct MATRADEAPI CReqKspbRpcStatementField
{
  int           iStrDate;              // ��ʼ����
  int           iEndDate;              // ��ֹ����
  LONGLONG      llCuacctCode;          // �ʽ��ʻ�
  char          chCurrency;            // ���Ҵ���
  char          chPrintFlag;           // �ش��ʶ
  char          chQryFlag;             // ��ѯ����
  int           iQryNum;               // ��������
  char          szQryPos[32 + 1];      // ��λ��
};
struct MATRADEAPI CRspKspbRpcStatementField
{
  char          szQryPos[32 + 1];      // ��λ��  
  int           iClearDate;            // ��������
  int           iBizDate;              // ��������
  int           iOrderDate;            // ��������
  int           iOrderTime;            // ����ʱ��
  int           iDigestId;             // ҵ�����
  char          szDigestName[16 + 1];  // ҵ��˵��
  LONGLONG      llCustId;              // �ͻ�����
  char          szCustName[16 + 1];    // �ͻ�����
  char          szIntOrg[4 + 1];       // ��������
  LONGLONG      llCuacctCode;          // �ʽ��ʺ�
  char          chCurrency;            // ���Ҵ���
  char          chStkex;               // �г�����
  char          szTrdacct[32 + 1];     // �ɶ�����
  LONGLONG      llFundEffect;          // �ʽ�����
  LONGLONG      llFundBal;             // �ʽ𱾴����
  int           iStkBal;               // �ɷݱ������
  char          szOrderId[10 + 1];     // ��ͬ���
  char          szStkCode[16 + 1];     // ֤ȯ����
  char          szStkName[16 + 1];     // ֤ȯ����
  char          szBsFlag[2 + 1];       // �������
  int           iMatchedQty;           // �ɽ�����
  LONGLONG      llMatchedPrice;        // �ɽ��۸�
  LONGLONG      llMatchedAmt;          // �ɽ����
};

//-------------------------------10101517:�����ѯ--------------------------
struct MATRADEAPI CReqKspbRpcQryDeliOrderField
{
  int           iStrdate;          // ��ʼ����
  int           iEnddate;          // ��ֹ����
  LONGLONG      llFundid;          // �ʽ��ʻ�
  char          chPrintflag;       // �ش��ʶ
  char          chQryflag;         // ��ѯ����
  int           iCount;            // ��������
  char          szPoststr[32 + 1]; // ��λ��
};
struct MATRADEAPI CRspKspbRpcQryDeliOrderField
{
  char          szPoststr[32 + 1];    // ��λ��
  int           iCleardate;           // ��������
  int           iBizdate;             // ��������
  int           iOrderdate;           // ί������
  int           iOrdertime;           // ί��ʱ��
  int           iDigestid;            // ҵ�����
  char          szDigestname[16 + 1]; // ҵ�����˵��
  LONGLONG      llFundid;             // �ʽ��ʻ�
  char          chMoneytype;          // ���Ҵ���
  char          chMarket;             // ����������
  char          szSecuid[32 + 1];     // �ɶ�����
  char          szCustname[16 + 1];   // �ͻ�����
  char          szOrderid[10 + 1];    // ��ͬ���
  char          szStkcode[16 + 1];    // ֤ȯ����
  char          szStkname[16 + 1];    // ֤ȯ����
  char          szBsflag[2 + 1];      // �������
  int           iMatchtime;           // �ɽ�ʱ��
  char          szMatchcode[20 + 1];  // �ɽ�����
  int           iMatchtimes;          // �ɽ�����
  int           iMatchqty;            // �ɽ�����
  char          szMatchprice[21 + 1]; // �ɽ��۸�
  char          szMatchamt[21 + 1];   // �ɽ����
  char          szFundeffect[21 + 1]; // ������
  char          szFeeYhs[21 + 1];     // ӡ��˰
  char          szFeeJsxf[21 + 1];    // ��׼������
  char          szFeeSxf[21 + 1];     // ������
  char          szFeeGhf[21 + 1];     // ������
  char          szFeeQsf[21 + 1];     // �����
  char          szFeeJygf[21 + 1];    // ���׹��
  char          szFeefront[21 + 1];   // ǰ̨����
  char          szFeeJsf[21 + 1];     // ���ַ�
  char          szFeeZgf[21 + 1];     // ֤�ܷ�
  char          szFundbal[21 + 1];    // �ʽ𱾴����
  int           iStkbal;              // �ɷݱ������
  int           iOrderqty;            // ί������
  char          szOrderprice[21 + 1]; // ί�м۸�
  char          chSourcetype;         // ����
  char          szBankcode[4 + 1];    // �ⲿ����
  char          szBankid[32 + 1];     // �ⲿ�˻�
  char          szFeeOneYhs[21 + 1];  // һ��ӡ��˰
  char          szFeeOneGhf[21 + 1];  // һ��������
  char          szFeeOneQsf[21 + 1];  // һ�������
  char          szFeeOneJygf[21 + 1]; // һ�����׹��
  char          szFeeOneJsf[21 + 1];  // һ�����ַ�
  char          szFeeOneZgf[21 + 1];  // һ��֤�ܷ�
  char          szFeeOneQtf[21 + 1];  // һ��������
  char          szFeeOneFxj[21 + 1];  // һ�����ս�
};

//-------------------------------00102023:�ɽ��ر�����------------------------------------
struct MATRADEAPI CRtnKspbRpcOrderFillField
{
  char          szMatchedSn[32 + 1];        // �ɽ����
  char          chStkex;                    // �����г�
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szOrderId[10 + 1];          // ��ͬ���
  char          szTrdacct[20 + 1];          // �����˻�
  LONGLONG      llMatchedQty;               // ���γɽ�����
  char          szMatchedPrice[11 + 1];     // ���γɽ��۸�
  char          szOrderFrzAmt[21 + 1];      // ί�ж�����
  char          szRltSettAmt[21 + 1];       // ʵʱ������
  char          szFundAvl[21 + 1];          // �ʽ���ý��ɽ���
  LONGLONG      llStkAvl;                   // ֤ȯ�����������ɽ���
  char          szOpptStkpbu[8 + 1];        // �Է�ϯλ
  char          szOpptTrdacct[20 + 1];      // �Է������˺�
  int           iMatchedDate;               // �ɽ�����
  char          szMatchedTime[8 + 1];       // �ɽ�ʱ��
  char          chIsWithdraw;               // ������־
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iOrderBsn;                  // ί������
  int           iCuacctSn;                  // �˻����
  char          szStkbd[2 + 1];             // ���װ��
  char          chMatchedType;              // �ɽ�����
  char          chOrderStatus;              // ί��״̬
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ֤ȯҵ����Ϊ
  char          szOfferRetMsg[64 + 1];      // �걨��Ϣ �������ϵ�ʱ���طϵ�ԭ��
  LONGLONG      llOrderQty;                 // ί������
  LONGLONG      llWithdrawnQty;             // �ѳ�������
  LONGLONG      llTotalMatchedQty;          // �ۼƳɽ�����
  char          chCuacctAttr;               // �˻�����
  char          szTotalMatchedAmt[21 + 1];  // �ۼƳɽ����
  LONGLONG      llStkQty;                   // ӵ����
  char          szMatchedAmt[21 + 1];       // �ѳɽ����
  char          szHFundAvl[21 + 1];         // �۹��ʽ����
  char          szRemark1[32 + 1];          // Ԥ���ֶ�1
  char          szRemark2[32 + 1];          // Ԥ���ֶ�2
  char          szRemark3[32 + 1];          // Ԥ���ֶ�3
  char          szRemark4[32 + 1];          // Ԥ���ֶ�4
  char          szRemark5[32 + 1];          // Ԥ���ֶ�5
  char          szRemark6[32 + 1];          // Ԥ���ֶ�6
  char          szRemark7[32 + 1];          // Ԥ���ֶ�7
  char          szRemark8[32 + 1];          // Ԥ���ֶ�8
  char          szRemark9[32 + 1];          // Ԥ���ֶ�9
  char          szRemarkA[32 + 1];          // Ԥ���ֶ�A
  char          szOrderNo[10 + 1];          // ί�б��
  LONGLONG      llStkTrdEtfctn;             // ETF�깺����
  LONGLONG      llStkTrdEtfrmn;             // ETF�������
};

//-------------------------------00102024:ȷ�ϻر�����------------------------------------
struct MATRADEAPI CRtnKspbRpcOrderConfirmField
{
  char          chStkex;                    // �����г�
  char          szStkCode[8 + 1];           // ֤ȯ����
  char          szOrderId[10 + 1];          // ��ͬ���
  char          szTrdacct[20 + 1];          // �����˻�
  char          chIsWithdraw;               // ������־
  LONGLONG      llCustCode;                 // �ͻ�����
  LONGLONG      llCuacctCode;               // �ʲ��˻�
  int           iOrderBsn;                  // ί������
  int           iCuacctSn;                  // �˻����
  char          szStkbd[2 + 1];             // ���װ��
  char          chOrderStatus;              // ί��״̬
  int           iStkBiz;                    // ֤ȯҵ��
  int           iStkBizAction;              // ҵ����Ϊ
  char          chCuacctAttr;               // �˻�����
  int           iOrderDate;                 // ί������
  int           iOrderSn;                   // ί�����
  int           iIntOrg;                    // �ڲ�����
  char          szStkpbu[8 + 1];            // ���׵�Ԫ
  char          szOrderPrice[21 + 1];       // ί�м۸�
  LONGLONG      llOrderQty;                 // ί������
  char          szRemark1[32 + 1];          // Ԥ���ֶ�1
  char          szRemark2[32 + 1];          // Ԥ���ֶ�2
  char          szRemark3[32 + 1];          // Ԥ���ֶ�3
  char          szRemark4[32 + 1];          // Ԥ���ֶ�4
  char          szRemark5[32 + 1];          // Ԥ���ֶ�5
  char          szRemark6[32 + 1];          // Ԥ���ֶ�6
  char          szRemark7[32 + 1];          // Ԥ���ֶ�7
  char          szRemark8[32 + 1];          // Ԥ���ֶ�8
  char          szRemark9[32 + 1];          // Ԥ���ֶ�9
  char          szRemarkA[32 + 1];          // Ԥ���ֶ�A
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_KSPB_TRADE_API_STRUCT_H__