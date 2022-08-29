#if !defined(__MA_CLI_FS_TRADE_API_STRUCT_H__)
#define __MA_CLI_FS_TRADE_API_STRUCT_H__

#include "maCliFs2Api.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10308011:������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryOrgInfoField
{
  int             iIntOrg;                    // �ڲ����� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryOrgInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iIntOrg;                    // �ڲ����� 
  char            szOrgName[32 + 1];          // �������� 
};

//-------------------------------10308013:֤ȯ��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryTrdInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryTrdInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            chStkStatus;                // ֤ȯ״̬ �ֵ�[STK_STATUS]
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkUplmtRatio[21 + 1];    // ��ͣ���� 
  char            szStkLwlmtRatio[21 + 1];    // ��ͣ���� 
  char            szStkUplmtPrice[21 + 1];    // ��ͣ�۸� 
  char            szStkLwlmtPrice[21 + 1];    // ��ͣ�۸� 
  LONGLONG        llStkUplmtQty;              // ��������(�޼�) 
  LONGLONG        llStkLwlmtQty;              // ��������(�޼�) 
  LONGLONG        llStkLwlmtQtyMp;            // ��������(�м�) 
  LONGLONG        llStkUplmtQtyMp;            // ��������(�м�) 
  LONGLONG        llStkLotSize;               // ÿ������ 
  char            chStkLotFlag;               // ��λ��־ 
  LONGLONG        llStkSpread;                // ��λ�۲� 
  char            chStkCalMktval;             // ��ֵ��־ �ֵ�[STK_CAL_MKTVAL]
  char            chStkSuspended;             // ͣ�Ʊ�־ �ֵ�[STK_SUSPENDED]
  char            szStkIsin[16 + 1];          // ���ʱ��� 
  char            chStkSubCls;                // ֤ȯ����� �ֵ�[STK_SUB_CLS]
  char            szStkBizes[512 + 1];        // ֤ȯҵ�� 
  char            chStkCustodyMode;           // �й�ģʽ �ֵ�[STK_CUSTODY_MODE]
  char            szStkUndlCode[8 + 1];       // ���ɴ��� 
  char            szStkFaceVal[21 + 1];       // ֤ȯ��ֵ 
  char            szBondInt[21 + 1];          // ծȯ��Ϣ 
  char            chStkLevel;                 // ֤ȯ���� �ֵ�[STK_LEVEL]
};

//-------------------------------10308012:�ֵ���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryDictInfoField
{
  char            szDdId[32 + 1];             // �ֵ��ʶ 
  char            szDdItem[32 + 1];           // �ֵ���Ŀ 
};

struct MATRADEAPI CRspFsQryDictInfoField
{
  char            szDdName[32 + 1];           // �ֵ����� 
  char            szDdId[32 + 1];             // �ֵ��ʶ 
  char            szDdItem[32 + 1];           // �ֵ��� 
  char            szDdItemName[128 + 1];      // �ֵ������� 
  int             iIntOrg;                    // �ڲ����� 
};

//-------------------------------10308014:ϵͳ״̬��ѯ--------------------------
struct MATRADEAPI CReqFsQrySysStatField
{
};

struct MATRADEAPI CRspFsQrySysStatField
{
  int             iTrdDate;                   // �������� 
  int             iSettDate;                  // �������� 
  char            chBizStatus;                // ϵͳ״̬ �ֵ�[BIZ_STATUS]
};

//-------------------------------10510605:��֤ת��--------------------------
struct MATRADEAPI CReqFsBankStkTransferField
{
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  int             iExtOrg;                    // ���д��� 
  char            szFundAuthData[32 + 1];     // �ʽ����� 
  char            szBankAuthData[32 + 1];     // �������� 
  char            szBizAmt[21 + 1];           // ҵ���� 
  char            szOpRemark[128 + 1];        // ������ע 
  int             iIntOrg;                    // �������� 
  char            chEncryptType;              // ����������� 0��AES�ټ�������1��δ�ټ�������2������
  char            szBankCustCode[200 + 1];    // ���пͻ��� 
  char            chBanktrantype;             // ת������ 1������ת֤ȯ2��֤ȯת����
};

struct MATRADEAPI CRspFsBankStkTransferField
{
  char            szSerialNo[64 + 1];         // ��ˮ��� 
};

//-------------------------------10510606:ȯ�̷�����������--------------------------
struct MATRADEAPI CReqFsQryInitiatorOrderBankBalField
{
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iExtOrg;                    // �ⲿ���� 
  char            szFundAuthData[32 + 1];     // �ʽ������ 
  char            szBankAuthData[32 + 1];     // �������� 
  char            szOpRemark[128 + 1];        // ��ע��Ϣ 
  char            chEncryptType;              // ����������� 0��AES�ټ�������1��δ�ټ�������2������
  int             iIntOrg;                    // �������� 
};

struct MATRADEAPI CRspFsQryInitiatorOrderBankBalField
{
  char            szSerialNo[64 + 1];         // ��ˮ��� 
};

//-------------------------------10510608:��ѯ��֤ҵ����ˮ--------------------------
struct MATRADEAPI CReqFsQryBankStkDetailField
{
  int             iOccurDate;                 // �������� 
  LONGLONG        llSerialNo;                 // ��ˮ��� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  LONGLONG        llCuacctCode;               // �ʲ��ʻ� 
  int             iExtOrg;                    // ���д��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  int             iIntOrg;                    // �ڲ����� 
  char            chParamType;                // �������� �ֵ�[PARAM_TYPE]
};

struct MATRADEAPI CRspFsQryBankStkDetailField
{
  LONGLONG        llSerialNo;                 // ��ˮ��� 
  int             iOccurDate;                 // �������� 
  char            szOccurTime[32 + 1];        // ����ʱ�� 
  int             iExtOrg;                    // ���д��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szCubsbTrdId[2 + 1];        // ��֤ҵ���ʶ �ֵ�[CUBSB_TRD_ID]
  char            szBizAmt[21 + 1];           // ҵ���� 
  char            szFudnBln[21 + 1];          // �ʽ�ǰ��� 
  char            chCubsbTrdStatus;           // ��֤ҵ��״̬ �ֵ�[CUBSB_TRD_STATUS]
  char            chSourceType;               // ������ 0��֤ȯ����1�����з���2��˫�߷���
  LONGLONG        llIntMsgCode;               // �ڲ���Ϣ���� 
  char            szIntMsgText[32 + 1];       // �ڲ�������Ϣ 
  LONGLONG        llExtMsgCode;               // �ⲿ��Ϣ���� 
  char            szExtMsgText[256 + 1];      // �ⲿ��Ϣ���� 
  LONGLONG        llExtSerialNo;              // �ⲿ��ˮ�� 
  char            szExtAnsStime[32 + 1];      // �ɽ�ʱ�� 
  char            szRemark[32 + 1];           // ��ע��Ϣ 
};

//-------------------------------10510620:�ͻ����ֽ�ת--------------------------
struct MATRADEAPI CReqFsCustCashTransferField
{
  int             iIntOrg;                    // �������� 
  LONGLONG        llOutfundid;                // ת���ʽ��ʺ� 
  LONGLONG        llOutpassword;              // ת���ʽ����� 
  LONGLONG        llInfundid;                 // ת���ʽ��ʺ� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szFundeffect[21 + 1];       // ת�ʽ�� 
  char            chCheckpwdflag;             // ��������־ 0����У��1��У��
  char            chCashcheck;                // �ֽ�֧Ʊ���� 0���ֽ�1��֧Ʊ
  char            chEncryptType;              // ����������� 0��AES�ټ�������1��δ�ټ�������2������
};

struct MATRADEAPI CRspFsCustCashTransferField
{
  LONGLONG        llOutSno;                   // ת����ˮ�� 
  LONGLONG        llInSno;                    // ת����ˮ�� 
};

//-------------------------------10510621:�˻��ʽ�鼯--------------------------
struct MATRADEAPI CReqFsQryAcctConcentrationField
{
  int             iOrgid;                     // �������� 
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            chCashcheck;                // �ֽ�֧Ʊ���� 0���ֽ�1��֧Ʊ
};

struct MATRADEAPI CRspFsQryAcctConcentrationField
{
  char            szFundeffect[21 + 1];       // �鼯��� 
  int             iRenum;                     // ���ر��� 
};

//-------------------------------10510601:��ѯת�������ʻ�--------------------------
struct MATRADEAPI CReqFsQryTransferAcctField
{
  int             iExtOrg;                    // ���д��� ����ֵ��ѯȫ��
  char            chCurrency;                 // ���Ҵ��� ����ֵ��ѯȫ��
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
};

struct MATRADEAPI CRspFsQryTransferAcctField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  int             iIntOrg;                    // �������� 
  int             iExtOrg;                    // ���д��� 
  char            szOrgName[32 + 1];          // �������� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szBankAcct[32 + 1];         // �����˻� 
  char            szBdmfAcct[32 + 1];         // ����˻� 
  LONGLONG        llCuacctCode;               // �ʽ��˺� 
};

//-------------------------------10510468:�Ƹ��˻��ʽ𻮲�--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctTransferField
{
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  LONGLONG        llFortuneCodeOut;           // ת���Ƹ��˻� 
  LONGLONG        llFortuneCodeIn;            // ת��Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szBizAmt[21 + 1];           // ������� ����
};

struct MATRADEAPI CRspFsQryFortuneAcctTransferField
{
  int             iOutSno;                    // ת����ˮ�� 
  int             iInSno;                     // ת����ˮ�� 
};

//-------------------------------10510473:�Ƹ��˻�������ˮ��ѯ--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctTransferDetailField
{
  int             iBeginDate;                 // ��ʼ���� 
  int             iEndDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  LONGLONG        llFortuneCode;              // �Ƹ��˻� -1��ʾ��ȫ��
  char            chDirection;                // �������� S��������(BIZ_CODE = 20040054)D�����뷽(BIZ_CODE = 20040054)���Ͳ�ȫ��
  LONGLONG        llSerialNo;                 // ��ˮ�� ���Ͳ�ȫ��
  char            chQryflag;                  // ��ѯ���� 0�����²�ѯ1�����ϲ�ѯ
  int             iCount;                     // �������� ÿ��ȡ������
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����(��ѯ���ص���ˮ��)
};

struct MATRADEAPI CRspFsQryFortuneAcctTransferDetailField
{
  char            szQryPos[32 + 1];           // ��λ�� ��ѯ���ض�λֵ(���һ����¼����ˮ��)
  int             iTrdDate;                   // �������� 
  LONGLONG        llSerialNo;                 // ��ˮ�� 
  char            chDirection;                // �������� �ֵ�[DIRECTION]
  int             iOperDate;                  // �������� 
  int             iOperTime;                  // ����ʱ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[16 + 1];         // �ͻ����� 
  int             iIntOrg;                    // �������� 
  LONGLONG        llCuacctCode;               // �ʽ��˺� 
  LONGLONG        llFortuneCode;              // �Ƹ��˻� 
  char            szFortuneAlias[32 + 1];     // �Ƹ��˻����� 
  LONGLONG        llOtherFortuneCode;         // �Է��Ƹ��˻� 
  char            szOtherFortuneAlias[32 + 1];// �Է��Ƹ��˻����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szFundeffect[21 + 1];       // �ʽ����� 
  char            chStatus;                   // ����״̬ 0:������1:�ɹ�
};

//-------------------------------10301105:�ͻ���¼--------------------------
struct MATRADEAPI CReqFsQryCustLoginField
{
  char            szAcctType[2 + 1];          // �˻����� U:�ͻ����� Z:�ʲ��˻� 00:��A�ɶ� 01:��B�ɶ� 02:����ɶ� 10:��A�ɶ� 11:��B�ɶ�
  char            szAcctId[32 + 1];           // �˻���ʶ 
  char            chUseScope;                 // ʹ�÷�Χ �ֵ�[USE_SCOPE]
  char            szEncryptKey[32 + 1];       // �������� 
  char            chAuthType;                 // ��֤���� �ֵ�[AUTH_TYPE]
  char            szAuthData[256 + 1];        // ��֤���� 
  char            szThirdParty[1024 + 1];     // ������ 
};

struct MATRADEAPI CRspFsQryCustLoginField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkTrdacct[20 + 1];       // ֤ȯ�˻� 
  int             iTrdacctSn;                 // �˻���� 
  char            szTrdacctExid[20 + 1];      // �����˻� 
  char            chTrdacctStatus;            // �˻�״̬ 
  char            chTregStatus;               // ָ��״̬ 
  char            chBregStatus;               // �ع�״̬ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szAcctType[2 + 1];          // �˻����� U:�ͻ����� Z:�ʲ��˻� 00:��A�ɶ� 01:��B�ɶ� 02:����ɶ� 10:��A�ɶ� 11:��B�ɶ�
  char            szAcctId[32 + 1];           // �˻���ʶ 
  char            szTrdacctName[32 + 1];      // �����˻����� 
  char            szChannelId[2 + 1];         // ͨ���� 
  char            szSessionId[128 + 1];       // �Ựƾ֤ 
  int             iIntOrg;                    // �ڲ����� 
  char            chCuacctAttr;               // �ʲ��˻����� �ֵ�[CUACCT_ATTR]
  int             iSubsysSn;                  // Զ����ϵͳ��� ָ�ʲ������Ӧ��ϵͳ���� �ԽӼ��н���ϵͳʱ�Ǽ��н���ϵͳ�Ľڵ���� �Խ��ʲ��й�ϵͳʱ,ָ���ǲ����й�ϵͳ
  char            szThirdParty[1024 + 1];     // ������ 
};

