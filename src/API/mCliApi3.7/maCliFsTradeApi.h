#if !defined(__MA_CLI_FS_TRADE_API_H__)
#define __MA_CLI_FS_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliFsTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFsTradeSpi : virtual public CCliTradeSpi
{
public:
  // ȷ�ϻر�
  virtual int OnRtnOrderConfirm(CRtnFsOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // �ɽ��ر�
  virtual int OnRtnOrderFill(CRtnFsOrderFillField *p_pRtnOrderFillField) {return 0;}
  // ������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308011(CFirstSetField *p_pFirstSetField, CRspFsQry10308011Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308013(CFirstSetField *p_pFirstSetField, CRspFsQry10308013Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ֵ���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308012(CFirstSetField *p_pFirstSetField, CRspFsQry10308012Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ϵͳ״̬��ѯ��Ӧ
  virtual int OnRspQry10308014(CFirstSetField *p_pFirstSetField, CRspFsQry10308014Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��֤ת����Ӧ
  virtual int OnRsp10510605(CFirstSetField *p_pFirstSetField, CRspFs10510605Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ȯ�̷�������������Ӧ
  virtual int OnRspQry10510606(CFirstSetField *p_pFirstSetField, CRspFsQry10510606Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ѯ��֤ҵ����ˮ��Ӧ
  virtual int OnRspQry10510608(CFirstSetField *p_pFirstSetField, CRspFsQry10510608Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ����ֽ�ת��Ӧ
  virtual int OnRsp10510620(CFirstSetField *p_pFirstSetField, CRspFs10510620Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �˻��ʽ�鼯��Ӧ
  virtual int OnRspQry10510621(CFirstSetField *p_pFirstSetField, CRspFsQry10510621Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ѯת�������ʻ���Ӧ
  virtual int OnRspQry10510601(CFirstSetField *p_pFirstSetField, CRspFsQry10510601Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻��ʽ𻮲���Ӧ
  virtual int OnRspQry10510468(CFirstSetField *p_pFirstSetField, CRspFsQry10510468Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻�������ˮ��ѯ��Ӧ
  virtual int OnRspQry10510473(CFirstSetField *p_pFirstSetField, CRspFsQry10510473Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ���¼��Ӧ
  virtual int OnRspQry10301105(CFirstSetField *p_pFirstSetField, CRspFsQry10301105Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʲ��˻���ѯ��Ӧ
  virtual int OnRspQry10308112(CFirstSetField *p_pFirstSetField, CRspFsQry10308112Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ƹ��˻���ѯ��Ӧ
  virtual int OnRspQry10308113(CFirstSetField *p_pFirstSetField, CRspFsQry10308113Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɶ���ѯ��Ӧ
  virtual int OnRspQry10308001(CFirstSetField *p_pFirstSetField, CRspFsQry10308001Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɶ�����Э���ѯ��Ӧ
  virtual int OnRspQry10308010(CFirstSetField *p_pFirstSetField, CRspFsQry10308010Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʵ���ƥ������Ӧ
  virtual int OnRsp10308611(CFirstSetField *p_pFirstSetField, CRspFs10308611Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ͻ��ʵ��Բ�ѯ��Ӧ
  virtual int OnRspQry10308068(CFirstSetField *p_pFirstSetField, CRspFsQry10308068Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʒ�ʵ��Բ�ѯ��Ӧ
  virtual int OnRspQry10308069(CFirstSetField *p_pFirstSetField, CRspFsQry10308069Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ʽ��ѯ��Ӧ
  virtual int OnRspQry10308048(CFirstSetField *p_pFirstSetField, CRspFsQry10308048Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ʲ���ѯ��Ӧ
  virtual int OnRspQry10308002(CFirstSetField *p_pFirstSetField, CRspFsQry10308002Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɷ��ʲ���ѯ��Ӧ
  virtual int OnRspQry10308003(CFirstSetField *p_pFirstSetField, CRspFsQry10308003Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ί�в�ѯ��Ӧ
  virtual int OnRspQry10308004(CFirstSetField *p_pFirstSetField, CRspFsQry10308004Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ճɽ���ѯ��Ӧ
  virtual int OnRspQry10308006(CFirstSetField *p_pFirstSetField, CRspFsQry10308006Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ճɽ�ͳ����Ӧ
  virtual int OnRspQry10308053(CFirstSetField *p_pFirstSetField, CRspFsQry10308053Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ɽ�������������Ӧ
  virtual int OnRspQry10308056(CFirstSetField *p_pFirstSetField, CRspFsQry10308056Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��������ί���걨��Ӧ
  virtual int OnRsp10308601(CFirstSetField *p_pFirstSetField, CRspFs10308601Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ����ί�г�����Ӧ
  virtual int OnRsp10308607(CFirstSetField *p_pFirstSetField, CRspFs10308607Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �ɳ���ί�в�ѯ��Ӧ
  virtual int OnRspQry10308054(CFirstSetField *p_pFirstSetField, CRspFsQry10308054Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ֤ȯ��������������Ӧ
  virtual int OnRsp10308606(CFirstSetField *p_pFirstSetField, CRspFs10308606Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���óɱ���Ӧ
  virtual int OnRsp10308602(CFirstSetField *p_pFirstSetField, CRspFs10308602Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���гֲ���ϸ��ѯ��Ӧ
  virtual int OnRspQry10308015(CFirstSetField *p_pFirstSetField, CRspFsQry10308015Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ��Ӧ
  virtual int OnRspQry10308016(CFirstSetField *p_pFirstSetField, CRspFsQry10308016Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ծԤ���в�Ʒ��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308017(CFirstSetField *p_pFirstSetField, CRspFsQry10308017Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���Ų�ѯ��Ӧ
  virtual int OnRspQry10418004(CFirstSetField *p_pFirstSetField, CRspFsQry10418004Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���ǩ��ѯ��Ӧ
  virtual int OnRspQry10418005(CFirstSetField *p_pFirstSetField, CRspFsQry10418005Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹��깺��ֵ��Ȳ�ѯ��Ӧ
  virtual int OnRspQry10418006(CFirstSetField *p_pFirstSetField, CRspFsQry10418006Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �¹���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10418007(CFirstSetField *p_pFirstSetField, CRspFsQry10418007Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�Ʒ����Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308019(CFirstSetField *p_pFirstSetField, CRspFsQry10308019Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ����������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308020(CFirstSetField *p_pFirstSetField, CRspFsQry10308020Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ���ѯ��Ӧ
  virtual int OnRspQry10308021(CFirstSetField *p_pFirstSetField, CRspFsQry10308021Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Լչ����Ӧ
  virtual int OnRsp10308608(CFirstSetField *p_pFirstSetField, CRspFs10308608Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���Ѻ�ʽ��ѯ��Ӧ
  virtual int OnRspQry10308022(CFirstSetField *p_pFirstSetField, CRspFsQry10308022Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�ԤԼ��ˮ��ѯ��Ӧ
  virtual int OnRspQry10308023(CFirstSetField *p_pFirstSetField, CRspFsQry10308023Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�ԤԼ��Ӧ
  virtual int OnRsp10308610(CFirstSetField *p_pFirstSetField, CRspFs10308610Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�����ǰ���غ�Լ��ѯ��Ӧ
  virtual int OnRspQry10308024(CFirstSetField *p_pFirstSetField, CRspFsQry10308024Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���չ��ȡ����Լ��ѯ��Ӧ
  virtual int OnRspQry10308025(CFirstSetField *p_pFirstSetField, CRspFsQry10308025Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ��Ӧ
  virtual int OnRspQry10308026(CFirstSetField *p_pFirstSetField, CRspFsQry10308026Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQry10308027(CFirstSetField *p_pFirstSetField, CRspFsQry10308027Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQry10308028(CFirstSetField *p_pFirstSetField, CRspFsQry10308028Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع���չ�ں�Լ��ѯ��Ӧ
  virtual int OnRspQry10308029(CFirstSetField *p_pFirstSetField, CRspFsQry10308029Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  virtual int OnRspQry10308074(CFirstSetField *p_pFirstSetField, CRspFsQry10308074Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩֱ�ӻ�����Ӧ
  virtual int OnRsp10308615(CFirstSetField *p_pFirstSetField, CRspFs10308615Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ��Լ��ѯ��Ӧ
  virtual int OnRspQry10308030(CFirstSetField *p_pFirstSetField, CRspFsQry10308030Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ�ɻ���ծ��ѯ��Ӧ
  virtual int OnRspQry10308085(CFirstSetField *p_pFirstSetField, CRspFsQry10308085Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ������Ȩ������ϸ��ѯ��Ӧ
  virtual int OnRspQry10308087(CFirstSetField *p_pFirstSetField, CRspFsQry10308087Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ�ɶ������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308031(CFirstSetField *p_pFirstSetField, CRspFsQry10308031Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱͶƱ�����ѯ��Ӧ
  virtual int OnRspQry10308032(CFirstSetField *p_pFirstSetField, CRspFsQry10308032Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ�鰸��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308033(CFirstSetField *p_pFirstSetField, CRspFsQry10308033Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ͶƱ��Ʒ��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308059(CFirstSetField *p_pFirstSetField, CRspFsQry10308059Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�۲���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308051(CFirstSetField *p_pFirstSetField, CRspFsQry10308051Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308050(CFirstSetField *p_pFirstSetField, CRspFsQry10308050Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�г�״̬��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308061(CFirstSetField *p_pFirstSetField, CRspFsQry10308061Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ���֤ȯ��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308034(CFirstSetField *p_pFirstSetField, CRspFsQry10308034Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ����������ѯ��Ӧ
  virtual int OnRspQry10308035(CFirstSetField *p_pFirstSetField, CRspFsQry10308035Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �۹�ͨ�ʽ��ѯ��Ӧ
  virtual int OnRspQry10308047(CFirstSetField *p_pFirstSetField, CRspFsQry10308047Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ڽ���������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308036(CFirstSetField *p_pFirstSetField, CRspFsQry10308036Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �̺������ѯ��Ӧ
  virtual int OnRspQry10308108(CFirstSetField *p_pFirstSetField, CRspFsQry10308108Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����Ͷ���߿ɽ���֤ȯ��ѯ��Ӧ
  virtual int OnRspQry10308037(CFirstSetField *p_pFirstSetField, CRspFsQry10308037Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ת�ֲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308067(CFirstSetField *p_pFirstSetField, CRspFsQry10308067Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��תѯ��ȷ����Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308109(CFirstSetField *p_pFirstSetField, CRspFsQry10308109Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ת����ҵ����Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308110(CFirstSetField *p_pFirstSetField, CRspFsQry10308110Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ��Ӧ
  virtual int OnRspQry10308038(CFirstSetField *p_pFirstSetField, CRspFsQry10308038Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ��Ӧ
  virtual int OnRspQry10308039(CFirstSetField *p_pFirstSetField, CRspFsQry10308039Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ��Ӧ
  virtual int OnRspQry10308062(CFirstSetField *p_pFirstSetField, CRspFsQry10308062Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308063(CFirstSetField *p_pFirstSetField, CRspFsQry10308063Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ʊ��Ѻ���֤ȯ��ѯ��Ӧ
  virtual int OnRspQry10308076(CFirstSetField *p_pFirstSetField, CRspFsQry10308076Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ��Ӧ
  virtual int OnRspQry10308077(CFirstSetField *p_pFirstSetField, CRspFsQry10308077Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ��Ӧ
  virtual int OnRspQry10308040(CFirstSetField *p_pFirstSetField, CRspFsQry10308040Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ��Ӧ
  virtual int OnRspQry10308041(CFirstSetField *p_pFirstSetField, CRspFsQry10308041Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع������ѯ�걨��Ӧ
  virtual int OnRsp10308613(CFirstSetField *p_pFirstSetField, CRspFs10308613Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع��ǹ������������ѯ��Ӧ
  virtual int OnRspQry10308072(CFirstSetField *p_pFirstSetField, CRspFsQry10308072Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع��������������ѯ��Ӧ
  virtual int OnRspQry10308073(CFirstSetField *p_pFirstSetField, CRspFsQry10308073Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع�δ����Э���ѯ��Ӧ
  virtual int OnRspQry10308075(CFirstSetField *p_pFirstSetField, CRspFsQry10308075Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�Э��ع���Լ��ѯ��Ӧ
  virtual int OnRspQry10308078(CFirstSetField *p_pFirstSetField, CRspFsQry10308078Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����ծȯЭ��ع����ؽ���ѯ��Ӧ
  virtual int OnRspQry10308086(CFirstSetField *p_pFirstSetField, CRspFsQry10308086Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��Ѻ�ɷ���ϸ��ѯ��Ӧ
  virtual int OnRspQry10308042(CFirstSetField *p_pFirstSetField, CRspFsQry10308042Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ծȯ��Ѻʽ�ع���Լ��ѯ��Ӧ
  virtual int OnRspQry10308043(CFirstSetField *p_pFirstSetField, CRspFsQry10308043Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�����Ϲ����볷����Ӧ
  virtual int OnRsp10308604(CFirstSetField *p_pFirstSetField, CRspFs10308604Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF����ί����Ӧ
  virtual int OnRsp10308605(CFirstSetField *p_pFirstSetField, CRspFs10308605Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�����Ϲ������ѯ��Ӧ
  virtual int OnRspQry10308057(CFirstSetField *p_pFirstSetField, CRspFsQry10308057Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF�ɷֹɳֲֲ�ѯ��Ӧ
  virtual int OnRspQry10308064(CFirstSetField *p_pFirstSetField, CRspFsQry10308064Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308052(CFirstSetField *p_pFirstSetField, CRspFsQry10308052Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308044(CFirstSetField *p_pFirstSetField, CRspFsQry10308044Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308045(CFirstSetField *p_pFirstSetField, CRspFsQry10308045Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��Ȩ��Ա��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308046(CFirstSetField *p_pFirstSetField, CRspFsQry10308046Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ����˰������Ϣά����Ӧ
  virtual int OnRspQry10308065(CFirstSetField *p_pFirstSetField, CRspFsQry10308065Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ҪԼ�չ�ת����ϵ���ѯ��Ӧ
  virtual int OnRspQry10308058(CFirstSetField *p_pFirstSetField, CRspFsQry10308058Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �Ϻ����ֹɷݶ�Ȳ�ѯ��Ӧ
  virtual int OnRspQry10308070(CFirstSetField *p_pFirstSetField, CRspFsQry10308070Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���ڼ��ֹɷݶ�Ȳ�ѯ��Ӧ
  virtual int OnRspQry10308071(CFirstSetField *p_pFirstSetField, CRspFsQry10308071Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // Ȩ֤��Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308084(CFirstSetField *p_pFirstSetField, CRspFsQry10308084Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����ع�������Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308079(CFirstSetField *p_pFirstSetField, CRspFsQry10308079Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // �����ع���Ѻ���ѯ��Ӧ
  virtual int OnRspQry10308080(CFirstSetField *p_pFirstSetField, CRspFsQry10308080Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع�ת���ɽ���Ϣ���ѯ��Ӧ
  virtual int OnRspQry10308081(CFirstSetField *p_pFirstSetField, CRspFsQry10308081Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع���Լ�ܱ��ѯ��Ӧ
  virtual int OnRspQry10308082(CFirstSetField *p_pFirstSetField, CRspFsQry10308082Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ���������ع���Լ��ϸ��ѯ��Ӧ
  virtual int OnRspQry10308083(CFirstSetField *p_pFirstSetField, CRspFsQry10308083Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ��Ӧ
  virtual int OnRspQry10308104(CFirstSetField *p_pFirstSetField, CRspFsQry10308104Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ��Ӧ
  virtual int OnRspQry10308105(CFirstSetField *p_pFirstSetField, CRspFsQry10308105Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ѯ�۽��׷ǹ������������ѯ��Ӧ
  virtual int OnRspQry10308106(CFirstSetField *p_pFirstSetField, CRspFsQry10308106Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷ�ɽ���ѯ��Ӧ
  virtual int OnRspQry10418001(CFirstSetField *p_pFirstSetField, CRspFsQry10418001Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷί�в�ѯ��Ӧ
  virtual int OnRspQry10418002(CFirstSetField *p_pFirstSetField, CRspFsQry10418002Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ��ʷ�����ѯ��Ӧ
  virtual int OnRspQry10418003(CFirstSetField *p_pFirstSetField, CRspFsQry10418003Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliFsTradeApi : virtual public CCliTradeApi
{
public:
  // Ĭ�Ϲ��캯��
  CCliFsTradeApi(void);

  // ��������
  virtual ~CCliFsTradeApi(void);

  // ������Ϣ��ѯ����
  virtual int ReqQry10308011(CReqFsQry10308011Field *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ��Ϣ��ѯ����
  virtual int ReqQry10308013(CReqFsQry10308013Field *p_pReqField, LONGLONG p_llRequestId);
  // �ֵ���Ϣ��ѯ����
  virtual int ReqQry10308012(CReqFsQry10308012Field *p_pReqField, LONGLONG p_llRequestId);
  // ϵͳ״̬��ѯ����
  virtual int ReqQry10308014(CReqFsQry10308014Field *p_pReqField, LONGLONG p_llRequestId);
  // ��֤ת������
  virtual int Req10510605(CReqFs10510605Field *p_pReqField, LONGLONG p_llRequestId);
  // ȯ�̷���������������
  virtual int ReqQry10510606(CReqFsQry10510606Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ѯ��֤ҵ����ˮ����
  virtual int ReqQry10510608(CReqFsQry10510608Field *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ����ֽ�ת����
  virtual int Req10510620(CReqFs10510620Field *p_pReqField, LONGLONG p_llRequestId);
  // �˻��ʽ�鼯����
  virtual int ReqQry10510621(CReqFsQry10510621Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ѯת�������ʻ�����
  virtual int ReqQry10510601(CReqFsQry10510601Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻��ʽ𻮲�����
  virtual int ReqQry10510468(CReqFsQry10510468Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻�������ˮ��ѯ����
  virtual int ReqQry10510473(CReqFsQry10510473Field *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ���¼����
  virtual int ReqQry10301105(CReqFsQry10301105Field *p_pReqField, LONGLONG p_llRequestId);
  // �ʲ��˻���ѯ����
  virtual int ReqQry10308112(CReqFsQry10308112Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ƹ��˻���ѯ����
  virtual int ReqQry10308113(CReqFsQry10308113Field *p_pReqField, LONGLONG p_llRequestId);
  // �ɶ���ѯ����
  virtual int ReqQry10308001(CReqFsQry10308001Field *p_pReqField, LONGLONG p_llRequestId);
  // �ɶ�����Э���ѯ����
  virtual int ReqQry10308010(CReqFsQry10308010Field *p_pReqField, LONGLONG p_llRequestId);
  // �ʵ���ƥ��������
  virtual int Req10308611(CReqFs10308611Field *p_pReqField, LONGLONG p_llRequestId);
  // �ͻ��ʵ��Բ�ѯ����
  virtual int ReqQry10308068(CReqFsQry10308068Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʒ�ʵ��Բ�ѯ����
  virtual int ReqQry10308069(CReqFsQry10308069Field *p_pReqField, LONGLONG p_llRequestId);
  // �ʽ��ѯ����
  virtual int ReqQry10308048(CReqFsQry10308048Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ʲ���ѯ����
  virtual int ReqQry10308002(CReqFsQry10308002Field *p_pReqField, LONGLONG p_llRequestId);
  // �ɷ��ʲ���ѯ����
  virtual int ReqQry10308003(CReqFsQry10308003Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ί�в�ѯ����
  virtual int ReqQry10308004(CReqFsQry10308004Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ճɽ���ѯ����
  virtual int ReqQry10308006(CReqFsQry10308006Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ճɽ�ͳ������
  virtual int ReqQry10308053(CReqFsQry10308053Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ɽ���������������
  virtual int ReqQry10308056(CReqFsQry10308056Field *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ��������ί���걨����
  virtual int Req10308601(CReqFs10308601Field *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ����ί�г�������
  virtual int Req10308607(CReqFs10308607Field *p_pReqField, LONGLONG p_llRequestId);
  // �ɳ���ί�в�ѯ����
  virtual int ReqQry10308054(CReqFsQry10308054Field *p_pReqField, LONGLONG p_llRequestId);
  // ֤ȯ����������������
  virtual int Req10308606(CReqFs10308606Field *p_pReqField, LONGLONG p_llRequestId);
  // ���óɱ�����
  virtual int Req10308602(CReqFs10308602Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���гֲ���ϸ��ѯ����
  virtual int ReqQry10308015(CReqFsQry10308015Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ����
  virtual int ReqQry10308016(CReqFsQry10308016Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ծԤ���в�Ʒ��Ϣ��ѯ����
  virtual int ReqQry10308017(CReqFsQry10308017Field *p_pReqField, LONGLONG p_llRequestId);
  // �¹���Ų�ѯ����
  virtual int ReqQry10418004(CReqFsQry10418004Field *p_pReqField, LONGLONG p_llRequestId);
  // �¹���ǩ��ѯ����
  virtual int ReqQry10418005(CReqFsQry10418005Field *p_pReqField, LONGLONG p_llRequestId);
  // �¹��깺��ֵ��Ȳ�ѯ����
  virtual int ReqQry10418006(CReqFsQry10418006Field *p_pReqField, LONGLONG p_llRequestId);
  // �¹���Ϣ��ѯ����
  virtual int ReqQry10418007(CReqFsQry10418007Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�Ʒ����Ϣ��ѯ����
  virtual int ReqQry10308019(CReqFsQry10308019Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ����������Ϣ��ѯ����
  virtual int ReqQry10308020(CReqFsQry10308020Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ���ѯ����
  virtual int ReqQry10308021(CReqFsQry10308021Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Լչ������
  virtual int Req10308608(CReqFs10308608Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���Ѻ�ʽ��ѯ����
  virtual int ReqQry10308022(CReqFsQry10308022Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�ԤԼ��ˮ��ѯ����
  virtual int ReqQry10308023(CReqFsQry10308023Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�ԤԼ����
  virtual int Req10308610(CReqFs10308610Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�����ǰ���غ�Լ��ѯ����
  virtual int ReqQry10308024(CReqFsQry10308024Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���չ��ȡ����Լ��ѯ����
  virtual int ReqQry10308025(CReqFsQry10308025Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ����
  virtual int ReqQry10308026(CReqFsQry10308026Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ����
  virtual int ReqQry10308027(CReqFsQry10308027Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ����
  virtual int ReqQry10308028(CReqFsQry10308028Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع���չ�ں�Լ��ѯ����
  virtual int ReqQry10308029(CReqFsQry10308029Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ����
  virtual int ReqQry10308074(CReqFsQry10308074Field *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩֱ�ӻ�������
  virtual int Req10308615(CReqFs10308615Field *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ��Լ��ѯ����
  virtual int ReqQry10308030(CReqFsQry10308030Field *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ�ɻ���ծ��ѯ����
  virtual int ReqQry10308085(CReqFsQry10308085Field *p_pReqField, LONGLONG p_llRequestId);
  // ������Ȩ������ϸ��ѯ����
  virtual int ReqQry10308087(CReqFsQry10308087Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ�ɶ������Ϣ��ѯ����
  virtual int ReqQry10308031(CReqFsQry10308031Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱͶƱ�����ѯ����
  virtual int ReqQry10308032(CReqFsQry10308032Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ�鰸��Ϣ��ѯ����
  virtual int ReqQry10308033(CReqFsQry10308033Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ͶƱ��Ʒ��Ϣ��ѯ����
  virtual int ReqQry10308059(CReqFsQry10308059Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�۲���Ϣ��ѯ����
  virtual int ReqQry10308051(CReqFsQry10308051Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ������Ϣ��ѯ����
  virtual int ReqQry10308050(CReqFsQry10308050Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�г�״̬��Ϣ��ѯ����
  virtual int ReqQry10308061(CReqFsQry10308061Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ���֤ȯ��Ϣ��ѯ����
  virtual int ReqQry10308034(CReqFsQry10308034Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ����������ѯ����
  virtual int ReqQry10308035(CReqFsQry10308035Field *p_pReqField, LONGLONG p_llRequestId);
  // �۹�ͨ�ʽ��ѯ����
  virtual int ReqQry10308047(CReqFsQry10308047Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ڽ���������Ϣ��ѯ����
  virtual int ReqQry10308036(CReqFsQry10308036Field *p_pReqField, LONGLONG p_llRequestId);
  // �̺������ѯ����
  virtual int ReqQry10308108(CReqFsQry10308108Field *p_pReqField, LONGLONG p_llRequestId);
  // ����Ͷ���߿ɽ���֤ȯ��ѯ����
  virtual int ReqQry10308037(CReqFsQry10308037Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ת�ֲ���Ϣ��ѯ����
  virtual int ReqQry10308067(CReqFsQry10308067Field *p_pReqField, LONGLONG p_llRequestId);
  // ��תѯ��ȷ����Ϣ��ѯ����
  virtual int ReqQry10308109(CReqFsQry10308109Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ת����ҵ����Ϣ��ѯ����
  virtual int ReqQry10308110(CReqFsQry10308110Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ����
  virtual int ReqQry10308038(CReqFsQry10308038Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ����
  virtual int ReqQry10308039(CReqFsQry10308039Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ����
  virtual int ReqQry10308062(CReqFsQry10308062Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ����
  virtual int ReqQry10308063(CReqFsQry10308063Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ʊ��Ѻ���֤ȯ��ѯ����
  virtual int ReqQry10308076(CReqFsQry10308076Field *p_pReqField, LONGLONG p_llRequestId);
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ����
  virtual int ReqQry10308077(CReqFsQry10308077Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ����
  virtual int ReqQry10308040(CReqFsQry10308040Field *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ����
  virtual int ReqQry10308041(CReqFsQry10308041Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع������ѯ�걨����
  virtual int Req10308613(CReqFs10308613Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع��ǹ������������ѯ����
  virtual int ReqQry10308072(CReqFsQry10308072Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع��������������ѯ����
  virtual int ReqQry10308073(CReqFsQry10308073Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع�δ����Э���ѯ����
  virtual int ReqQry10308075(CReqFsQry10308075Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�Э��ع���Լ��ѯ����
  virtual int ReqQry10308078(CReqFsQry10308078Field *p_pReqField, LONGLONG p_llRequestId);
  // ����ծȯЭ��ع����ؽ���ѯ����
  virtual int ReqQry10308086(CReqFsQry10308086Field *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��Ѻ�ɷ���ϸ��ѯ����
  virtual int ReqQry10308042(CReqFsQry10308042Field *p_pReqField, LONGLONG p_llRequestId);
  // ծȯ��Ѻʽ�ع���Լ��ѯ����
  virtual int ReqQry10308043(CReqFsQry10308043Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF�����Ϲ����볷������
  virtual int Req10308604(CReqFs10308604Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF����ί������
  virtual int Req10308605(CReqFs10308605Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF�����Ϲ������ѯ����
  virtual int ReqQry10308057(CReqFsQry10308057Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF�ɷֹɳֲֲ�ѯ����
  virtual int ReqQry10308064(CReqFsQry10308064Field *p_pReqField, LONGLONG p_llRequestId);
  // ETF��Ϣ��ѯ����
  virtual int ReqQry10308052(CReqFsQry10308052Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ����
  virtual int ReqQry10308044(CReqFsQry10308044Field *p_pReqField, LONGLONG p_llRequestId);
  // ����LOF����ֲ�ϲ���Ϣ��ѯ����
  virtual int ReqQry10308045(CReqFsQry10308045Field *p_pReqField, LONGLONG p_llRequestId);
  // ��Ȩ��Ա��Ϣ��ѯ����
  virtual int ReqQry10308046(CReqFsQry10308046Field *p_pReqField, LONGLONG p_llRequestId);
  // ����˰������Ϣά������
  virtual int ReqQry10308065(CReqFsQry10308065Field *p_pReqField, LONGLONG p_llRequestId);
  // ҪԼ�չ�ת����ϵ���ѯ����
  virtual int ReqQry10308058(CReqFsQry10308058Field *p_pReqField, LONGLONG p_llRequestId);
  // �Ϻ����ֹɷݶ�Ȳ�ѯ����
  virtual int ReqQry10308070(CReqFsQry10308070Field *p_pReqField, LONGLONG p_llRequestId);
  // ���ڼ��ֹɷݶ�Ȳ�ѯ����
  virtual int ReqQry10308071(CReqFsQry10308071Field *p_pReqField, LONGLONG p_llRequestId);
  // Ȩ֤��Ϣ��ѯ����
  virtual int ReqQry10308084(CReqFsQry10308084Field *p_pReqField, LONGLONG p_llRequestId);
  // �����ع�������Ϣ��ѯ����
  virtual int ReqQry10308079(CReqFsQry10308079Field *p_pReqField, LONGLONG p_llRequestId);
  // �����ع���Ѻ���ѯ����
  virtual int ReqQry10308080(CReqFsQry10308080Field *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع�ת���ɽ���Ϣ���ѯ����
  virtual int ReqQry10308081(CReqFsQry10308081Field *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع���Լ�ܱ��ѯ����
  virtual int ReqQry10308082(CReqFsQry10308082Field *p_pReqField, LONGLONG p_llRequestId);
  // ���������ع���Լ��ϸ��ѯ����
  virtual int ReqQry10308083(CReqFsQry10308083Field *p_pReqField, LONGLONG p_llRequestId);
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ����
  virtual int ReqQry10308104(CReqFsQry10308104Field *p_pReqField, LONGLONG p_llRequestId);
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ����
  virtual int ReqQry10308105(CReqFsQry10308105Field *p_pReqField, LONGLONG p_llRequestId);
  // ѯ�۽��׷ǹ������������ѯ����
  virtual int ReqQry10308106(CReqFsQry10308106Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷ�ɽ���ѯ����
  virtual int ReqQry10418001(CReqFsQry10418001Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷί�в�ѯ����
  virtual int ReqQry10418002(CReqFsQry10418002Field *p_pReqField, LONGLONG p_llRequestId);
  // ��ʷ�����ѯ����
  virtual int ReqQry10418003(CReqFsQry10418003Field *p_pReqField, LONGLONG p_llRequestId);

public:
  // �첽�ص�����
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // �����ص�����
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // ������Ϣ��ѯ��Ӧ
  void OnRspQry10308011(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��Ϣ��ѯ��Ӧ
  void OnRspQry10308013(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ֵ���Ϣ��ѯ��Ӧ
  void OnRspQry10308012(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ϵͳ״̬��ѯ��Ӧ
  void OnRspQry10308014(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��֤ת����Ӧ
  void OnRsp10510605(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ȯ�̷�������������Ӧ
  void OnRspQry10510606(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ѯ��֤ҵ����ˮ��Ӧ
  void OnRspQry10510608(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ����ֽ�ת��Ӧ
  void OnRsp10510620(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �˻��ʽ�鼯��Ӧ
  void OnRspQry10510621(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ѯת�������ʻ���Ӧ
  void OnRspQry10510601(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻��ʽ𻮲���Ӧ
  void OnRspQry10510468(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻�������ˮ��ѯ��Ӧ
  void OnRspQry10510473(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ���¼��Ӧ
  void OnRspQry10301105(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʲ��˻���ѯ��Ӧ
  void OnRspQry10308112(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ƹ��˻���ѯ��Ӧ
  void OnRspQry10308113(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɶ���ѯ��Ӧ
  void OnRspQry10308001(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɶ�����Э���ѯ��Ӧ
  void OnRspQry10308010(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʵ���ƥ������Ӧ
  void OnRsp10308611(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ͻ��ʵ��Բ�ѯ��Ӧ
  void OnRspQry10308068(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʒ�ʵ��Բ�ѯ��Ӧ
  void OnRspQry10308069(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ʽ��ѯ��Ӧ
  void OnRspQry10308048(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ʲ���ѯ��Ӧ
  void OnRspQry10308002(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɷ��ʲ���ѯ��Ӧ
  void OnRspQry10308003(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ί�в�ѯ��Ӧ
  void OnRspQry10308004(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ճɽ���ѯ��Ӧ
  void OnRspQry10308006(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ճɽ�ͳ����Ӧ
  void OnRspQry10308053(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ɽ�������������Ӧ
  void OnRspQry10308056(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��������ί���걨��Ӧ
  void OnRsp10308601(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ����ί�г�����Ӧ
  void OnRsp10308607(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �ɳ���ί�в�ѯ��Ӧ
  void OnRspQry10308054(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ֤ȯ��������������Ӧ
  void OnRsp10308606(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���óɱ���Ӧ
  void OnRsp10308602(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���гֲ���ϸ��ѯ��Ӧ
  void OnRspQry10308015(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���пͻ�ƽ����ϸ��ѯ��Ӧ
  void OnRspQry10308016(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ծԤ���в�Ʒ��Ϣ��ѯ��Ӧ
  void OnRspQry10308017(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���Ų�ѯ��Ӧ
  void OnRspQry10418004(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���ǩ��ѯ��Ӧ
  void OnRspQry10418005(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹��깺��ֵ��Ȳ�ѯ��Ӧ
  void OnRspQry10418006(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �¹���Ϣ��ѯ��Ӧ
  void OnRspQry10418007(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�Ʒ����Ϣ��ѯ��Ӧ
  void OnRspQry10308019(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ����������Ϣ��ѯ��Ӧ
  void OnRspQry10308020(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ���ѯ��Ӧ
  void OnRspQry10308021(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Լչ����Ӧ
  void OnRsp10308608(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���Ѻ�ʽ��ѯ��Ӧ
  void OnRspQry10308022(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�ԤԼ��ˮ��ѯ��Ӧ
  void OnRspQry10308023(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�ԤԼ��Ӧ
  void OnRsp10308610(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�����ǰ���غ�Լ��ѯ��Ӧ
  void OnRspQry10308024(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���չ��ȡ����Լ��ѯ��Ӧ
  void OnRspQry10308025(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع��ɵ����Զ�չ�ں�Լ��ѯ��Ӧ
  void OnRspQry10308026(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���ȡ����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  void OnRspQry10308027(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���ȡ����ʼ����ԤԼ��Լ��ѯ��Ӧ
  void OnRspQry10308028(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع���չ�ں�Լ��ѯ��Ӧ
  void OnRspQry10308029(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ۻع�����ǰ��ֹԤԼ��Լ��ѯ��Ӧ
  void OnRspQry10308074(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩֱ�ӻ�����Ӧ
  void OnRsp10308615(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ��Լ��ѯ��Ӧ
  void OnRspQry10308030(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ�ɻ���ծ��ѯ��Ӧ
  void OnRspQry10308085(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ������Ȩ������ϸ��ѯ��Ӧ
  void OnRspQry10308087(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ�ɶ������Ϣ��ѯ��Ӧ
  void OnRspQry10308031(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱͶƱ�����ѯ��Ӧ
  void OnRspQry10308032(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ�鰸��Ϣ��ѯ��Ӧ
  void OnRspQry10308033(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ͶƱ��Ʒ��Ϣ��ѯ��Ӧ
  void OnRspQry10308059(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�۲���Ϣ��ѯ��Ӧ
  void OnRspQry10308051(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ������Ϣ��ѯ��Ӧ
  void OnRspQry10308050(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�г�״̬��Ϣ��ѯ��Ӧ
  void OnRspQry10308061(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ���֤ȯ��Ϣ��ѯ��Ӧ
  void OnRspQry10308034(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ����������ѯ��Ӧ
  void OnRspQry10308035(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �۹�ͨ�ʽ��ѯ��Ӧ
  void OnRspQry10308047(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ڽ���������Ϣ��ѯ��Ӧ
  void OnRspQry10308036(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �̺������ѯ��Ӧ
  void OnRspQry10308108(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����Ͷ���߿ɽ���֤ȯ��ѯ��Ӧ
  void OnRspQry10308037(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ת�ֲ���Ϣ��ѯ��Ӧ
  void OnRspQry10308067(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��תѯ��ȷ����Ϣ��ѯ��Ӧ
  void OnRspQry10308109(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ת����ҵ����Ϣ��ѯ��Ӧ
  void OnRspQry10308110(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��Լ�ܱ��ѯ��Ӧ
  void OnRspQry10308038(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��Լ��ϸ��ѯ��Ӧ
  void OnRspQry10308039(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ��ʼ���ײο�����ѯ��Ӧ
  void OnRspQry10308062(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ�ɷݶ����Ϣ��ѯ��Ӧ
  void OnRspQry10308063(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ʊ��Ѻ���֤ȯ��ѯ��Ӧ
  void OnRspQry10308076(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����˹�Ʊ��Ѻ��Ȳ�ѯ��Ӧ
  void OnRspQry10308077(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ծȯ��ѺʽЭ��ع���Լ��ѯ��Ӧ
  void OnRspQry10308040(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��ѺʽЭ��ع����֤ȯ��ѯ��Ӧ
  void OnRspQry10308041(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع������ѯ�걨��Ӧ
  void OnRsp10308613(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع��ǹ������������ѯ��Ӧ
  void OnRspQry10308072(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع��������������ѯ��Ӧ
  void OnRspQry10308073(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع�δ����Э���ѯ��Ӧ
  void OnRspQry10308075(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�Э��ع���Լ��ѯ��Ӧ
  void OnRspQry10308078(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����ծȯЭ��ع����ؽ���ѯ��Ӧ
  void OnRspQry10308086(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��Ѻ�ɷ���ϸ��ѯ��Ӧ
  void OnRspQry10308042(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ծȯ��Ѻʽ�ع���Լ��ѯ��Ӧ
  void OnRspQry10308043(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�����Ϲ����볷����Ӧ
  void OnRsp10308604(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF����ί����Ӧ
  void OnRsp10308605(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�����Ϲ������ѯ��Ӧ
  void OnRspQry10308057(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF�ɷֹɳֲֲ�ѯ��Ӧ
  void OnRspQry10308064(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF��Ϣ��ѯ��Ӧ
  void OnRspQry10308052(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ�LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  void OnRspQry10308044(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����LOF����ֲ�ϲ���Ϣ��ѯ��Ӧ
  void OnRspQry10308045(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��Ȩ��Ա��Ϣ��ѯ��Ӧ
  void OnRspQry10308046(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ����˰������Ϣά����Ӧ
  void OnRspQry10308065(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ҪԼ�չ�ת����ϵ���ѯ��Ӧ
  void OnRspQry10308058(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �Ϻ����ֹɷݶ�Ȳ�ѯ��Ӧ
  void OnRspQry10308070(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���ڼ��ֹɷݶ�Ȳ�ѯ��Ӧ
  void OnRspQry10308071(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // Ȩ֤��Ϣ��ѯ��Ӧ
  void OnRspQry10308084(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����ع�������Ϣ��ѯ��Ӧ
  void OnRspQry10308079(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // �����ع���Ѻ���ѯ��Ӧ
  void OnRspQry10308080(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع�ת���ɽ���Ϣ���ѯ��Ӧ
  void OnRspQry10308081(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع���Լ�ܱ��ѯ��Ӧ
  void OnRspQry10308082(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ���������ع���Լ��ϸ��ѯ��Ӧ
  void OnRspQry10308083(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H��ȫ��ͨ�ɶ��˻���Ϣ��ѯ��Ӧ
  void OnRspQry10308104(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H�ɾ���֤ȯ�����Ӧ��ϵ��ѯ��Ӧ
  void OnRspQry10308105(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ѯ�۽��׷ǹ������������ѯ��Ӧ
  void OnRspQry10308106(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷ�ɽ���ѯ��Ӧ
  void OnRspQry10418001(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷί�в�ѯ��Ӧ
  void OnRspQry10418002(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ��ʷ�����ѯ��Ӧ
  void OnRspQry10418003(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FS_TRADE_API_H__