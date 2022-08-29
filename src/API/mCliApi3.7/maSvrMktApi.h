//----------------------------------------------------------------------------
// ��Ȩ������������ģ�����ڽ�֤΢�ں˼ܹ�ƽ̨(KMAP)��һ����
//           ��֤�Ƽ��ɷ����޹�˾  ��Ȩ����
//
// �ļ����ƣ�maSvrMktApi.h
// ģ�����ƣ�΢�ܹ�C++�����ⲿ��������Դ�Ľӿ�
// ģ��������
// �������ߣ������
// �������ڣ�2019-06-20
// ģ��汾��001.000.000
//----------------------------------------------------------------------------
// �޸�����      �汾          ����            ��ע
//----------------------------------------------------------------------------
// 2019-06-20    001.000.000   �����          ԭ��
//----------------------------------------------------------------------------
#if !defined(__MA_SVRMKTAPI_API_H__)
#define __MA_SVRMKTAPI_API_H__
#include "maMktData.h"

#if defined(LONGLONG)
#undef LONGLONG;
#endif

#if defined(ULONGLONG)
#undef ULONGLONG;
#endif

#if defined (_MSC_VER) && (_MSC_VER == 1200)
  #define FORMAT_LONGLONG "%I64d"
  #if defined(WIN32)
  typedef __int64 LONGLONG;
  typedef unsigned __int64 ULONGLONG;
  #endif
#else
  #define FORMAT_LONGLONG "%lld"
  typedef long long LONGLONG;
  typedef unsigned long long ULONGLONG;
#endif


#if defined(WIN32) || defined(WIN64) || defined(OS_IS_WINDOWS)
  #if defined MA_MKTDATA_API_EXPORTS
    #define MAMKTDATAAPI __declspec(dllexport)
  #else
    #define MAMKTDATAAPI __declspec(dllimport)
  #endif
#elif defined(OS_IS_LINUX)
  #define MAMKTDATAAPI
#endif


#define BGN_NAMESPACE_MACLI   namespace macli {
#define END_NAMESPACE_MACLI   }
#define USE_NAMESPACE_MACLI   using namespace macli;

// �������Դdll/so��������ԭ��
//int MACLIAPI CreateSvrMktSpi(void **p_ppclSpi, void *p_pclApi);
//int MACLIAPI ReleaseSvrMktSpi(void *p_pclSpi);

BGN_NAMESPACE_MACLI

// ������Ϣ���Ͷ���
enum EN_SVRMKT_MSG_TYPE
{
  SVRMKT_MSG_TYPE_REQ = 1,  // ������Ϣ
  SVRMKT_MSG_TYPE_REP = 2,  // ��Ӧ��Ϣ
  SVRMKT_MSG_TYPE_PUB = 3,  // ������Ϣ
  SVRMKT_MSG_TYPE_RTM = 4,  // ������Ϣ
  SVRMKT_MSG_TYPE_CON = 5,  // ������Ϣ
  SVRMKT_MSG_TYPE_OTH = 9,  // ������Ϣ
};

// ��������
enum EN_SVRMKT_TYPE
{
  SVRMKT_TYPE_DATA = '1',
  SVRMKT_TYPE_DATA2 = '2',
  SVRMKT_TYPE_DATA3 = '3',
  SVRMKT_TYPE_MATCH = '4',
  SVRMKT_TYPE_ORDER = '5',
  SVRMKT_TYPE_INDEX = '6',
  SVRMKT_TYPE_ORDER_QUEUE = '7',
  SVRMKT_TYPE_TRANSACTION = '8',
};

// �����ӿ���
class MAMKTDATAAPI ISvrMktApiFmt
{
public:
  // ���
  virtual int BeginWrite(void) = 0;
  virtual int SetValue(const char *p_pszValue, int p_iValueLen, const char *p_pszField) = 0;
  virtual int EndWrite(void) = 0;
  virtual int Make(unsigned char **ppszData, int &p_refiDataLen) = 0;
  virtual int CreateTable(void) = 0;
  virtual int AddRow(void) = 0;
  virtual int SaveRow(void) = 0;