//-------------------------------10308112:�ʲ��˻���ѯ--------------------------
struct MATRADEAPI CReqFsQryAssetsAcctField
{
  LONGLONG        llUserCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
};

struct MATRADEAPI CRspFsQryAssetsAcctField
{
  LONGLONG        llUserCode;                 // �û����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCuacctAttr;               // �ʲ��˻����� �ֵ�[CUACCT_ATTR]
  char            szCuacctCls[4 + 1];         // �ʲ��˻���� 
  char            szCuacctLvl[4 + 1];         // �ʲ��˻����� 
  char            szCuacctGrp[4 + 1];         // �ʲ��˻���� 
  char            chCuacctStatus;             // �ʲ��˻�״̬ �ֵ�[CUACCT_STATUS]
  char            chMainFlag;                 // ���˻���ʶ �ֵ�[TRDACCT_MAIN_FLAG]
  int             iIntOrg;                    // �ڲ����� 
  int             iOpenDate;                  // �������� 
  int             iCloseDate;                 // �������� 
};

//-------------------------------10308113:�Ƹ��˻���ѯ--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

struct MATRADEAPI CRspFsQryFortuneAcctField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chFortuneStatus;            // �Ƹ��˻�״̬ �ֵ�[CUACCT_STATUS]
  char            szFortuneAlias[32 + 1];     // �Ƹ��˻����� 
  int             iOpenDate;                  // �������� 
  int             iCloseDate;                 // �������� 
};

//-------------------------------10308001:�ɶ���ѯ--------------------------
struct MATRADEAPI CReqFsQrySecuField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
};

struct MATRADEAPI CRspFsQrySecuField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chTrdacctStatus;            // �˻�״̬ �ֵ�[TRDACCT_STATUS]
  char            chTregStatus;               // ָ��״̬ �ֵ�[TREG_STATUS]
  char            szCustName[8 + 1];          // �ͻ����� 
  char            chTrdacctExcls;             // �����˻���� �ֵ�[TRDACCT_EXCLS]
  char            chTrdacctMainFlag;          // �������˻���־ �ֵ�[TRDACCT_MAIN_FLAG]
  char            chLimitFlag;                // ���޹ɶ���־ �ֵ�[LIMIT_FLAG]
};

//-------------------------------10308010:�ɶ�����Э���ѯ--------------------------
struct MATRADEAPI CReqFsQrySecuTransferAgreementField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
};

struct MATRADEAPI CRspFsQrySecuTransferAgreementField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szCustAgmtType[128 + 1];    // Э������ �ֵ�[CUST_AGMT_TYPE]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  int             iEftDate;                   // ��Ч���� 
  int             iExpDate;                   // ʧЧ���� 
};

//-------------------------------10308611:�ʵ���ƥ����--------------------------
struct MATRADEAPI CReqFsPropriatenceCheckField
{
  LONGLONG        llCustCode;                 // �ͻ����� CUST_CODE��CUACCT_CODE����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʲ��˺� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
};

struct MATRADEAPI CRspFsPropriatenceCheckField
{
  LONGLONG        llPropRiskNo;               // �ʵ�����ˮ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  char            chCheckResult;              // �ʵ���ƥ���� 0 У��ͨ��1 ��ƥ��2 δ����3 �ͻ����յȼ��ѹ���4 רҵͶ�����ѹ���
  char            chCustRiskLevel;            // �ͻ����յȼ� �ֵ�[CUST_RISK_LEVEL]
  char            chCustInvestLmt;            // �ͻ�Ͷ������ �ֵ�[INVEST_LMT]
  char            szCustInvestPro[128 + 1];   // �ͻ�Ͷ��Ʒ�� �ֵ�[INVEST_PRO]
  char            chCustExpectIncome;         // �ͻ�Ԥ������ �ֵ�[EXINCOME_TYPE]
  char            chStkRiskLevel;             // ��Ʒ���յȼ� �ֵ�[STK_RISK_LEVEL]
  char            chStkInvestPeriod;          // Ͷ������ �ֵ�[INVEST_LMT]
  char            szStkInvestType[2 + 1];     // Ͷ��Ʒ�� �ֵ�[INVEST_PRO]
  char            chStkExincomeType;          // Ԥ������ �ֵ�[EXINCOME_TYPE]
  char            chRiskLevelFitFlag;         // ���յȼ�ƥ���� �ֵ�[FIT_FLAG]
  char            chInvestPeriodFitFlag;      // Ͷ������ƥ���� �ֵ�[FIT_FLAG]
  char            chInvestTypeFitFlag;        // Ͷ��Ʒ��ƥ���� �ֵ�[FIT_FLAG]
  char            chExincomeTypeFitFlag;      // Ԥ������ƥ���� �ֵ�[FIT_FLAG]
  char            szCheckDesc[512 + 1];       // У�������� 
  char            chCustLastriskFlag;         // ��Ϳͻ���������ʾ �ֵ�[CUST_LASTRISK_FLAG]
  int             iCustRiskLevelDate;         // ���յȼ���Ч�� 
};

//-------------------------------10308068:�ͻ��ʵ��Բ�ѯ--------------------------
struct MATRADEAPI CReqFsQryCustPropriatenceCheckField
{
  LONGLONG        llCuacctCode;               // �ʲ��˺� 
};

struct MATRADEAPI CRspFsQryCustPropriatenceCheckField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chProfInvestorType;         // רҵͶ���߷��� �ֵ�[PROF_INVESTOR_TYPE]
  int             iProfExpDate;               // רҵͶ���ߵ������� 
  char            chInvestLmt;                // Ͷ������ �ֵ�[INVEST_LMT]
  char            szInvestPro[32 + 1];        // Ͷ��Ʒ�� �ֵ�[INVEST_PRO]
  char            chExpectIncome;             // Ԥ������ �ֵ�[EXINCOME_TYPE]
  char            chCustLastriskFlag;         // ��Ϳͻ���������ʾ �ֵ�[CUST_LASTRISK_FLAG]
  char            chCustRiskLevel;            // �ͻ����յȼ� �ֵ�[CUST_RISK_LEVEL]
  int             iCustRiskLevelDate;         // ���յȼ���Ч�� 
};

//-------------------------------10308069:��Ʒ�ʵ��Բ�ѯ--------------------------
struct MATRADEAPI CReqFsQryProdPropriatenceCheckField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
};

struct MATRADEAPI CRspFsQryProdPropriatenceCheckField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chStkRiskLevel;             // ��Ʒ���յȼ� �ֵ�[STK_RISK_LEVEL]
  char            chInvestPeriod;             // Ͷ������ �ֵ�[INVEST_LMT]
  char            szInvestType[2 + 1];        // Ͷ��Ʒ�� �ֵ�[INVEST_PRO]
  char            chExincomeType;             // Ԥ������ �ֵ�[EXINCOME_TYPE]
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
};

//-------------------------------10308048:�ʽ��ѯ--------------------------
struct MATRADEAPI CReqFsQryFundField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� �ʲ��˻��Ϳͻ����벻��ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  LONGLONG        llCustCode;                 // �ͻ����� 
};

struct MATRADEAPI CRspFsQryFundField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szFundValue[21 + 1];        // �ʽ��ʲ� 
  char            szFundPrebln[21 + 1];       // �ʽ�������� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  char            szFundAvl[21 + 1];          // �ʽ���ý�� 
  char            szLwlmtFund[21 + 1];        // �ʽ����� 
  char            chFundStatus;               // �ʽ�״̬ 
  char            chCuacctAttr;               // �ʲ��˻����� �ֵ�[CUACCT_ATTR]
  char            szFundAle[21 + 1];          // �ʽ��ȡ 
  char            szFundTrdUfz[21 + 1];       // �ʽ��׽ⶳ��� 
  char            szFundBorrowBln[21 + 1];    // �ʽ��ڲ������ 
  int             iIntOrg;                    // �ڲ����� 
  char            szFundFrz[21 + 1];          // �ʽ𶳽��� 
  char            szFundTrdFrz[21 + 1];       // �ʽ��׶����� 
  char            szFundTrdOtd[21 + 1];       // �ʽ�����;��� 
  int             iExtOrg;                    // �ⲿ���� 
};

//-------------------------------10308002:���ʲ���ѯ--------------------------
struct MATRADEAPI CReqFsQryTotalAssetsField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

struct MATRADEAPI CRspFsQryTotalAssetsField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iIntOrg;                    // �ڲ����� 
  char            szMarketValue[21 + 1];      // �ʲ���ֵ 
  char            szFundValue[21 + 1];        // �ʽ��ʲ� 
  char            szStkValue[21 + 1];         // ��ֵ 
  char            szFundPrebln[21 + 1];       // �ʽ�������� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  char            szFundAvl[21 + 1];          // �ʽ���ý�� 
  char            chFundStatus;               // �ʽ�״̬ 
  char            chCuacctAttr;               // �ʲ��˻����� �ֵ�[CUACCT_ATTR]
  char            szIncome[21 + 1];           // ��ӯ�� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

//-------------------------------10308003:�ɷ��ʲ���ѯ--------------------------
struct MATRADEAPI CReqFsQryShareAssetsField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� �ʲ��˻��Ϳͻ����벻��ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� ֧�ֶ�ѡ����ʽ��ֵ1��ֵ2��ֵ3�����磺00,01,03,10,11
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
  LONGLONG        llCustCode;                 // �ͻ����� 
};

struct MATRADEAPI CRspFsQryShareAssetsField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  LONGLONG        llStkPrebln;                // ֤ȯ������� 
  LONGLONG        llStkBln;                   // ֤ȯ��� 
  LONGLONG        llStkAvl;                   // ֤ȯ�������� 
  LONGLONG        llStkQty;                   // ��ǰӵ���� 
  char            szCurrentPrice[21 + 1];     // ���¼۸� 
  char            szStkPcostRlt[21 + 1];      // �ֲֳɱ� 
  char            szMktVal[21 + 1];           // ��ֵ 
  char            szCostPrice[21 + 1];        // �ɱ��۸� 
  char            szIncome[21 + 1];           // ӯ�� 
  char            szProfitRate[21 + 1];       // ӯ������ 
  LONGLONG        llStkTrdFrz;                // ֤ȯ���׶������� 
  LONGLONG        llStkTrdUfz;                // ֤ȯ���׽ⶳ���� 
  char            szAveragePrice[21 + 1];     // ������� 
  char            chStkCls;                   // ֤ȯ��� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llStkSale;                  // ������������ 
  LONGLONG        llStkTrdOtd;                // ֤ȯ������;���� 
  LONGLONG        llStkFrz;                   // ֤ȯ�������� 
};

//-------------------------------10308004:����ί�в�ѯ--------------------------
struct MATRADEAPI CReqFsQryCurrDayOrderField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� �ʲ��˻��Ϳͻ����벻��ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chFlag;                     // ��ѯ��־ 
  char            szStkBiz[1024 + 1];         // ҵ����� �ֵ�[STK_BIZ]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chOrderMode;                // ί��ģʽ 
};

struct MATRADEAPI CRspFsQryCurrDayOrderField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  char            szOrderTime[32 + 1];        // ί��ʱ�� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chOrderStatus;              // ί��״̬ �ֵ�[ORDER_STATUS]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szBondInt[21 + 1];          // ծȯ��Ϣ 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szOrderUfzAmt[21 + 1];      // ί�нⶳ��� 
  LONGLONG        llOfferQty;                 // �걨���� 
  int             iOfferStime;                // �걨ʱ�� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  char            chIsWithdrawn;              // �ѳ�����־ �ֵ�[IS_WITHDRAWN]
  char            szMatchedAmt[21 + 1];       // �ɽ���� 
  char            szOfferRetMsg[64 + 1];      // �걨������Ϣ 
  char            szReferRate[21 + 1];        // �ο����� 
  char            szAfundAmt[21 + 1];         // A���ʽ�䶯��� 
  char            szHfundAmt[21 + 1];         // �۹��ʽ�䶯��� 
  char            szBizParam[512 + 1];        // ҵ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  char            chOrderValidFlag;           // ί����Ч��־ 
};

//-------------------------------10308006:���ճɽ���ѯ--------------------------
struct MATRADEAPI CReqFsQryCurrDayMatchField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� �ʲ��˻��Ϳͻ����벻��ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chFlag;                     // ��ѯ��־ 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
  LONGLONG        llCustCode;                 // �ͻ����� 
};

struct MATRADEAPI CRspFsQryCurrDayMatchField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szMatchedSn[16 + 1];        // �ɽ���� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            szMatchedAmt[21 + 1];       // �ѳɽ���� 
  char            chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  char            chMatchedType;              // �ɽ����� �ֵ�[MARCHED_TYPE]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
};

//-------------------------------10308053:���ճɽ�ͳ��--------------------------
struct MATRADEAPI CReqFsQryCurrDayMatchCountField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[32 + 1];            // ���װ�� ֧�ֶ�ѡ����ʽ��ֵ1��ֵ2��ֵ3�����磺00,01,03,10,11
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryCurrDayMatchCountField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  LONGLONG        llMatchedQty;               // �ɽ����� 
  char            szMatchedAmt[21 + 1];       // �ɽ���� 
  char            chMatchedType;              // �ɽ����� �ֵ�[MATCHED_TYPE]
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
};

