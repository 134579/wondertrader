//----------------------------------------------------------------------------
// ��Ȩ������������ģ�����ڽ�֤΢�ں˼ܹ�ƽ̨(KMAP)��һ����
//           ��֤�Ƽ��ɷ����޹�˾  ��Ȩ����
//
// �ļ����ƣ�maSvrMktMgr.h
// ģ�����ƣ�΢�ܹ�C++�����ⲿ��������Դ�Ĺ���ӿ�
// ģ��������
// �������ߣ������
// �������ڣ�2019-06-20
// ģ��汾��001.000.000
//----------------------------------------------------------------------------
// �޸�����      �汾          ����            ��ע
//----------------------------------------------------------------------------
// 2019-06-20    001.000.000   �����          ԭ��
//----------------------------------------------------------------------------
#if !defined(__MA_SVRMKT_MGR_H__)
#define __MA_SVRMKT_MGR_H__
#include "maSvrMktApi.h"

// ����dll/so��������ԭ��
//int MAMKTDATAAPI CreateSvrMktMgr(void **p_ppclMgr, void *p_pclSpi);
//int MAMKTDATAAPI ReleaseSvrMktMgr(void *p_pclMgr);

BGN_NAMESPACE_MACLI

class ISvrMktMgrSpi
{
public:
  // �Ͽ���Ӧ(������������ӳɹ�ʱ����)
  virtual int OnConnect(void) = 0;

  // �Ͽ���Ӧ(��������������״ζϿ�ʱ����)
  virtual int OnDisconnect(int p_iErrorCode, const char *p_pszErrorMsg) = 0;

  // ������Ӧ(��������������ؽ��ɹ�ʱ����)
  virtual int OnReconnect(void) = 0;

  // Ӧ����Ϣ��Ӧ
  virtual int OnResponse(unsigned char *p_pszData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;

  // ������Ϣ��Ӧ
  virtual int OnPublish(unsigned char *p_pszPubData, int p_iDataLen, LONGLONG p_llAcceptSn, const char *p_pszTopic) = 0;

  // ��ʾ��Ϣ��Ӧ
  virtual int OnMessage(int p_iDate, int p_iTime, unsigned int p_iPid, unsigned int p_iTid, int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0;
};

class ISvrMktMgrApi
{
public:
  // ����Formatʵ��
  virtual ISvrMktApiFmt* CreateFormat(void) = 0;

  // ����Formatʵ��
  virtual void ReleaseFormat(ISvrMktApiFmt *p_pclFmt) = 0;

public:
  // ��ʼ���뷴��ʼ��
  virtual int Initialize(void) = 0;
  virtual int Uninitialize(void) = 0;

  // ����ģ��
  virtual const char* GetModule(void) = 0;
  virtual int SetModule(const char* p_pszModule) = 0;
  virtual bool CheckModlue(void) = 0;

  // ��������
  virtual unsigned int GetQueMaxSize(void) = 0;
  virtual int SetQueMaxSize(unsigned int p_uiQueMaxSize) = 0;
  virtual unsigned int GetReqQueDepth(void) = 0;
  virtual unsigned int GetRepQueDepth(void) = 0;
  virtual unsigned int GetPubQueDepth(void) = 0;

  // ���м�״̬
  virtual bool IsRunning(void) = 0;
  virtual unsigned int GetPid(void) = 0;
  virtual int Startup(bool p_bDebug = false) = 0;
  virtual int Shutdown(void) = 0;

  // ����֪ͨ
  virtual int Request(unsigned char *p_pszReqData, int p_iDataLen, LONGLONG p_llRequestId, const char *p_pszFuncId) = 0;

  // ������Ϣ
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_SVRMKT_MGR_H__