  // ���
  virtual int Parse(unsigned char *p_pszData, int p_iDataLen) = 0;
  virtual int GetTableCount(int &p_refiTableCount) = 0;
  virtual int OpenTable(int p_iTableIdx) = 0;
  virtual int GetRowCount(int &p_refiRowCount) = 0;
  virtual int ReadRow(int p_iRowIdx) = 0;
  virtual int GetValue(char *p_pszValue, int p_iValueSize, int &p_refiValueLen, const char *p_pszField) = 0;

  // ����
  virtual int GetValue(char *p_pszValue, int p_iValueSize, const char *p_pszField) = 0;
  virtual int SetValue(const char *p_pszValue, const char *p_pszField) = 0;

  virtual int GetValue(char &p_refchValue,  const char *p_pszField) = 0;
  virtual int SetValue(char p_chValue, const char *p_pszField) = 0;

  virtual int GetValue(short &p_refsValue,  const char *p_pszField) = 0;
  virtual int SetValue(short p_sValue, const char *p_pszField) = 0;

  virtual int GetValue(unsigned short &p_refusValue, const char *p_pszField) = 0;
  virtual int SetValue(unsigned short p_usValue, const char *p_pszField) = 0;

  virtual int GetValue(int &p_refiValue, const char *p_pszField) = 0;
  virtual int SetValue(int p_iValue, const char *p_pszField) = 0;

  virtual int GetValue(unsigned int &p_refuiValue, const char *p_pszField) = 0;
  virtual int SetValue(unsigned int p_uiValue, const char *p_pszField) = 0;

  virtual int GetValue(LONGLONG &p_refllValue, const char *p_pszField) = 0;
  virtual int SetValue(LONGLONG p_llValue, const char *p_pszField) = 0;

  virtual int GetValue(ULONGLONG &p_refullValue, const char *p_pszField) = 0;
  virtual int SetValue(ULONGLONG p_ullValue, const char *p_pszField) = 0;

  virtual int GetValue(double &p_refdValue, const char *p_pszField) = 0;
  virtual int SetValue(double p_dValue, const char *p_pszField) = 0;

  // ������Ϣ
  virtual const char* GetLastErrorText(void) = 0;
};

// �������Դ��Ӧ�ӿ���
class MAMKTDATAAPI ISvrMktSpi
{
public:
  // ������Ӧ
  virtual int OnSetParam(const char* p_pszParam) {return 0;}

  // ������Ӧ
  virtual int OnStartup(void) {return 0;}

  // �˳���Ӧ
  virtual int OnShutdown(void) {return 0;}

  // ������Ӧ
  virtual int OnRequest(unsigned char *p_pszReqData, int p_iDataLen, LONGLONG  p_llRequestId, const char *p_pszFuncId) {return 0;}
};

// �������ӿ���
class MAMKTDATAAPI ISvrMktApi
{
public:
  // ����Formatʵ��
  virtual ISvrMktApiFmt* CreateFormat(void) = 0;

  // ����Formatʵ��
  virtual void ReleaseFormat(ISvrMktApiFmt *p_pclFmt) = 0;

  // ������Ӧ��Ϣ(p_llRequestIDΨһ����)
  virtual int Response(unsigned char *p_pszRspData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;

  // ����������Ϣ
  virtual int Publish(ST_MKT_DATA2 &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_DATA3 &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_MATCH &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_TRANSACTION &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_ORDER &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_ORDER_QUEUE &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_INDEX &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_THOUNSAND_DATA &refstData, LONGLONG p_llAcceptSn) = 0;

  // ����������Ϣ
  virtual int Message(int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0; 

  // ��ȡ��������Ϣ
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_SVRMKTAPI_API_H__