//-------------------------------10308056:���ɽ�����������--------------------------
struct MATRADEAPI CReqFsQryMaxLimitTransferCountField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkbdLink[2 + 1];         // ������� 
  char            szTrdacctLink[20 + 1];      // �����ɶ� 
  int             iTrdDate;                   // �������� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szStkpbu[8 + 1];            // ����ϯλ 
  char            szStkprop[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            szSdStkType[2 + 1];         // �ϵ�֤ȯ��� �ֵ�[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // Ȩ������ �ֵ�[EQUIT_TYPE]
  char            chCirculType;               // ��ͨ���� �ֵ�[CIRCUL_TYPE]
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  char            chStkBizCtvFlag;            // ҵ��ת����־ 
  char            szOrderText[256 + 1];       // ί����չ 
};

struct MATRADEAPI CRspFsQryMaxLimitTransferCountField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llStkQty;                   // �ɷֹ����� 
  LONGLONG        llMaxOrerQty;               // ����ί������ 
};

//-------------------------------10308601:֤ȯ��������ί���걨--------------------------
struct MATRADEAPI CReqFsTrdTransferOrderField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  int             iOrderBsn;                  // ί������ 
  char            szClientInfo[256 + 1];      // �ն���Ϣ ����Ӳ�̺�\CPU\��������
  char            chSecurityLevel;            // ��ȫ�ֶ� 0:�ް�ȫ1:У��Ʊ��2:У������
  char            szSecurityInfo[256 + 1];    // ��ȫ��Ϣ ��ȫ�ֶ�ΪУ��Ʊ��������дƱ�ݰ�ȫ�ֶ�Ϊ����������д������������
  char            szOrderText[256 + 1];       // ί����չ ���ֶ������ҵ�񳡾���д��ʽ���£�Key1:Value1,key2:Value2,key3:Value3,...
  char            chStkBizCtvFlag;            // ҵ��ת����־ 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

struct MATRADEAPI CRspFsTrdTransferOrderField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ������ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  int             iOrderSn;                   // ί����� 
  char            chOrderStatus;              // ί��״̬ �ֵ�[ORDER_STATUS]
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

//-------------------------------10308607:֤ȯ����ί�г���--------------------------
struct MATRADEAPI CReqFsTrdTransferPurchaseCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szClientInfo[256 + 1];      // �ն���Ϣ ����Ӳ�̺�\CPU\��������
};

struct MATRADEAPI CRspFsTrdTransferPurchaseCancelField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ������ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
};

//-------------------------------10308054:�ɳ���ί�в�ѯ--------------------------
struct MATRADEAPI CReqFsQryAvlTrdTransferPurchaseCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� ֧�ֶ�ѡ����ʽ��ֵ1��ֵ2��ֵ3�����磺00,01,03,10,11
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkBiz[1024 + 1];         // ֤ȯҵ�� ���ֶ�����ҵ�����֧�ֶ�ѡ����ʽ��ֵ1��ֵ2��ֵ3�����磺100,101,206
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryAvlTrdTransferPurchaseCancelField
{
  char            szQryPos[32 + 1];           // ��λ����ѯ 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  char            szOrderTime[32 + 1];        // ί��ʱ�� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chOrderStatus;              // ί��״̬ �ֵ�[ORDER_STATUS]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szOrderUfzAmt[21 + 1];      // ί�нⶳ��� 
  LONGLONG        llOfferQty;                 // �걨���� 
  int             iOfferStime;                // �걨ʱ�� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  char            chIsWithdrawn;              // �ѳ�����־ �ֵ�[IS_WITHDRAWN]
  int             iOrderBsn;                  // ί������ 
  char            szMatchedAmt[21 + 1];       // �ɽ���� 
};

//-------------------------------10308606:֤ȯ������������--------------------------
struct MATRADEAPI CReqFsTrdQuantityCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iOrderBsn;                  // ί������ 
};

struct MATRADEAPI CRspFsTrdQuantityCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  int             iCancelRet;                 // ������� ����ֵ0����ʾ�ɹ�-1����ʾʧ��
  char            szRetInfo[256 + 1];         // ������Ϣ ����ί�н����Ϣ
};

//-------------------------------10308602:���óɱ�--------------------------
struct MATRADEAPI CReqFsResetCostField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szCostPrice[21 + 1];        // �ɱ��۸� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
};

struct MATRADEAPI CRspFsResetCostField
{
  char            szCostPrice[21 + 1];        // �޸ĺ�ɱ��� 
  char            szCurrentPrice[21 + 1];     // ���¼۸� 
  char            szMktVal[21 + 1];           // ��ֵ 
};

//-------------------------------10308015:��ծԤ���гֲ���ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBondsPreIssuePosiField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBondsPreIssuePosiField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            chBidMode;                  // �б귽ʽ �ֵ�[BID_MODE]
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iTrdDate;                   // �������� 
  int             iMatchTime;                 // �ɽ�ʱ�� 
  char            szMatchedSn[16 + 1];        // �ɽ���� 
  LONGLONG        llPositionQty;              // ԭʼ�ֲ����� 
  char            chPositionType;             // �ֲ���� �ֵ�[POSITION_TYPE]
  char            szPositionPrice[21 + 1];    // �ֲּ۸� 
  char            szDepositAmt[21 + 1];       // ԭʼ��Լ��֤�� 
  char            szMarginAmt[21 + 1];        // �۲֤�� 
  char            szProfitAmt[21 + 1];        // ������ 
  char            szCustRate[21 + 1];         // ԭʼ��Լ��֤����� 
  LONGLONG        llCloseQty;                 // ƽ������ 
};

//-------------------------------10308016:��ծԤ���пͻ�ƽ����ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryCustBondsPreIssueClosePosiField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryCustBondsPreIssueClosePosiField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ���ֺ�ͬ��� 
  int             iSettDate;                  // �������� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iOpenDate;                  // �������� 
  int             iCloseDate;                 // ƽ������ 
  char            szCloseOrderId[10 + 1];     // ƽ�ֺ�ͬ��� 
  char            chCloseType;                // ƽ����� �ֵ�[CLOSE_TYPE]
  char            szClosePrice[21 + 1];       // ƽ�ּ۸� 
  char            szBackDeposit[21 + 1];      // ������Լ��֤�� 
  char            szMarginAmt[21 + 1];        // �۲֤�� 
  char            szProfitAmt[21 + 1];        // ������ 
  LONGLONG        llCloseQty;                 // ƽ������ 
};

//-------------------------------10308017:��ծԤ���в�Ʒ��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBondsPreProdInfoField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBondsPreProdInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iOpenDate;                  // �������� 
  int             iLastDate;                  // ��������� 
  char            chBidMode;                  // �б귽ʽ �ֵ�[BID_MODE]
  char            szFacePrice[21 + 1];        // ��ֵ�۸� 
  char            szBenchPrice[21 + 1];       // ��׼�۸� 
  char            szDepositRate[21 + 1];      // �����Լ��֤����� 
  LONGLONG        llRefDuration;              // �ο����� 
  LONGLONG        llIssueAmount;              // ���ڷ����� 
  char            chIssueStatus;              // ����״̬ �ֵ�[ISSUE_STATUS]
};

//-------------------------------10418004:�¹���Ų�ѯ--------------------------
struct MATRADEAPI CReqFsQryNewShareDistributionField
{
  LONGLONG        llCuacctCode;               // �ʽ��˺� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
  int             iBgnDate;                   // ��ʼ���� ��������SETT_DATE
  int             iEndDate;                   // �������� ��������SETT_DATE
};

struct MATRADEAPI CRspFsQryNewShareDistributionField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iSettDate;                  // �������� 
  LONGLONG        llSerialNo;                 // ��ˮ��� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʽ��˺� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���� �ֵ�[CURRENCY]
  char            szOrderId[32 + 1];          // ��ͬ��� 
  char            chAssignFlag;               // �����ǩ��־ �ֵ�[ASSIGN_FLAG]
  char            szBeginAssignNo[32 + 1];    // ��ʼ��� 
  LONGLONG        llAssignNoNum;              // ��Ÿ��� 
  LONGLONG        llOrderQty;                 // ί������ 
};

//-------------------------------10418005:�¹���ǩ��ѯ--------------------------
struct MATRADEAPI CReqFsQryNewShareWinningField
{
  LONGLONG        llCuacctCode;               // �ʽ��˺� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
  int             iBgnDate;                   // �������� ��������SETT_DATE
  int             iEndDate;                   // ��ǩ���� ��������SETT_DATE
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szOrderId[32 + 1];          // ��ͬ��� 
};

struct MATRADEAPI CRspFsQryNewShareWinningField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iSuccessDate;               // ��ǩ���� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʽ��˺� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iIntOrg;                    // �ڲ����� 
  char            chCurrency;                 // ���� �ֵ�[CURRENCY]
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkCodeLink[10 + 1];      // ֤ȯ���루������ 
  char            szOrderId[32 + 1];          // ��ͬ��� 
  LONGLONG        llSuccessNo;                // ��ǩ���� 
  LONGLONG        llSuccessQty;               // ��ǩ���� 
  char            szSubscribePrice[21 + 1];   // �Ϲ��۸� 
};

//-------------------------------10418006:�¹��깺��ֵ��Ȳ�ѯ--------------------------
struct MATRADEAPI CReqFsQryNewShareSubscriptionLimitField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ����뼰֤ȯ�˻�����ͬʱΪ��
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� �ͻ����뼰֤ȯ�˻�����ͬʱΪ��
  int             iIntOrg;                    // �ڲ����� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryNewShareSubscriptionLimitField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  int             iIntOrg;                    // �������� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  LONGLONG        llCustQuota;                // �ͻ���ֵ��� 
  LONGLONG        llKcCustQuota;              // �ƴ����깺��� 
  int             iReceiveDate;               // �������� 
  char            szRemark[32 + 1];           // ��ע 
};

//-------------------------------10418007:�¹���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryNewShareInfoField
{
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
};

struct MATRADEAPI CRspFsQryNewShareInfoField
{
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szIssuePrice[21 + 1];       // ���м۸� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llStkUplmtQty;              // ������� 
  LONGLONG        llStkLwlmtQty;              // ������� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iStkBiz;                    // ���ױ�ʶ �ֵ�[STK_BIZ]
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
};

//-------------------------------10308019:���ۻع�Ʒ����Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductInfoField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductInfoField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szRepchCode[8 + 1];         // ծȯ�ع����� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szQuoteYearRate[21 + 1];    // ���ۻع��������� 
  char            szQuoteBreakRate[21 + 1];   // ������ǰ���������� 
  char            chQuoteAutoBuyFlag;         // �����Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  char            chQuoteTradeFlag;           // ���ۻع����ױ�־ �ֵ�[QUOTE_TRADE_FLAG]
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  char            chProdmode;                 // ��Ʒģʽ �ֵ�[PROD_MODE]
  char            chIsvalid;                  // ��Ʒ�Ƿ���Ч �ֵ�[PROD_IS_VALID]
  int             iRepchDays;                 // ծȯ�ع����� 
  int             iBegintDate;                // �������� 
  int             iEndDate;                   // �������� 
  int             iNextBeginDate;             // ���ַ������� 
  int             iNextEndDate;               // ���ֵ������� 
  int             iNextPreBookDate;           // ����ԤԼ��ʼ���� 
  char            szNextYearRate[21 + 1];     // ���ֵ����������� 
  char            szNextBreakRate[21 + 1];    // ������ǰ��ֹ������ 
};

//-------------------------------10308020:���ۻع���Ѻ����������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductInfoConversionRateField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductInfoConversionRateField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szCodeCvtrate[21 + 1];      // ������ 
  char            szStkLofNav[21 + 1];        // ����ֵ 
};

//-------------------------------10308021:���ۻع���Ѻ���ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �ɶ����� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llStkPrebln;                // ֤ȯ������� 
  LONGLONG        llStkBln;                   // ֤ȯ��� 
  LONGLONG        llStkAvl;                   // ֤ȯ�������� 
};

//-------------------------------10308608:���ۻع���Լչ��--------------------------
struct MATRADEAPI CReqFsQuoteRepurchaseContflagField
{
  int             iTrdDate;                   // �������� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  LONGLONG        llContQty;                  // չ������ 
};

struct MATRADEAPI CRspFsQuoteRepurchaseContflagField
{
};

//-------------------------------10308022:���ۻع���Ѻ�ʽ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchasePledgeFundField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �ɶ����� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchasePledgeFundField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szBondCashPrebln[21 + 1];   // ������� 
  char            szBondCashBln[21 + 1];      // ��������� 
  char            szBondCashAvl[21 + 1];      // ������������ 
};

//-------------------------------10308023:���ۻع�ԤԼ��ˮ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseReservationDetailField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseReservationDetailField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iPreExecDate;               // ��ǰ��ִֹ������ 
  LONGLONG        llMatchQty;                 // ��ǰ��ֹ���� 
  char            szMatchAmt[21 + 1];         // ��ǰ��ֹ��� 
  LONGLONG        llSerialNo;                 // ԤԼ��ˮ�� 
  int             iRepchDate;                 // �������� 
  int             iOccurDate;                 // �������� 
  char            szOccurTime[32 + 1];        // ����ʱ�� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
  char            szMatchId[16 + 1];          // �ɽ���� 
  int             iMatchDate;                 // �ɽ����� 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            chStatus;                   // ״̬ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308610:���ۻع�ԤԼ--------------------------
struct MATRADEAPI CReqFsQuoteRepurchaseReservationField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
  char            chActType;                  // �������� �ֵ�[ACT_TYPE]
  int             iPreExecDate;               // ԤԼִ������ 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llMatchQty;                 // ��ǰ��ֹ���� 
  char            szMatchAmt[21 + 1];         // ��ǰ��ֹ��� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  int             iIntOrg;                    // �ڲ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iRepchDate;                 // �������� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  int             iMatchDate;                 // �ɽ����� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  LONGLONG        llSerialNo;                 // ԤԼ��ˮ�� 
};

