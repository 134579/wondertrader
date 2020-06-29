#pragma once

#include "WtSimpDataMgr.h"

#include "../WtCore/WtExecuter.h"
#include "../WtCore/TraderAdapter.h"
#include "../WtCore/ParserAdapter.h"
#include "../WtCore/ActionPolicyMgr.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_OTP_BEGIN
class WTSVariant;
NS_OTP_END

USING_NS_OTP;

class WtExecRunner : public IParserStub
{
public:
	/*
	 *	��ʼ��
	 */
	bool init(const char* logProfile = "logcfgexec.json");

	bool config(const char* cfgFile);

	void run(bool bAsync = false);

	void release();

	bool addExeFactories(const char* folder);

	IBaseDataMgr*	get_bd_mgr() { return &_bd_mgr; }

	IHotMgr* get_hot_mgr() { return &_hot_mgr; }

	WTSSessionInfo* get_session_info(const char* sid, bool isCode = true);

	//////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// ����ʵʱ��������
	/// </summary>
	/// <param name="curTick">���µ�tick����</param>
	/// <param name="isHot">�Ƿ���������Լ����</param>
	virtual void handle_push_quote(WTSTickData* curTick, bool isHot = false) override;

private:
	bool initTraders();
	bool initParsers();
	bool initExecuters();
	bool initDataMgr();
	bool initActionPolicy();

private:
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	WtExecuterFactory	_exe_factory;

	WTSVariant*			_config;

	WtSimpDataMgr		_data_mgr;

	WTSBaseDataMgr		_bd_mgr;
	WTSHotMgr			_hot_mgr;
	ActionPolicyMgr		_act_policy;
};