struct MATRADEAPI CRspFsQuoteRepurchaseReservationField
{
};

//-------------------------------10308024:���ۻع�����ǰ���غ�Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValPreRepurchaseContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValPreRepurchaseContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            szAdvRepchAmt[21 + 1];      // ��ǰ���ؽ�� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308025:���ۻع���չ��ȡ����Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValFlagCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValFlagCancelField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            szAdvRepchAmt[21 + 1];      // ��ǰ���ؽ�� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308026:���ۻع��ɵ����Զ�չ�ں�Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAutoValFlagContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAutoValFlagContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            szAdvRepchAmt[21 + 1];      // ��ǰ���ؽ�� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308027:���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            szAdvRepchAmt[21 + 1];      // ��ǰ���ؽ�� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  LONGLONG        llRemainQty;                // ����ǰ��ֹ���� 
  LONGLONG        llSerialNo;                 // ԤԼ��ˮ�� 
};

//-------------------------------10308028:���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlCancelStaTransferPreContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlCancelStaTransferPreContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iPreExecDate;               // ��ǰ��ִֹ������ 
  LONGLONG        llMatchQty;                 // ��ǰ��ֹ���� 
  char            szMatchAmt[21 + 1];         // ��ǰ��ֹ��� 
  LONGLONG        llSerialNo;                 // ԤԼ��ˮ�� 
  int             iRepchDate;                 // �������� 
  int             iOccurDate;                 // �������� 
  char            szOccurTime[32 + 1];        // ����ʱ�� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chPreBookType;              // ԤԼ���� �ֵ�[PRE_BOOK_TYPE]
  char            szMatchId[16 + 1];          // �ɽ���� 
  int             iMatchDate;                 // �ɽ����� 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            chStatus;                   // ״̬ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308029:���ۻع���չ�ں�Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValFlagContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValFlagContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  LONGLONG        llContQtyReal;              // ��չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
};

//-------------------------------10308074:���ۻع�����ǰ��ֹԤԼ��Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlAdvanceEndPreContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlAdvanceEndPreContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  char            chStkCls;                   // ֤ȯ��� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // ������������ 
  char            szQuoteBreakRate[21 + 1];   // ��ǰ��ֹ������ 
  char            chQuoteAutoBuyFlag;         // �Զ�չ�ڱ�־ �ֵ�[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // չ�ڽ�ֹ���� 
  char            szAdvRepchAmt[21 + 1];      // ��ǰ���ؽ�� 
  int             iSubContractRepchDate;      // չ�ں��Ӻ�Լ�������� 
  LONGLONG        llContQty;                  // �Զ�չ������ 
  char            szProdcode[12 + 1];         // ��Ʒ���� 
  LONGLONG        llRemainQty;                // ����ǰ��ֹ���� 
};

//-------------------------------10308615:������Ȩֱ�ӻ���--------------------------
struct MATRADEAPI CReqFsFinancingRightDirectRepaymentField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ֤ȯҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szRepayContractAmt[21 + 1]; // ������� 
  char            szRemark[128 + 1];          // ��ע 
};

struct MATRADEAPI CRspFsFinancingRightDirectRepaymentField
{
  char            szRealRepayAmt[21 + 1];     // ʵ�ʻ����� 
};

//-------------------------------10308030:������Ȩ��Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryFinancingRightcontractField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chContractStatus;           // ��Լ״̬ �ֵ�[CONTRACT_STATUS]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryFinancingRightcontractField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            szContractSno[32 + 1];      // ��Լ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʽ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llWrntQty;                  // ��Ȩ���� 
  char            szWrntAmt[21 + 1];          // ��Ȩ��� 
  char            chContractStatus;           // ��Լ״̬ �ֵ�[CONTRACT_STATUS]
  char            szTaxAmt[21 + 1];           // ��˰��� 
  int             iExpiringDate;              // �������� 
  int             iClosingDate;               // �˽����� 
  char            szRepayAmt[21 + 1];         // �ѹ黹���� 
  char            szRltRepayAmt[21 + 1];      // ʵʱ�黹���� 
  int             iIntCalDate;                // �ϴμ�Ϣ���� 
  char            szInitIntRate[21 + 1];      // ��ʼ��Ϣ������ 
  char            szIntRate[21 + 1];          // ��Ϣ������ 
  char            szAwardInt[21 + 1];         // �Ѽ���Ϣ 
  char            szDueInt[21 + 1];           // Ӧ����Ϣ 
  char            szRepayInt[21 + 1];         // �ѻ���Ϣ 
  char            szInitPuniRate[21 + 1];     // ��ʼΥԼ������ 
  char            szPuniRate[21 + 1];         // ΥԼ������ 
  char            szAwardPuniAmt[21 + 1];     // �Ѽ�ΥԼ�� 
  char            szDuePuniAmt[21 + 1];       // Ӧ��ΥԼ�� 
  char            szRepayPuniAmt[21 + 1];     // �ѻ�ΥԼ�� 
  char            chCycleType;                // ��Ϣ�Զ��������� 
  int             iCycleValue;                // ��Ϣ�Զ����������� 
};

//-------------------------------10308085:������Ȩ�ɻ���ծ��ѯ--------------------------
struct MATRADEAPI CReqFsQryFinancingRightCanrePayDebtField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
};

struct MATRADEAPI CRspFsQryFinancingRightCanrePayDebtField
{
  LONGLONG        llDebt;                     // �ɻ���ծ 
  LONGLONG        llRltDebt;                  // ʵʱ�ѻ���ծ 
};

//-------------------------------10308087:������Ȩ������ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryFinancingRightPayDetailField
{
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chRepayType;                // �������� �ֵ�[WRNT_REPAY_TYPE]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryFinancingRightPayDetailField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            szContractSno[32 + 1];      // ��Լ��� 
  char            chRepayType;                // �������� �ֵ�[WRNT_REPAY_TYPE]
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  char            szRepayAmt[21 + 1];         // ��Լ���𻹿� 
  char            szRepayInt[21 + 1];         // ��Լ��Ϣ���� 
  char            szRepayPuniAmt[21 + 1];     // ΥԼ�𻹿� 
};

//-------------------------------10308031:����ͶƱ�ɶ������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingTrdacctsConferenceInfoField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            szCompanyCode[8 + 1];       // ��˾���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryNetworkVotingTrdacctsConferenceInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szCompanyCode[8 + 1];       // ��˾���� 
  char            szCompanyInfo[8 + 1];       // ��˾��Ϣ 
  int             iMeetingDateBegin;          // �ɶ���Ὺʼ�� 
  int             iMeetingDateEnd;            // �ɶ��������� 
  char            chMeetingType;              // �ɶ�������� �ֵ�[MEETING_TYPE]
  char            szMeetingDesc[512 + 1];     // �ɶ�������� 
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
};

//-------------------------------10308032:����ͶƱͶƱ�����ѯ--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingResultField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryNetworkVotingResultField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iVoteDate;                  // ͶƱ���� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szVoteStkCode[8 + 1];       // ��Ʒ���� 
  char            szVoteResult[128 + 1];      // ͶƱ��� 
  char            szVoteId[10 + 1];           // �鰸��� 
  char            szVoteInfo[256 + 1];        // �鰸��Ϣ 
  char            chVoteType;                 // �鰸���� �ֵ�[VOTE_TYPE]
};

//-------------------------------10308033:����ͶƱ�鰸��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingMotionInfoField
{
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            szVoteId[10 + 1];           // �鰸��� 
  char            chVoteType;                 // �鰸���� �ֵ�[VOTE_TYPE]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryNetworkVotingMotionInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            szVoteId[10 + 1];           // �鰸��� 
  char            szVoteInfo[256 + 1];        // �鰸��Ϣ 
  char            chVoteType;                 // �鰸���� �ֵ�[VOTE_TYPE]
  LONGLONG        llVoteLeijino;              // �ۻ�ͶƱӦѡ�� 
  char            szVoteRole[64 + 1];         // �ɶ���� 
  char            szVoteRelation[128 + 1];    // �鰸��ϵ 
  char            szVoteStkCodes[512 + 1];    // ͶƱ��Ʒ(��) 
};

//-------------------------------10308059:����ͶƱ��Ʒ��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingProdInfoField
{
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            szVoteStkCode[8 + 1];       // ��Ʒ���� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryNetworkVotingProdInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szMeetingSeq[10 + 1];       // �ɶ������� 
  char            szVoteStkCode[8 + 1];       // ��Ʒ���� 
  int             iRegDate;                   // ��Ȩ�Ǽ��� 
  int             iLastTrdDate;               // ������� 
  char            chVoteStkType;              // �ɷ���� �ֵ�[VOTE_STK_TYPE]
  char            szStkSabbr[8 + 1];          // ��Ʊ��� 
};

//-------------------------------10308051:�۹�ͨ�۲���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHkCuaccDifferencetField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szPrice[21 + 1];            // �۸� 
};

struct MATRADEAPI CRspFsQryHkCuaccDifferencetField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            chStkCls;                   // ֤ȯ��� �ֵ�[CURRENCY]
  char            szBgnPrice[21 + 1];         // �۸����� 
  char            szEndPrice[21 + 1];         // �۸����� 
  char            szStkSpread[21 + 1];        // �۲��λ�� 
  char            szMinBgnPrice[21 + 1];      // ��ͼ۸����� 
  char            szMaxEndPrice[21 + 1];      // ��߼۸����� 
  int             iPriceUnitNum;              // �۲����� 
};

//-------------------------------10308050:�۹�ͨ������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHkRationInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
};

struct MATRADEAPI CRspFsQryHkRationInfoField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szBuyRate[21 + 1];          // ����ο����� 
  char            szDayBuyRiseRate[21 + 1];   // �ռ�����ο����ʸ������� 
  char            szNightBuyRiseRate[21 + 1]; // ҹ������ο����ʸ������� 
  char            szSaleRate[21 + 1];         // �����ο����� 
  char            szDaySaleRiseRate[21 + 1];  // �ռ������ο����ʸ������� 
  char            szNightSaleRiseRate[21 + 1];// ҹ�������ο����ʸ������� 
  char            szMidRate[21 + 1];          // �м�ο����� 
  char            szBuySettRate[21 + 1];      // ���������� 
};

//-------------------------------10308061:�۹�ͨ�г�״̬��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHkShareMarketInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
};

struct MATRADEAPI CRspFsQryHkShareMarketInfoField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szThresholdQuota[21 + 1];   // ÿ�ճ�ʼ��� 
  char            szPosQuota[21 + 1];         // ����ʣ���� 
  char            chQuotaStatus;              // ���״̬ �ֵ�[QUOTA_STATUS]
  char            chMktTrdStatus;             // �г�����״̬ �ֵ�[MKT_TRD_STATUS]
};

//-------------------------------10308034:�۹�ͨ���֤ȯ��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHkShareCommTrdInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryHkShareCommTrdInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chLotTrdStatus;             // ���ֽ���״̬ �ֵ�[LOT_TRD_STATUS]
  char            chOddTrdStatus;             // ��ɽ���״̬ �ֵ�[ODD_TRD_STATUS]
  char            chObjectFlag;               // ��ı�־ �ֵ�[OBJECT_FLAG]
  int             iUpdDate;                   // �������� 
};

//-------------------------------10308035:�۹�ͨ����������ѯ--------------------------
struct MATRADEAPI CReqFsQryHkShareTransferDateField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // �������� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryHkShareTransferDateField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iPhysicalDate;              // �������� 
  char            chTrdDateFlag;              // �����ձ�־ �ֵ�[TRD_DATE_FLAG]
  char            chSettDateFlag;             // �����ձ�־ �ֵ�[SETT_DATE_FLAG]
};

//-------------------------------10308047:�۹�ͨ�ʽ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHkShareFundField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
};

struct MATRADEAPI CRspFsQryHkShareFundField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szFundAvl[21 + 1];          // �۹�ͨ�ʽ���� 
  char            szFundTrdFrz[21 + 1];       // �۹�ͨ�ʽ��׶��� 
  char            szFundTrdUfz[21 + 1];       // �۹�ͨ�ʽ��׽ⶳ 
  char            szFundTrdOtd[21 + 1];       // �۹�ͨ�ʽ�����; 
  char            szFundTrdBln[21 + 1];       // �۹�ͨ�ʽ������� 
};

//-------------------------------10308036:���ڽ���������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBlockTradingMktField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBlockTradingMktField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chMkCls;                    // ������� �ֵ�[MK_CLS]
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            chSide;                     // �������� �ֵ�[SIDE]
  char            szConfirmId[8 + 1];         // Լ���� 
  int             iOrderTime;                 // ί��ʱ�� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  LONGLONG        llMatchedQty;               // �ɽ����� 
  char            szContactInfo[50 + 1];      // ��ϵ��Ϣ 
  char            chExecType;                 // ִ������ �ֵ�[EXEC_TYPE]
};

//-------------------------------10308108:�̺������ѯ--------------------------
struct MATRADEAPI CReqFsQrylosingMktSecondboardField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chStkPhQuoteType;           // �̺󱨼����� �ֵ�[PH_QUOTE_TYPE]
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQrylosingMktSecondboardField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            chStkex;                    // �г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkPhhqlb[5 + 1];         // ������� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  char            chStkPhQuoteType;           // �������� �ֵ�[PH_QUOTE_TYPE]
  char            szStkPhQuotePrice[21 + 1];  // �۸� 
  LONGLONG        llStkMatchedQty;            // �ɽ����� 
  char            szStkMatchedAmt[21 + 1];    // �ɽ���� 
  LONGLONG        llStkMatchedCount;          // �ɽ����� 
  LONGLONG        llStkBuyLeftQty;            // ����ʣ������ 
  LONGLONG        llStkSaleLeftQty;           // ����ʣ������ 
  char            szUpdTime[32 + 1];          // ����ʱ�� 
  char            szRemark[56 + 1];           // ��ע 
};

//-------------------------------10308037:����Ͷ���߿ɽ���֤ȯ��ѯ--------------------------
struct MATRADEAPI CReqFsQryRestrictedInvestorValTransferTrdField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryRestrictedInvestorValTransferTrdField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
};

//-------------------------------10308067:��ת�ֲ���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkTranLayeringInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryStkTranLayeringInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szStkLayerInfor[16 + 1];    // �ֲ���Ϣ 
};

//-------------------------------10308109:��תȷ����Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkTranInquiryConfirmInfoField
{
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            chQnqrStatus;               // ȷ��״̬ �ֵ�[QNQR_STATUS]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryStkTranInquiryConfirmInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  LONGLONG        llMatchQty2;                // �ɽ�����2 
  char            chQnqrStatus;               // ȷ��״̬ �ֵ�[QNQR_STATUS]
  int             iMatchDate;                 // �ɽ����� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szRemark[1024 + 1];         // ��ע��Ϣ 
};

//-------------------------------10308110:��ת����ҵ����Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkTranIssuingBusinessInfoField
{
  char            chIssuStatus;               // ����״̬ �ֵ�[ISSU_STATUS]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryStkTranIssuingBusinessInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szStkCode[8 + 1];           // ���д��� 
  char            szStkName[32 + 1];          // ֤ȯ���� 
  char            szStkUndlCode[8 + 1];       // ��Ĵ��� 
  char            chIssuStatus;               // ����״̬ �ֵ�[ISSU_STATUS]
  char            szStkSpread[21 + 1];        // �۲��λ�� 
  LONGLONG        llStkLotSize;               // ÿ������ 
  char            szLwlmtPrice[21 + 1];       // ����걨�� 
  char            szUplmtPrice[21 + 1];       // ����걨�� 
  LONGLONG        llLwlmtQty;                 // ����걨���� 
  LONGLONG        llUplmtQty;                 // ����걨���� 
  LONGLONG        llPublishCount;             // ���η����� 
  int             iPublishDate;               // �������� 
  int             iRfqBgnDate;                // ѯ�ۿ�ʼ���� 
  int             iRfqEndDate;                // ѯ�۽�ֹ���� 
};

//-------------------------------10308038:��Ʊ��Ѻ��Լ�ܱ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkPledgeContractField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            chRepchFlag;                // ���ر�־ 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryStkPledgeContractField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szGpzysno[32 + 1];          // ��Լ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  int             iLoanerIntOrg;              // ������Ӫҵ������ 
  char            szLoanerStkpbu[8 + 1];      // ������ϯλ 
  char            szLoanerTrdacct[20 + 1];    // �����˹ɶ� 
  char            szLoanerName[40 + 1];       // ���������� 
  char            chLoanerKind;               // ���������� �ֵ�[LOANER_KIND]
  int             iRepchDate;                 // �ع����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szBackAmt[21 + 1];          // �ѻ���� 
  char            szBackAmtReal[21 + 1];      // ʵʱ���ؽ�� 
  char            szDueIntr[21 + 1];          // Ӧ����Ϣ 
  char            szPayInterest[21 + 1];      // �ѻ���Ϣ 
  char            szGpzybzRate[21 + 1];       // ��Լ���ϱ��� 
  char            szWarningRate[21 + 1];      // Ԥ���߱��� 
  char            szLowestRate[21 + 1];       // ����߱��� 
  char            szEndingRate[21 + 1];       // ��ֹ�߱��� 
  char            szSafeRate[21 + 1];         // ���ձ��� 
  char            chSafeStatus;               // ����״̬ �ֵ�[SAFE_STATUS]
  char            szFundusedo[32 + 1];        // �ʽ���; �ֵ�[FUND_PROC_METHOD]
  char            chRiskStatus;               // ΥԼ״̬ �ֵ�[RISK_STATUS]
  char            chStatus;                   // ״̬ �ֵ�[STATUS]
};

//-------------------------------10308039:��Ʊ��Ѻ��Լ��ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkPledgeContractDetailField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szGpzysno[32 + 1];          // ��Լ��� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryStkPledgeContractDetailField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szGpzysno[32 + 1];          // ��Լ��� 
  int             iItemno;                    // ��Լ��� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szTrdacct[20 + 1];          // �����˻� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  LONGLONG        llBonusqtyUncome;           // ��Ѻ��;���� 
  LONGLONG        llBonusqty;                 // ������� 
  LONGLONG        llBackqty;                  // ��Ѻ���� 
  LONGLONG        llBackqtyReal;              // �����Ѻ���� 
  LONGLONG        llAvlBackQty;               // �ɽ�Ѻ���� 
  char            szBonusAmt[21 + 1];         // ������� 
  char            szBackbonusAmt[21 + 1];     // ������Ѻ��� 
  char            szBackbonusAmtReal[21 + 1]; // �����Ѻ��� 
  char            szAvlBonusAmt[21 + 1];      // �ɽ�Ѻ��� 
  LONGLONG        llWysbqty;                  // ΥԼ���걨�ɷ����� 
  LONGLONG        llWyczqty;                  // ΥԼ����ί������ 
  LONGLONG        llWyczmatchqty;             // ΥԼ���������ɽ����� 
  int             iOrderDate;                 // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  int             iStartorderdate;            // ��ʼ��Ѻ���� 
  char            szStartOrderId[10 + 1];     // ��ʼ��ͬ��� 
  char            szStartMatchCode[16 + 1];   // ��ʼ�ɽ���� 
  char            szStkprop[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            szSdStkType[2 + 1];         // �ϵ�֤ȯ��� �ֵ�[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // Ȩ������ �ֵ�[EQUIT_TYPE]
  char            szInitYear[4 + 1];          // ������� 
  char            chCirculType;               // ��ͨ���� �ֵ�[CIRCUL_TYPE]
  char            chStatus;                   // ״̬ �ֵ�[STATUS]
};

//-------------------------------10308062:��Ʊ��Ѻ��ʼ���ײο�����ѯ--------------------------
struct MATRADEAPI CReqFsQryStkPledgeInitTrdReferAmtField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szStkprop[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            chCirculType;               // ��ͨ���� �ֵ�[CIRCUL_TYPE]
};

struct MATRADEAPI CRspFsQryStkPledgeInitTrdReferAmtField
{
  char            szReferenceAtm[21 + 1];     // �ο���� 
};

//-------------------------------10308063:��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkPledgeQuotaInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
};

struct MATRADEAPI CRspFsQryStkPledgeQuotaInfoField
{
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llStkAvl;                   // ֤ȯ�������� 
  char            szStkprop[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            szSdStkType[2 + 1];         // �ϵ�֤ȯ��� �ֵ�[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // Ȩ������ �ֵ�[EQUIT_TYPE]
  char            chCirculType;               // ��ͨ���� �ֵ�[CIRCUL_TYPE]
};

//-------------------------------10308076:��Ʊ��Ѻ���֤ȯ��ѯ--------------------------
struct MATRADEAPI CReqFsQryStkPledgeTrdField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryStkPledgeTrdField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szCodeCvtrate[21 + 1];      // ������ 
  char            szLimitCvtrate[21 + 1];     // ���۹������� 
  LONGLONG        llTrdMinQty;                // ����������� 
  char            szMinAmt[21 + 1];           // ���ٳ�ʼ��� 
  char            chPriceType;                // ��ʼ�۸����� 0: ������1: ����2: ���¼�3: ��ֵ����Ʊ1Ԫ4: ǰ20������������ƽ����
  LONGLONG        llCountLimit;               // �ܶ�� 
};

//-------------------------------10308077:�����˹�Ʊ��Ѻ��Ȳ�ѯ--------------------------
struct MATRADEAPI CReqFsQryLenderStkPledgeQuotaField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szLoanerTrdacct[20 + 1];    // �����˹ɶ� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryLenderStkPledgeQuotaField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szLoanerTrdacct[20 + 1];    // �����˹ɶ� 
  char            szLoanerName[40 + 1];       // ���������� 
  char            chLoanerKind;               // ���������� �ֵ�[LOANER_KIND]
  char            szCountLimit[21 + 1];       // �ܶ�� 
  char            chLoanerStatus;             // ����״̬ 0: ���� 1: ��ֹ
};

//-------------------------------10308040:����ծȯ��ѺʽЭ��ع���Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryszBondPledgeRepurchaseContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chRepchStatus;              // ��Լ״̬ �ֵ�[REPCH_STATUS_Z]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryszBondPledgeRepurchaseContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szStkprop[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            szBonusAmt[21 + 1];         // ������� 
  int             iRepchDate;                 // �������� 
  char            szRepchAmt[21 + 1];         // ���ؽ�� 
  char            szDueRate[21 + 1];          // ������ 
  int             iRealRepchDate;             // ʵ�ʹ������� 
  char            szRealRepchAmt[21 + 1];     // ʵ�ʹ��ؽ�� 
  char            szRealDueRate[21 + 1];      // ʵ�������� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // ��Լ״̬ �ֵ�[REPCH_STATUS_Z]
  char            szOpptStkpbu[8 + 1];        // ���ַ����׵�Ԫ 
  char            szOpptTrdacct[20 + 1];      // ���ַ��ɶ��˻� 
  char            szOriginalContractId[32 + 1];// ��ʼ��Լ��� 
};

//-------------------------------10308041:ծȯ��ѺʽЭ��ع����֤ȯ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBondPledgeRepurchaseTrdField
{
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBondPledgeRepurchaseTrdField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chPriceType;                // �۸����� �ֵ�[PRICE_TYPE]
  char            szCodeCvtrate[21 + 1];      // ������ 
  LONGLONG        llTrdMinQty;                // ���ٽ������� 
  char            szOrderPrice[21 + 1];       // ��ʼ���׼۸� 
  char            szMinAmt[21 + 1];           // ��С���׽�� 
  int             iOccupedDate;               // ������ֹ�� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
};

//-------------------------------10308613:�Ϻ�Э��ع������ѯ�걨--------------------------
struct MATRADEAPI CReqFsshContRepurchaseMarketField
{
  char            chHqCls;                    // ������� 0 �C δ����Э��1-�ǹ�������2-��������
  char            chQueryType;                // ��ѯ���� HQ_CLSΪ0 ʱ��Ч1 ���ڽ����ѯ����2 ����������ѯ����3�����Ѻ��ѯ����4 ��ȯ�걨��ѯ����5 ��ǰ��ֹ��ѯ����
  char            chRepchStatus;              // �ع�״̬ HQ_CLSΪ0 ʱ��Ч3 ���յ���(��ѯ���͵��ڽ���͵�������ʱ��д3)4 δ���� (��ѯ����Ϊ��ȯ�걨����ǰ��ֹʱ��д4)5 �ѹ��� (��ѯ����Ϊ�����Ѻʱ��д5)
  LONGLONG        llBeginSn;                  // ��ʼ��� ��ʼ��ţ�ȫ�г�����������������������СֵΪ0�����ֵΪ1000000000
  char            szCompanyId[3 + 1];         // �����̴��� HQ_CLSΪ0 ʱΪ���ع�������Ϊ��ѯ�������̴���
  char            szOperCode[6 + 1];          // ����Ա���� HQ_CLSΪ0 ʱΪ���ع�������Ϊ��ѯ������Ա����
  char            szOpptCompanyId[3 + 1];     // ��ع��������̴��� HQ_CLSΪ0 ʱ��Ч
};

struct MATRADEAPI CRspFsshContRepurchaseMarketField
{
};

//-------------------------------10308072:�Ϻ�Э��ع��ǹ������������ѯ--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseClsMarketField
{
  char            szQuoteRefId[10 + 1];       // ���������������� 
  char            chMdUpdateAction;           // ����������� �ֵ�[MD_UPDATE_ACTION]
  char            chOrdType;                  // �����걨���� �ֵ�[ORD_TYPE]
  char            chSide;                     // �������� �ֵ�[SIDE_R]
  char            szSecurityId[6 + 1];        // ��Ѻȯ���� 
  char            chOrderRestrictions;        // ������������ �ֵ�[ORDER_RESTRICTIONS]
  char            szCompanyId[3 + 1];         // ���������̴��� 
  char            szOpptTrdacctName[10 + 1];  // ���ַ������˻����� 
  char            szOpptOperCode[6 + 1];      // ���ַ�����Ա���� 
  char            szApplReqId[10 + 1];        // ��ѯ������ 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryshContRepurchaseClsMarketField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  char            szApplReqId[10 + 1];        // ��ѯ������ 
  int             iRecSn;                     // ��¼�� 
  int             iEndSeqNo;                  // ����������� 
  char            szQuoteRefId[10 + 1];       // ���������������� 
  char            chMdUpdateAction;           // ����������� �ֵ�[MD_UPDATE_ACTION]
  char            chOrdType;                  // �����걨���� �ֵ�[ORD_TYPE]
  char            szPrice[21 + 1];            // �ع����� 
  int             iRepurchaseTerm;            // �ع����� 
  int             iUaInterestAccrualDays;     // ʵ��ռ������ 
  int             iSettlDate;                 // �״ν����� 
  int             iMaturityDate;              // �ع������� 
  int             iSettlDate2;                // ���ڽ����� 
  char            chSide;                     // �������� �ֵ�[SIDE_R]
  char            szSecurityId[6 + 1];        // ��Ѻȯ���� 
  char            szSecurityName[8 + 1];      // ��Ѻȯ��� 
  char            szUnderlyingSecurityId[6 + 1];// ԭ��Ѻȯ���� 
  LONGLONG        llOrderQty;                 // ��Ѻȯ���� 
  char            szContractMultiplier[21 + 1];// ������� 
  char            szTotalValueTraded[21 + 1]; // �ɽ���� 
  char            szUnderlyingQty[21 + 1];    // ԭ�ɽ��걨�ɽ���� 
  char            szAccruedInterestAmt[21 + 1];// �ع���Ϣ 
  char            chOrderRestrictions;        // ������������ �ֵ�[ORDER_RESTRICTIONS]
  int             iExecRefId;                 // �״��걨ʱ�������ɽ���� 
  char            szCompanyId[3 + 1];         // ���������̴��� 
  char            szOrigTradeDate[8 + 1];     // �ɽ��걨ʱ�������ɽ����� 
  char            szOpptCompanyName[16 + 1];  // ���ַ������̼�� 
  char            szOpptTrdacctName[10 + 1];  // ���ַ������˻����� 
  char            szOpptOperCode[6 + 1];      // ���ַ�����Ա���� 
  char            szSettlCurrAmt[21 + 1];     // ���ڽ����� 
  char            szLastQty[21 + 1];          // ��Ѻȯ���ܶ�ϼ� 
};

//-------------------------------10308073:�Ϻ�Э��ع��������������ѯ--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseOpenMarketField
{
  char            szQuoteRefId[10 + 1];       // ���������������� 
  char            chMdUpdateAction;           // ����������� �ֵ�[MD_UPDATE_ACTION]
  char            chOrdType;                  // �����걨���� �ֵ�[ORD_TYPE]
  char            chSide;                     // �������� �ֵ�[SIDE_R]
  char            szSecurityId[6 + 1];        // ��Ѻȯ���� 
  char            szOperCode[6 + 1];          // ���𷽽���Ա���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryshContRepurchaseOpenMarketField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  char            szApplReqId[10 + 1];        // ��ѯ������ 
  int             iRecSn;                     // ��¼�� 
  int             iEndSeqNo;                  // ����������� 
  char            szQuoteRefId[10 + 1];       // ���������������� 
  char            chMdUpdateAction;           // ����������� �ֵ�[MD_UPDATE_ACTION]
  char            chOrdType;                  // �����걨���� �ֵ�[ORD_TYPE]
  char            chSide;                     // �������� �ֵ�[SIDE_R]
  char            szSecurityId[6 + 1];        // ��Ѻȯ���� 
  char            szSecurityName[8 + 1];      // ��Ѻȯ��� 
  LONGLONG        llOrderQty;                 // ��Ѻȯ���� 
  char            szContractMultiplier[21 + 1];// ������� 
  char            szPrice[21 + 1];            // �ع����� 
  int             iRepurchaseTerm;            // �ع����� 
  char            szTotalValueTraded[21 + 1]; // �ɽ���� 
  int             iSettlDate;                 // �״ν����� 
  char            szCompanyName[16 + 1];      // ���𷽽����̼�� 
  char            szOperCode[6 + 1];          // ���𷽽���Ա���� 
};

//-------------------------------10308075:�Ϻ�Э��ع�δ����Э���ѯ--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseUnsettledBillField
{
  char            szCompanyCode[3 + 1];       // �����̴��� 
  char            szOperCode[6 + 1];          // ����Ա���� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_C]
  char            chOptFlag;                  // �����־ �ֵ�[OPT_FLAG]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szOpptCompanyCode[3 + 1];   // �����̴���(����) 
  char            szOpptCompanyName[10 + 1];  // �����̼��(����) 
  char            szOpptOperCode[6 + 1];      // ����Ա����(����) 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryshContRepurchaseUnsettledBillField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iMatchDate;                 // �ɽ����� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iIntOrg;                    // �ڲ����� 
  char            szCompanyCode[3 + 1];       // �����̴��� 
  char            szCompanyName[10 + 1];      // �����̼�� 
  char            szOperCode[6 + 1];          // ����Ա���� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkPbu[8 + 1];            // ���׵�Ԫ 
  char            szMatchId[16 + 1];          // �ɽ���� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  int             iRepchDate;                 // �ع����� 
  int             iEndDate;                   // ���ڽ������� 
  int             iRepchLimitDates;           // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ؽ�� 
  char            szInterest[21 + 1];         // Ӧ����Ϣ 
  char            szDueRate[21 + 1];          // ������ 
  char            szConversionRate[21 + 1];   // ������ 
  char            szSumFaceAmt[21 + 1];       // ��Ѻȯ���ܶ� 
  int             iRealHoldDates;             // ʵ��ռ������ 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_C]
  char            chOptFlag;                  // �����־ �ֵ�[OPT_FLAG]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szOpptCompanyCode[3 + 1];   // �����̴���(����) 
  char            szOpptCompanyName[10 + 1];  // �����̼��(����) 
  char            szOpptOperCode[6 + 1];      // ����Ա����(����) 
  char            szPawneeName[30 + 1];       // ��Ȩ������ 
};

//-------------------------------10308078:�Ϻ�Э��ع���Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseContractField
{
  char            szCompanyCode[3 + 1];       // �����̴��� 
  char            szOperCode[6 + 1];          // ����Ա���� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_H]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szOpptCompanyCode[3 + 1];   // �����̴���(����) 
  char            szOpptCompanyName[10 + 1];  // �����̼��(����) 
  char            szOpptOperCode[6 + 1];      // ����Ա����(����) 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryshContRepurchaseContractField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iSettDate;                  // �������� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  int             iIntOrg;                    // �ڲ����� 
  char            szCompanyCode[3 + 1];       // �����̴��� 
  char            szCompanyName[10 + 1];      // �����̼�� 
  char            szOperCode[6 + 1];          // ����Ա���� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkPbu[8 + 1];            // ���׵�Ԫ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkProp[2 + 1];           // �ɷ����� �ֵ�[STKPROP]
  char            szBonusAmt[21 + 1];         // ������� 
  int             iRepchDate;                 // �ع����� 
  int             iEndDate;                   // ���ڽ������� 
  int             iRepchLimitDates;           // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ؽ�� 
  char            szDueRate[21 + 1];          // ������ 
  char            szConversionRate[21 + 1];   // ������ 
  char            szSumFaceAmt[21 + 1];       // ��Ѻȯ���ܶ� 
  int             iRealHoldDates;             // ʵ��ռ������ 
  int             iRealRepchDate;             // ʵ�ʹ������� 
  char            szRealRepchAmt[21 + 1];     // ʵ�ʹ��ؽ�� 
  char            szRealDueRate[21 + 1];      // ʵ�ʹ��������� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_H]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szOpptCompanyCode[3 + 1];   // �����̴���(����) 
  char            szOpptCompanyName[10 + 1];  // �����̼��(����) 
  char            szOpptOperCode[6 + 1];      // ����Ա����(����) 
  char            szPawneeName[30 + 1];       // ��Ȩ������ 
  char            chCustCls;                  // �ͻ���� 
  char            szCuacctCls[4 + 1];         // �ʲ��˻���� 
  char            szRemark[1024 + 1];         // ��ע 
};

//-------------------------------10308086:����ծȯЭ��ع����ؽ���ѯ--------------------------
struct MATRADEAPI CReqFsQryszBondContRepurchaseQuotaField
{
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iRepayOpeningDate;          // ��ʼ�������� 
  char            szInitTrdAmt[21 + 1];       // ��ʼ���׽�� 
  char            szRepchRate[21 + 1];        // �������� 
};

struct MATRADEAPI CRspFsQryszBondContRepurchaseQuotaField
{
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iRepayOpeningDate;          // ��ʼ�������� 
  char            szInitTrdAmt[21 + 1];       // ��ʼ���׽�� 
  char            szRepchRate[21 + 1];        // �������� 
  char            szRepchAmt[21 + 1];         // ���ؽ�� 
};

//-------------------------------10308042:ծȯ��Ѻ�ɷ���ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBondPledgeStkDetailField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBondPledgeStkDetailField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llStkPrebln;                // ֤ȯ������� 
  LONGLONG        llStkBln;                   // ֤ȯ��� 
  LONGLONG        llStkAvl;                   // ֤ȯ�������� 
};

//-------------------------------10308043:ծȯ��Ѻʽ�ع���Լ��ѯ--------------------------
struct MATRADEAPI CReqFsQryBondPledgeStkRepurchaseContField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryBondPledgeStkRepurchaseContField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iRepchDate;                 // �������� 
  int             iTrdDate;                   // �������� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  char            szMatchedPrice[21 + 1];     // �ɽ��۸� 
  LONGLONG        llRepchQty;                 // �ع����� 
  char            szRepchAmt[21 + 1];         // ���ڽ�� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llOrderQty;                 // ί������ 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_Z]
};

//-------------------------------10308604:ETF�����Ϲ����볷��--------------------------
struct MATRADEAPI CReqFsETFOfflineApplicationCancelField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  int             iTrdDate;                   // �������� 
  int             iOrderSn;                   // ί����� 
};

struct MATRADEAPI CRspFsETFOfflineApplicationCancelField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ������ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkSubCode[8 + 1];        // �Ϲ����� 
  char            szStkSubName[16 + 1];       // �Ϲ��������� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  int             iOrderSn;                   // ί����� 
};

//-------------------------------10308605:ETF����ί��--------------------------
struct MATRADEAPI CReqFsETFOrderField
{
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szEtfCode[8 + 1];           // ETF���� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szPriceInfo[1024 + 1];      // �۸���Ϣ 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            chBizType;                  // �������� 'B'������, 'P'������, 'S'��������'4'��������'5'������
  char            szClientInfo[256 + 1];      // �ն���Ϣ ����Ӳ�̺�\CPU\��������
  char            chSecurityLevel;            // ��ȫ�ֶ� 0:�ް�ȫ1:У��Ʊ��2:У������
  char            szSecurityInfo[256 + 1];    // ��ȫ��Ϣ ��ȫ�ֶ�ΪУ��Ʊ��������дƱ�ݰ�ȫ�ֶ�Ϊ����������д������������
  char            szTrdacctLink[20 + 1];      // �����ɶ� ���ڿ��г�ETFʱ����
  char            chPublishCtrlFlag;          // �ֱֲ������� 
  int             iOrderBsn;                  // ί������ 
};

struct MATRADEAPI CRspFsETFOrderField
{
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ������ 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
};

//-------------------------------10308057:ETF�����Ϲ������ѯ--------------------------
struct MATRADEAPI CReqFsQryETFOfflineApplicationField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  LONGLONG        llTrdDate;                  // �������� 
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkSubCode[8 + 1];        // �Ϲ����� 
  int             iOrderSn;                   // ί����� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkBiz[1024 + 1];         // ֤ȯҵ�� �ֵ�[STK_BIZ]
  char            chStkSubStatus;             // ����״̬ �ֵ�[STK_SUB_STATUS]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryETFOfflineApplicationField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  char            szOrderTime[32 + 1];        // ί��ʱ�� 
  int             iOrderSn;                   // ί����� 
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szOrderType[32 + 1];        // ί������ �ֵ�[ORDER_TYPE]
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // �����˻� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  int             iStkBizAction;              // ҵ����Ϊ �ֵ�[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iSubsysSn;                  // ��ϵͳ���� 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szOrderUfzAmt[21 + 1];      // ί�нⶳ��� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  char            chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  char            chIsWithdrawn;              // �ѳ�����־ �ֵ�[IS_WITHDRAWN]
  char            szEtfCode[8 + 1];           // ETF���� 
  char            szEtfName[16 + 1];          // ETF���� 
  char            szStkSubCode[8 + 1];        // �Ϲ����� 
  char            szStkSubName[16 + 1];       // �Ϲ��������� 
  char            szBillFee[21 + 1];          // ���� 
  char            szCommissionRate[21 + 1];   // �Ϲ����ñ��� 
  char            chStkSubStatus;             // ����״̬ '0'δ�Ϲ� '1'���Ϲ� '2'�ѳ��� '3'������ '4'�Ѵ���
  char            szTrdacctLink[20 + 1];      // ���������˻� 
  char            szStkbdLink[2 + 1];         // �������װ�� �ֵ�[STKBD]
  char            szStkpbuLink[8 + 1];        // �������׵�Ԫ 
  LONGLONG        llStkAvlFrz;                // ֤ȯ�������� 
  int             iUpdDate;                   // �������� 
  char            szCustName[32 + 1];         // �ͻ����� 
  char            szIdCode[48 + 1];           // ֤������ 
};

//-------------------------------10308064:ETF�ɷֹɳֲֲ�ѯ--------------------------
struct MATRADEAPI CReqFsQryETFCotPositionField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szEtfCode[8 + 1];           // ETF���� 
  char            chEtfMode;                  // ETFģʽ �ֵ�[ETF_MODE]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryETFCotPositionField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szEtfCode[8 + 1];           // ETF���� 
  char            szStkCode[8 + 1];           // �ɷֹɴ��� 
  char            szStkName[8 + 1];           // �ɷֹɼ�� 
  char            chStkMarket;                // �ɷֹɽ����г� 
  LONGLONG        llStkQty;                   // �ɷֹ����� 
  LONGLONG        llStkAvl;                   // �ͻ��ֲ����� 
  char            chEtfInsteadFlag;           // �����־ 0'��ʾ��ֹ�ֽ�����������й�Ʊ��1'��ʾ���Խ����ֽ���������ù�Ʊ����Ʊ����Ļ����ֽ������2'��ʾ�������ֽ����3'��ʾ���й�Ʊ�˲��ֽ����4'��ʾ���й�Ʊ�����ֽ����5'��ʾ�ǻ����˲��ֽ����6'��ʾ�ǻ�������ֽ����
  char            szEtfOverflowRate[21 + 1];  // ��۱��� 
  char            szCreationInsteadAmt[21 + 1];// �깺������ 
  char            szRedemptionInsteadAmt[21 + 1];// ��������� 
};

//-------------------------------10308052:ETF��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryEtfInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szEtfCode[8 + 1];           // ETF���� 
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iQryNum;                    // ��ѯ���� 
};

struct MATRADEAPI CRspFsQryEtfInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szEtfCode[8 + 1];           // ETF���� 
  char            szEtfName[32 + 1];          // ETF���� 
  char            chEtfPublish;               // ETF������־ 0--������1--����
  char            chEtfCrFlag;                // �깺��ر�־ 0-�������깺���1-�����깺���2-�����깺�����������3-������أ��������깺
  LONGLONG        llEtfCrUnit;                // �깺��ص�λ 
  char            szEtfCashRatio[21 + 1];     // �ֽ�������� 
  int             iEtfStkNum;                 // ETF��Ʊ����Ʊ�� 
  char            szEtfEstmCash[21 + 1];      // ETF��Ʊ���ֽ�� 
  char            szEtfCashComp[21 + 1];      // ��׼��λ�ֽ� 
  char            szEtfNavPercu[21 + 1];      // ��׼��λ��ֵ 
  char            szEtfNav[21 + 1];           // ETF��λ��ֵ 
  char            chEtfType;                  // ETF���� �ֵ�[ETF_TYPE]
  char            chEtfMode;                  // ETFģʽ �ֵ�[ETF_MODE]
  char            chQsEtfCrFlag;              // ȯ���깺��ر�־ 0-�������깺���1-�����깺���2-�����깺�����������3-������أ��������깺
  int             iTrdDate;                   // �������� 
  char            szRgCode[8 + 1];            // �����Ϲ����� 
  char            szEjscCode[8 + 1];          // �����г�������� 
};

//-------------------------------10308044:�Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryshLOFFundSplitMergeField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkLofCode[8 + 1];        // LOF���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryshLOFFundSplitMergeField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkLofCode[8 + 1];        // ��֤LOF���� 
  char            szStkLofName[10 + 1];       // ��֤LOF���� 
  char            szStkLofStatusSh[8 + 1];    // ��֤LOF״̬ 
  int             iStkLofRatio;               // ����ת��ϵ�� 
  char            chStkLofType;               // �������� 
  char            chStkLofSubType;            // ���������� 
  LONGLONG        llStkLofPmUnit;             // ��ֺϲ���λ 
  LONGLONG        llStkLofMergeFloor;         // ��С�ϲ��� 
  LONGLONG        llStkLofPartitionFloor;     // ��С����� 
  char            szStkLofMainCode[8 + 1];    // ĸ������� 
  char            szStkLofMainName[10 + 1];   // ĸ�������� 
  int             iStkLofMainRatio;           // ĸ�ݶ�ת��ϵ�� 
  char            szStkLofMainStatus[8 + 1];  // ĸ����״̬ 
};

//-------------------------------10308045:����LOF����ֲ�ϲ���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryszLOFFundSplitMergeField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkLofCode[8 + 1];        // LOF���� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQryszLOFFundSplitMergeField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // ֤ȯ������ �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chStkLofStatus;             // ����״̬ �ֵ�[STK_LOF_STATUS]
  int             iStkMsRatio;                // �ϲ�/�ֲ���� 
  LONGLONG        llStkMergeFloor;            // ��С�ϲ��� 
  LONGLONG        llStkSplitFloor;            // ��С����� 
  char            szStkLofMainCode[8 + 1];    // ĸ������� 
};

//-------------------------------10308046:��Ȩ��Ա��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQrySecurInfoField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  int             iQryNum;                    // ��ѯ���� ÿ��ȡ������
};

struct MATRADEAPI CRspFsQrySecurInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szWrntStkCode[8 + 1];       // ��Ĵ��� 
  char            szWrntExeCode[8 + 1];       // ��Ȩ���� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  char            chCustType;                 // �ͻ����� �ֵ�[CUST_TYPE_F]
  int             iProrateTime;               // ���ڹ���ʱ�� 
  char            chProcTax;                  // �Ƿ�������˰ �ֵ�[PROC_TAX]
  char            szBegIncomeTax[21 + 1];     // ����˰������� 
  char            chStatus;                   // ״̬ �ֵ�[STATUS_S]
};

//-------------------------------10308065:����˰������Ϣά��--------------------------
struct MATRADEAPI CReqFsQryTaxInfoMaintenanceField
{
  int             iSno;                       // ��ˮ�� 
};

struct MATRADEAPI CRspFsQryTaxInfoMaintenanceField
{
  int             iSno;                       // ��ˮ�� 
  char            szBeginAmt[21 + 1];         // ��ʼ��� 
  char            szEndAmt[21 + 1];           // ������� 
  char            szTaxRate[21 + 1];          // ��������˰�� 
  char            szFreeAmt[21 + 1];          // ����۳��� 
};

//-------------------------------10308058:ҪԼ�չ�ת����ϵ���ѯ--------------------------
struct MATRADEAPI CReqFsQryTenderSgConverRelatField
{
  char            szStkbd[32 + 1];            // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ҪԼ���� 
  char            szSgrCode[8 + 1];           // �չ��˱��� 
};

struct MATRADEAPI CRspFsQryTenderSgConverRelatField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ҪԼ���� 
  char            szSgrCode[8 + 1];           // �չ��˱��� 
  char            szSgPrice[21 + 1];          // �չ��۸� 
  char            szPurchaserNane[32 + 1];    // �չ������� 
  int             iTenderBegDate;             // ҪԼ��ʼ���� 
  int             iTenderEndDate;             // ҪԼ�������� 
};

//-------------------------------10308070:�Ϻ����ֹɷݶ�Ȳ�ѯ--------------------------
struct MATRADEAPI CReqFsQryshReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
};

struct MATRADEAPI CRspFsQryshReduceStkQuotaField
{
  int             iTrdDate;                   // �������� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  LONGLONG        llMaxReductQty;             // �������������� 
  LONGLONG        llDzjyMaxReductQty;         // �������������� 
  LONGLONG        llTotalReductQty;           // �����ۼƼ������� 
  LONGLONG        llDzjyTotalReductQty;       // ���ڽ����ۼƼ������� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llStkConfirmQty;            // ֤ȯ�϶�����1 
  LONGLONG        llStkConfirmQty2;           // ֤ȯ�϶�����2 
  LONGLONG        llStkBln;                   // ֤ȯ��� 
  LONGLONG        llDzjyLimitQty;             // ���ڼ������÷��������� 
  LONGLONG        llStkFrz;                   // ֤ȯ�������� 
  LONGLONG        llLimitAvlQty;              // ���ۼ���ʣ���� 
  LONGLONG        llDzjyNoLimitAvlQty;        // ���ڼ��ַ�����ʣ���� 
  LONGLONG        llDzjyLimitAvlQty;          // ���ڼ�������ʣ���� 
};

//-------------------------------10308071:���ڼ��ֹɷݶ�Ȳ�ѯ--------------------------
struct MATRADEAPI CReqFsQryszReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
};

struct MATRADEAPI CRspFsQryszReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  LONGLONG        llStkBln;                   // ֤ȯ��� 
  LONGLONG        llStkFrz;                   // ֤ȯ�������� 
  LONGLONG        llDzjyLimitQty;             // ���÷��������� 
  LONGLONG        llDzjyReductQty;            // ����ͨ�����ڼ��� 
  LONGLONG        llReductQty;                // �ɾ��ۺʹ��ڼ��� 
  LONGLONG        llStkBuyQty;                // �����г����� 
  LONGLONG        llReductLimit;              // ���ۼ��ֶ�� 
  LONGLONG        llDzjyReductLimit;          // ���ڼ��ֶ�� 
  LONGLONG        llInitDzjyLimitQty;         // ��ʼ���÷��������� 
  LONGLONG        llInitDzjyReductQty;        // ��ʼ����ͨ�����ڼ��� 
  LONGLONG        llInitReductQty;            // ��ʼ�ɾ��ۺʹ��ڼ��� 
  LONGLONG        llInitStkBuyQty;            // ��ʼ�����г����� 
  LONGLONG        llInitReductLimit;          // ��ʼ���ۼ��ֶ�� 
  LONGLONG        llInitDzjyReductLimit;      // ��ʼ���ڼ��ֶ�� 
  LONGLONG        llDzjyLimitQtyInclfrz;      // ���÷���������(������) 
  LONGLONG        llDzjyReductQtyInclfrz;     // ����ͨ�����ڼ���(������) 
  LONGLONG        llReductQtyInclfrz;         // �ɾ��ۺʹ��ڼ���(������) 
  LONGLONG        llStkBuyQtyInclfrz;         // �����г�����(������) 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llLimitAvlQty;              // ���ۼ���ʣ���� 
  LONGLONG        llDzjyNoLimitAvlQty;        // ���ڼ��ַ�����ʣ���� 
  LONGLONG        llDzjyLimitAvlQty;          // ���ڼ�������ʣ���� 
};

//-------------------------------10308084:Ȩ֤��Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryWarrantInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szWrntCode[8 + 1];          // Ȩ֤���� 
  char            szWrntExeCode[8 + 1];       // ��Ȩ���� 
};

struct MATRADEAPI CRspFsQryWarrantInfoField
{
  char            chStkex;                    // �����г� 
  char            szStkbd[2 + 1];             // ���װ�� 
  char            szWrntCode[8 + 1];          // Ȩ֤���� 
  char            chWrntCls;                  // Ȩ֤��� �ֵ�[WRNT_CLS]
  char            szWrntStkCode[8 + 1];       // ��Ĵ��� 
  char            szWrntExeCode[8 + 1];       // ��Ȩ���� 
  char            chWrntExeMode;              // ��Ȩģʽ �ֵ�[WRNT_EXE_MODE]
  char            szWrntExePrice[21 + 1];     // ��Ȩ�۸� 
  char            szWrntExeRatio[21 + 1];     // ��Ȩ���� 
  int             iWrntExeBgnDate;            // ��Ȩ��ʼ���� 
  int             iWrntExeEndDate;            // ��Ȩ�������� 
  char            chWrntSettMode;             // ���㷽ʽ �ֵ�[WRNT_SETT_MODE]
  char            szWrntSettPrice[21 + 1];    // ����۸� 
  char            szWrntTrdbleVol[21 + 1];    // ��ͨ�ܶ� 
  int             iWrntExpireDate;            // �������� 
  char            szWrntCreateCode[8 + 1];    // ������� 
};

//-------------------------------10308079:�����ع�������Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryThreeRePurchaseInfoField
{
  char            szBasketId[8 + 1];          // ���ӱ�� 
  char            szBondCode[8 + 1];          // ծȯ���� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ����ֵΪ��¼�����ѯ�������ʹ��
  int             iQryNum;                    // ��ѯ���� ���ֵΪ1000
};

struct MATRADEAPI CRspFsQryThreeRePurchaseInfoField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szBasketId[8 + 1];          // ���ӱ�� 
  char            szBasketName[20 + 1];       // �������� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szBondCode[8 + 1];          // ծȯ���� 
  char            szBasketRate[21 + 1];       // �ۿ��� 
  int             iUpdDate;                   // �������� 
};

//-------------------------------10308080:�����ع���Ѻ���ѯ--------------------------
struct MATRADEAPI CReqFsQryThreeRePurchasepledgeField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szQryPos[32 + 1];           // ��λ�� ����ֵΪ��¼�����ѯ�������ʹ��
  int             iQryNum;                    // ��ѯ���� ���ֵΪ1000
};

struct MATRADEAPI CRspFsQryThreeRePurchasepledgeField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  LONGLONG        llStkTotal;                 // ��������� 
  LONGLONG        llStkUse;                   // �������Ѻ���� 
  LONGLONG        llStkAvl;                   // �������Ѻ���� 
  LONGLONG        llStkTrdUse;                // �������� 
  LONGLONG        llStkRealIn;                // ����������� 
  LONGLONG        llStkRealOut;               // ���ճ������� 
  char            szRemark[128 + 1];          // ��ע��Ϣ 
};

//-------------------------------10308081:���������ع�ת���ɽ���Ϣ���ѯ--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseMatchInfoField
{
  int             iTrdDate;                   // �������� 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szSfcontractNo[32 + 1];     // ��Լ��� 
  char            szOpptStkpbu[8 + 1];        // ���ַ�ϯλ 
  char            chStatus;                   // ״̬ �ֵ�[STATUS_F]
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseMatchInfoField
{
  int             iTrdDate;                   // �������� 
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ�����֣� 
  int             iSfTrdType;                 // �걨ҵ����� �ֵ�[SF_TRD_TYPE]
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szSfcontractNo[32 + 1];     // ��Լ��� 
  char            szDueRate[21 + 1];          // ������ 
  char            szRepchAmt[21 + 1];         // ���ؽ�� 
  int             iRepchLimitDates;           // �ع����� 
  int             iBaskets;                   // �������� 
  char            szBaskrtid[32 + 1];         // ���ӱ�� 
  int             iCodes;                     // ծȯ���� 
  char            szBondCodes[256 + 1];       // ����֤ȯ 
  char            szCodeQty[256 + 1];         // ����֤ȯ���� 
  char            szBondCode1[8 + 1];         // ��Ѻծȯ1 
  char            szBondCodeQty1[20 + 1];     // ��Ѻծȯ1���� 
  char            szBondCode2[8 + 1];         // ��Ѻծȯ2 
  char            szBondCodeQty2[20 + 1];     // ��Ѻծȯ2���� 
  char            szBondCode3[8 + 1];         // ��Ѻծȯ3 
  char            szBondCodeQty3[20 + 1];     // ��Ѻծȯ3���� 
  char            szBondCode4[8 + 1];         // ��Ѻծȯ4 
  char            szBondCodeQty4[20 + 1];     // ��Ѻծȯ4���� 
  char            szBondCode5[8 + 1];         // ��Ѻծȯ5 
  char            szBondCodeQty5[20 + 1];     // ��Ѻծȯ5���� 
  char            szHcBondCode[8 + 1];        // ��Ѻծȯ 
  char            szHcCodeQty[20 + 1];        // ��Ѻծȯ���� 
  char            chStatus;                   // ״̬ �ֵ�[STATUS_F]
  char            szRemark[1024 + 1];         // ��ע 
};

//-------------------------------10308082:���������ع���Լ�ܱ��ѯ--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseContTotalField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_C]
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseContTotalField
{
  int             iTrdDate;                   // �������� 
  int             iMatchDate;                 // �ɽ����� 
  char            szSfcontractNo[32 + 1];     // ��Լ��� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkPbu[8 + 1];            // ���׵�Ԫ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  int             iRepchDate;                 // �ع����� 
  int             iEndDate;                   // ���ڽ������� 
  int             iRepchLimitDates;           // �ع����� 
  char            szMatchAmt[21 + 1];         // ��ʼ���׽�� 
  char            szRepchAmt[21 + 1];         // �ع���� 
  char            szDueRate[21 + 1];          // �ع����� 
  char            szDueIntr[21 + 1];          // �ع���Ϣ 
  int             iRealRepchDate;             // ʵ�ʹ������� 
  char            szRealRepchAmt[21 + 1];     // ʵ�ʹ��ؽ�� 
  int             iRealHoldDates;             // ʵ��ռ������ 
  char            szRealDueRate[21 + 1];      // ʵ�ʹ������� 
  char            szRealDueIntr[21 + 1];      // ʵ�ʹ�����Ϣ 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_F]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szRemark[1024 + 1];         // ��ע 
};

//-------------------------------10308083:���������ع���Լ��ϸ��ѯ--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseContDetailField
{
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szSfcontractNo[32 + 1];     // ��Լ��� 
  char            szQryPos[32 + 1];           // ��λ�� ����ֵΪ��¼�����ѯ�������ʹ��
  int             iQryNum;                    // ��ѯ���� ���ֵΪ1000
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseContDetailField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iTrdDate;                   // �������� 
  int             iMatchDate;                 // �ɽ����� 
  char            szSfcontractNo[32 + 1];     // ��Լ��� 
  int             iItemno;                    // ��Լ��� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkPbu[8 + 1];            // ���׵�Ԫ 
  char            szOrderId[10 + 1];          // ��ͬ��� 
  char            szMatchId[16 + 1];          // �ɽ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szBasketId[8 + 1];          // ���ӱ�� 
  char            szBasketRate[21 + 1];       // �����ۿ��� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkProp[2 + 1];           // �ɷ����� 
  char            szBonusAmt[21 + 1];         // ������� 
  LONGLONG        llMatchQty;                 // ��Ѻ���� 
  LONGLONG        llBackqty;                  // ��Ѻ���� 
  LONGLONG        llBackqtyReal;              // �����Ѻ���� 
  char            chContractType;             // ��Լ���� �ֵ�[CONTRACT_TYPE]
  char            chRepchStatus;              // �ع�״̬ �ֵ�[REPCH_STATUS_F]
  char            szOpptStkpbu[8 + 1];        // ���׵�Ԫ(����) 
  char            szOpptTrdacct[20 + 1];      // �����˻�(����) 
  char            szRemark[1024 + 1];         // ��ע 
};

//-------------------------------10308104:H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHStkFullCirAcctInfoField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
};

struct MATRADEAPI CRspFsQryHStkFullCirAcctInfoField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szTrdacct[20 + 1];          // �����˻� 
  char            szStkCode[8 + 1];           // ֤ȯ���� 
  char            szSellApprovalSn[50 + 1];   // ����������ĺ� 
  char            szBuyApprovalSn[50 + 1];    // ����������ĺ� 
  LONGLONG        llAssetJcInit;              // ���ֹɷݳ�ʼ��� 
  LONGLONG        llAssetZcInit;              // ���ֹɷݳ�ʼ��� 
  LONGLONG        llAssetJcBln;               // ���ֹɷݶ����� 
  LONGLONG        llAssetZcBln;               // ���ֹɷݶ����� 
  LONGLONG        llAssetJcAvl;               // ���ֹɷݶ�ȿ��� 
  LONGLONG        llAssetZcAvl;               // ���ֹɷݶ�ȿ��� 
  int             iEffectDateJc;              // ���ֶ����Чʱ�� 
  int             iEffectDateZc;              // ���ֶ����Чʱ�� 
};

//-------------------------------10308105:H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ--------------------------
struct MATRADEAPI CReqFsQryHStkForTrdIdCorrespondField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkCodeI[8 + 1];          // ����֤ȯ���� 
  char            szStkCodeO[8 + 1];          // ����֤ȯ���� 
};

struct MATRADEAPI CRspFsQryHStkForTrdIdCorrespondField
{
  char            chStkex;                    // �����г� �ֵ�[STKEX]
  char            szStkCodeI[8 + 1];          // ����֤ȯ���� 
  char            szStkCodeO[8 + 1];          // ����֤ȯ���� 
};

//-------------------------------10308106:ѯ�۽��׷ǹ������������ѯ--------------------------
struct MATRADEAPI CReqFsQryInquireTransferCloseMarketInfoField
{
  char            szBjType[4 + 1];            // �������� �ֵ�[INQUIRY_QUOTE_TYPE]
  char            chQuoteStatus;              // ״̬ �ֵ�[INQUIRY_QUOTE_STATUS]
  char            chSide;                     // �������� �ֵ�[INQUIRY_SIDE]
};

struct MATRADEAPI CRspFsQryInquireTransferCloseMarketInfoField
{
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szBjType[4 + 1];            // �������� �ֵ�[INQUIRY_QUOTE_TYPE]
  char            szJysOrderId[16 + 1];       // ������ί�б�� 
  char            szConferNum[16 + 1];        // ҪԼ�� 
  char            chQuoteStatus;              // ״̬ �ֵ�[INQUIRY_QUOTE_STATUS]
  char            szCompanyName[16 + 1];      // ���۷������̼�� 
  char            chSide;                     // �������� �ֵ�[INQUIRY_SIDE]
  char            szPrice[21 + 1];            // �۸� 
  int             iInquiryTime;               // ѯ��ʱ�� 
  char            szStkCode[8 + 1];           // ծȯ���� 
  LONGLONG        llStkQty;                   // ծȯ���� 
};

//-------------------------------10418001:��ʷ�ɽ���ѯ--------------------------
struct MATRADEAPI CReqFsQryHisFillField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ͻ����뼰֤ȯ�˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʽ��˺� �ͻ����뼰֤ȯ�˻�����ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // �������� 
};

struct MATRADEAPI CRspFsQryHisFillField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  char            szMatchedTime[8 + 1];       // �ɽ�ʱ�� 
  LONGLONG        llOrderSn;                  // ί����� 
  int             iOrderBsn;                  // ί������ 
  char            szOrderId[32 + 1];          // ��ͬ��� 
  int             iIntOrg;                    // �ڲ����� 
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chDlvyCurrency;             // ���ձ��� �ֵ�[CURRENCY]
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szMatchSn[20 + 1];          // �ɽ���� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  LONGLONG        llMatchQty;                 // �ѳɽ����� 
  char            szMatchAmt[21 + 1];         // �ѳɽ���� 
  LONGLONG        llMatchCnt;                 // �ɽ����� 
};

//-------------------------------10418002:��ʷί�в�ѯ--------------------------
struct MATRADEAPI CReqFsQryHisOrderField
{
  LONGLONG        llCuacctCode;               // �ʽ��˺� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  int             iIntOrg;                    // �ڲ����� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
  int             iBgnDate;                   // ��ʼ���� ��������TRD_DATE
  int             iEndDate;                   // �������� ��������TRD_DATE
};

struct MATRADEAPI CRspFsQryHisOrderField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iIntOrg;                    // �ڲ����� 
  int             iTrdDate;                   // �������� 
  int             iOrderDate;                 // ί������ 
  char            szOrderId[32 + 1];          // ��ͬ��� 
  char            chOrderStatus;              // ί��״̬ �ֵ�[ORDER_STATUS]
  LONGLONG        llCustCode;                 // �ͻ����� 
  LONGLONG        llCuacctCode;               // �ʲ��˻� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szStkpbu[8 + 1];            // ���׵�Ԫ 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  int             iStkBiz;                    // ֤ȯҵ�� �ֵ�[STK_BIZ]
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            chCurrency;                 // ���Ҵ��� �ֵ�[CURRENCY]
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderAmt[21 + 1];         // ί�н�� 
  char            szOrderFrzAmt[21 + 1];      // ί�ж����� 
  char            szOrderUfzAmt[21 + 1];      // ί�нⶳ��� 
  LONGLONG        llWithdrawnQty;             // �ѳ������� 
  LONGLONG        llMatchedQty;               // �ѳɽ����� 
  char            chIsWithdraw;               // ������־ �ֵ�[IS_WITHDRAW]
  char            chIsWithdrawn;              // �ѳ�����־ �ֵ�[IS_WITHDRAWN]
  int             iOrderBsn;                  // ί������ 
  char            szOrderTime[32 + 1];        // ί��ʱ�� 
  char            szMatchedAmt[21 + 1];       // �ѳɽ���� 
  char            szSettAmt[21 + 1];          // ������ 
};

//-------------------------------10418003:��ʷ�����ѯ--------------------------
struct MATRADEAPI CReqFsQryHisDeliOrderField
{
  LONGLONG        llCustCode;                 // �ͻ����� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  LONGLONG        llCuacctCode;               // �ʽ��˺� �ʽ��˻���֤ȯ�˻�����ͬʱΪ��
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            szTrdacct[20 + 1];          // ֤ȯ�˻� 
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            chStkCls;                   // ֤ȯ��� �ֵ�[STK_CLS]
  char            szStkbd[2 + 1];             // ���װ�� �ֵ�[STKBD]
  char            szQryPos[32 + 1];           // ��λ�� ��һ�����
  LONGLONG        llQryNum;                   // ��ѯ���� ÿ��ȡ������
  int             iBgnDate;                   // ��ʼ���� 
  int             iEndDate;                   // �������� 
};

struct MATRADEAPI CRspFsQryHisDeliOrderField
{
  char            szQryPos[32 + 1];           // ��λ�� 
  int             iSettDate;                  // �������� 
  int             iShareDlvyDate;             // �ɷݽ������� 
  char            szFundDlvyDate[21 + 1];     // �ʽ������� 
  char            szOrderTime[32 + 1];        // ί��ʱ�� 
  char            szStkBiz[10 + 1];           // ֤ȯҵ�� 
  LONGLONG        llCuacctCode;               // �ʽ��ʻ� 
  char            szFortuneCode[20 + 1];      // �Ƹ��˻� 
  char            chDlvyCurrency;             // ���ձ��� �ֵ�[DLVY_CURRENCY]
  char            chStkex;                    // �г� �ֵ�[STKEX]
  LONGLONG        llCustCode;                 // �ͻ����� 
  char            szCustName[32 + 1];         // �ͻ����� 
  LONGLONG        llOrderId;                  // ��ͬ��� 
  char            szStkCode[10 + 1];          // ֤ȯ���� 
  char            szStkName[16 + 1];          // ֤ȯ���� 
  char            szMatchTime[8 + 1];         // �ɽ�ʱ�� 
  char            szMatchId[20 + 1];          // �ɽ���� 
  LONGLONG        llMatchCnt;                 // �ɽ����� 
  LONGLONG        llMatchQty;                 // �ɽ����� 
  char            szMatchPrice[21 + 1];       // �ɽ��۸� 
  char            szMatchAmt[21 + 1];         // �ɽ���� 
  char            szSettAmt[21 + 1];          // ������ 
  char            szStampDuty[21 + 1];        // ӡ��˰ 
  char            szCommision[21 + 1];        // ������ 
  char            szTransferFee[21 + 1];      // ������ 
  char            szClearingFee[21 + 1];      // ����� 
  char            szTrdRegFee[21 + 1];        // ���׹�� 
  char            szHandleFee[21 + 1];        // ���ַ� 
  char            szSecuReguFee[21 + 1];      // ֤�ܷ� 
  char            szFundBln[21 + 1];          // �ʽ���� 
  LONGLONG        llStkBln;                   // �ɷ���� 
  LONGLONG        llOrderQty;                 // ί������ 
  char            szOrderPrice[21 + 1];       // ί�м۸� 
  char            szVentureFee[21 + 1];       // ���ս� 
};

//-------------------------------00102023:�ɽ��ر�����------------------------------------
struct MATRADEAPI CRtnFsOrderFillField
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
struct MATRADEAPI CRtnFsOrderConfirmField
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

#endif  //__MA_CLI_FS_TRADE_API_STRUCT_H__