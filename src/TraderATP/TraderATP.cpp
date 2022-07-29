/*!
 * \file TraderATP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderATP.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/AtpTradeApi/x64/atptradeapi.lib")
#else
#pragma comment(lib, "../API/AtpTradeApi/x86/atptradeapi.lib")
#endif
#endif

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	const char* buffer = fmtutil::format(format, args...);

	sink->handleTraderLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderATP *instance = new TraderATP();
		return instance;
	}

	EXPORT_FLAG void deleteTrader(ITraderApi* &trader)
	{
		if (NULL != trader)
		{
			delete trader;
			trader = NULL;
		}
	}
}

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if(auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}


TraderATP::TraderATP()
	: _api(NULL)
	, _sink(NULL)
	, _ordref(makeRefID())
	, _reqid(1)
	, _orders(NULL)
	, _trades(NULL)
	, _positions(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _close_flag(false)
{
}


TraderATP::~TraderATP()
{
}


/*
WTSEntrust* TraderATP::makeEntrust(XTPOrderInfo* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)order_info->quantity,
		order_info->price,
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	pRet->setOffsetType(wrapOffsetType(order_info->position_effect));
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), order_info->order_client_id);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderATP::makeOrderInfo(XTPQueryOrderRsp* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(order_info->price);
	pRet->setVolume((uint32_t)order_info->quantity);
	pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	pRet->setOrderFlag(WOF_NOR);
	pRet->setOffsetType(wrapOffsetType(order_info->position_effect));

	pRet->setVolTraded((uint32_t)order_info->qty_traded);
	pRet->setVolLeft((uint32_t)order_info->qty_left);

	pRet->setCode(code.c_str());
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate((uint32_t)(order_info->insert_time / 1000000000));
	uint32_t uTime = order_info->insert_time % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderState(order_info->order_status));
	if (order_info->order_status >= XTP_ORDER_STATUS_REJECTED)
		pRet->setError(true);

	pRet->setEntrustID(genEntrustID(order_info->order_client_id).c_str());
	pRet->setOrderID(fmt::format("{}", order_info->order_xtp_id).c_str());

	pRet->setStateMsg("");

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) == 0)
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag);

		if (strlen(pRet->getOrderID()) > 0)
		{
			m_oidCache.put(StrUtil::trim(pRet->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
				write_log(_sink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSTradeInfo* TraderATP::makeTradeInfo(XTPQueryTradeRsp* trade_info)
{
	std::string code, exchg;
	if (trade_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = trade_info->ticker;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)trade_info->quantity);
	pRet->setPrice(trade_info->price);
	pRet->setTradeID(trade_info->exec_id);
	pRet->setContractInfo(contract);

	uint32_t uTime = (uint32_t)(trade_info->trade_time % 1000000000);
	uint32_t uDate = (uint32_t)(trade_info->trade_time / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	WTSDirectionType dType = wrapDirectionType(trade_info->side, trade_info->position_effect);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(trade_info->position_effect));
	pRet->setRefOrder(fmt::format("{}", trade_info->order_xtp_id).c_str());
	pRet->setTradeType(WTT_Common);

	double amount = trade_info->quantity*pRet->getPrice();
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}
*/

void TraderATP::OnLogin(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, 0);

	write_log(_sink, LL_WARN, "[TraderrXTP] {} login success: {}...", _user, reason);
}

void TraderATP::OnLogout(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Logout, 0);

	write_log(_sink, LL_WARN, "[TraderrXTP] {} logout: {}...", _user, reason);
}

void TraderATP::OnConnectFailure(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, -1);

	write_log(_sink, LL_WARN, "[TraderrXTP] Connect failed {}...", reason);
}

void TraderATP::OnConnectTimeOut(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Connect, -1);

	write_log(_sink, LL_WARN, "[TraderrXTP] Connect timeout {}...", reason);
}

void TraderATP::OnHeartbeatTimeout(const std::string& reason)
{
	write_log(_sink, LL_WARN, "[TraderrXTP] Heartbeat timeout {}...", reason);
}

void TraderATP::OnClosed(const std::string& reason)
{
	write_log(_sink, LL_WARN, "[TraderrXTP] Server closed {}...", reason);
}

void TraderATP::OnEndOfConnection(const std::string& reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, -1);

	write_log(_sink, LL_WARN, "[TraderrXTP] Connection ended {}...", reason);

	_close_flag = true;
}

void TraderATP::OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg &cust_login_resp)
{
	if(cust_login_resp.permisson_error_code == 0)
	{
		_tradingday = TimeUtils::getCurDate();
		_cust_id = cust_login_resp.cust_id;

		{
			//��ʼ��ί�е�������
			std::stringstream ss;
			ss << "./atpdata/local/";
			std::string path = StrUtil::standardisePath(ss.str());
			if (!StdFile::exists(path.c_str()))
				boost::filesystem::create_directories(path.c_str());
			ss << _user << "_eid.sc";
			m_eidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
				write_log(_sink, LL_WARN, message);
			});
		}

		{
			//��ʼ��������ǻ�����
			std::stringstream ss;
			ss << "./atpdata/local/";
			std::string path = StrUtil::standardisePath(ss.str());
			if (!StdFile::exists(path.c_str()))
				boost::filesystem::create_directories(path.c_str());
			ss << _user << "_oid.sc";
			m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
				write_log(_sink, LL_WARN, message);
			});
		}

		if (_sink)
			_sink->onLoginResult(true, "", _tradingday);
		_state = TS_ALLREADY;
	}
	else
	{
		if (_sink)
			_sink->onLoginResult(false, fmt::format("{}", cust_login_resp.permisson_error_code).c_str(), _tradingday);
		_state = TS_ALLREADY;
	}
}

void TraderATP::OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &cust_logout_resp)
{
	if (cust_logout_resp.permisson_error_code == 0)
	{
		_state = TS_NOTLOGIN;
		if (_sink)
			_sink->onLogout();
	}
	else
	{
		write_log(_sink, LL_ERROR, "[TraderrXTP] Logout failed: {}", cust_logout_resp.permisson_error_code);
	}
}

void TraderATP::OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
}

// �����´ｻ����ȷ��
void TraderATP::OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack)
{
}

// �ɽ��ر�
void TraderATP::OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er)
{
}

// �����´��ڲ��ܾ�
void TraderATP::OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection)
{
}

void TraderATP::OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg)
{
	WTSArray* ayFunds = WTSArray::create();
	WTSAccountInfo* fundInfo = WTSAccountInfo::create();
	fundInfo->setPreBalance(msg.init_leaves_value / 10000.0);
	fundInfo->setBalance(msg.leaves_value / 10000.0);
	fundInfo->setAvailable(msg.available_tall / 10000.0);
	ayFunds->append(fundInfo, false);

	if (_sink)
		_sink->onRspAccount(ayFunds);

	ayFunds->release();
}

void TraderATP::OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg &msg)
{
	WTSArray* ayOrders = WTSArray::create();
	for (const auto& unit : msg.order_array)
	{
		/*std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " business_type : " << (int32_t)it->business_type <<
			" security_id : " << it->security_id <<
			" security_symbol : " << it->security_symbol <<
			" market_id : " << it->market_id <<
			" account_id : " << it->account_id <<
			" side : " << it->side <<
			" ord_type : " << it->ord_type <<
			" ord_status : " << (int32_t)it->ord_status <<
			" transact_time : " << it->transact_time <<
			" order_price : " << it->order_price <<
			" exec_price : " << it->exec_price <<
			" order_qty : " << it->order_qty <<
			" leaves_qty : " << it->leaves_qty <<
			" cum_qty : " << it->cum_qty <<
			" cl_ord_no : " << it->cl_ord_no <<
			" order_id : " << it->order_id <<
			" cl_ord_id : " << it->cl_ord_id <<
			" client_seq_id : " << it->client_seq_id <<
			" orig_cl_ord_no : " << it->orig_cl_ord_no <<
			" frozen_trade_value : " << it->frozen_trade_value <<
			" frozen_fee : " << it->frozen_fee <<
			" reject_reason_code : " << it->reject_reason_code <<
			" ord_rej_reason : " << it->ord_rej_reason << std::endl;
		i++;*/
		WTSOrderInfo* ordInfo = makeOrderInfo(&unit);
		if(ordInfo == NULL)
			continue;

		ayOrders->append(ordInfo, false);
	}

	if (_sink)
		_sink->onRspOrders(ayOrders);

	ayOrders->release();
}

void TraderATP::OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg)
{
	WTSArray* ayTrades = WTSArray::create();
	for (const auto& unit : msg.order_array)
	{
		/*std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " business_type : " << (int32_t)it->business_type <<
			" security_id : " << it->security_id <<
			" security_symbol : " << it->security_symbol <<
			" market_id : " << it->market_id <<
			" account_id : " << it->account_id <<
			" side : " << it->side <<
			" ord_type : " << it->ord_type <<
			" ord_status : " << (int32_t)it->ord_status <<
			" transact_time : " << it->transact_time <<
			" order_price : " << it->order_price <<
			" exec_price : " << it->exec_price <<
			" order_qty : " << it->order_qty <<
			" leaves_qty : " << it->leaves_qty <<
			" cum_qty : " << it->cum_qty <<
			" cl_ord_no : " << it->cl_ord_no <<
			" order_id : " << it->order_id <<
			" cl_ord_id : " << it->cl_ord_id <<
			" client_seq_id : " << it->client_seq_id <<
			" orig_cl_ord_no : " << it->orig_cl_ord_no <<
			" frozen_trade_value : " << it->frozen_trade_value <<
			" frozen_fee : " << it->frozen_fee <<
			" reject_reason_code : " << it->reject_reason_code <<
			" ord_rej_reason : " << it->ord_rej_reason << std::endl;
		i++;*/
		WTSTradeInfo* trdInfo = makeTradeInfo(&unit);
		if (trdInfo == NULL)
			continue;

		ayTrades->append(trdInfo, false);
	}

	if (_sink)
		_sink->onRspTrades(ayTrades);

	ayTrades->release();
}

void TraderATP::OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg)
{
}

#pragma region "ITraderApi"
bool TraderATP::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_accountid = params->getCString("acckey");
	_accpasswd = params->getCString("acckey");

	_front = params->getCString("front");

	std::string module = params->getCString("atpmodule");
	if (module.empty()) module = "atptradeapi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;

	m_hInstATP = DLLHelper::load_library(dllpath.c_str());

	static bool inited = false;
	if(!inited)
	{
		// ��ʼ��API
		const std::string station_name = ""; // վ����Ϣ�����ֶ��Ѿ���ʹ��
		const std::string cfg_path = ".";      // �����ļ�·��
		const std::string log_dir_path = ""; // ��־·��
		bool record_all_flag = true;         // �Ƿ��¼����ί����Ϣ
		std::unordered_map<std::string, std::string> encrypt_cfg; // ���ܿ�����
		bool connection_retention_flag = false;   // �Ƿ����ûỰ����

		// encrypt_cfg������д��
		encrypt_cfg["ENCRYPT_SCHEMA"] = "0";              // �ַ� 0 ��ʾ ������Ϣ�е����� password ����
		encrypt_cfg["ATP_ENCRYPT_PASSWORD"] = "";         // �����뼰�����޸���������Ϣ�������ֶμ����㷨
		encrypt_cfg["ATP_LOGIN_ENCRYPT_PASSWORD"] = "";   // ���뼰�����޸���Ϣ�������ֶεļ����㷨so·��
		encrypt_cfg["GM_SM2_PUBLIC_KEY_PATH"] = "";       // ���ù����㷨ʱ��ͨ����key���� GM�㷨���ü���ʹ�õĹ�Կ·��
		encrypt_cfg["RSA_PUBLIC_KEY_PATH"] = "";          // ���ʹ��rsa�㷨���ܣ�ͨ����key���� rsa�㷨���ü���ʹ�õĹ�Կ·��
		ATPRetCodeType ec = ATPTradeAPI::Init(station_name, cfg_path, log_dir_path, record_all_flag, encrypt_cfg, connection_retention_flag);
		if (ec != ErrorCode::kSuccess)
		{
			write_log(_sink, LL_ERROR, "ATPTradeAPI init failed: {}", ec);
			return false;
		}

		inited = true;
	}
	return true;
}

void TraderATP::release()
{
	if (_api)
	{
		_api->Close();
		while (!_close_flag)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		delete _api;
		_api = NULL;
	}

	if (_orders)
		_orders->clear();

	if (_positions)
		_positions->clear();

	if (_trades)
		_trades->clear();
}

void TraderATP::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderATP::reconnect()
{
	if (_api)
	{
		_api->Close();
		while(!_close_flag)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		delete _api;
		_api = NULL;
	}

	_api = new ATPTradeAPI();

	// ����������Ϣ
	ATPConnectProperty prop;
	prop.user = _user;												// �����û���
	prop.password = _pass;											// �����û�����
	prop.locations = { _front, _front2};							// ���������ڵ�ĵ�ַ+�˿�
	prop.heartbeat_interval_milli = 5000;                           // ����������ʱ��������λ������
	prop.connect_timeout_milli = 5000;                              // ���ӳ�ʱʱ�䣬��λ������
	prop.reconnect_time = 10;                                       // �������Ӵ���
	prop.client_name = "WonderTrader";                              // �ͻ��˳�������
	prop.client_version = "v1.0";									// �ͻ��˳���汾
	prop.mode = 0;

	ATPRetCodeType ec = _api->Connect(prop, this);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Connect error: {}", ec);
	}
}

void TraderATP::connect()
{
	reconnect();

	if (_thrd_worker == NULL)
	{
		boost::asio::io_service::work work(_asyncio);
		_thrd_worker.reset(new StdThread([this](){
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_asyncio.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderATP::disconnect()
{
	release();
}

bool TraderATP::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderATP::genEntrustID(char* buffer, uint32_t orderRef)
{
	//���ﲻ��ʹ��sessionid����Ϊÿ�ε�½�᲻ͬ�����ʹ�õĻ������ܻ���ɲ�Ψһ�����
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderATP::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderATP::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		uint32_t orderref = _ordref.fetch_add(1) + 1;
		fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderATP::doLogin(const char* productInfo)
{
	_state = TS_LOGINING;

	// ���õ�����Ϣ
	ATPReqCustLoginOtherMsg login_msg;
	wt_strcpy(login_msg.fund_account_id, _accountid.c_str());           // �ʽ��˻�ID
	wt_strcpy(login_msg.password, _accpasswd.c_str());                  // �ͻ�������
	login_msg.login_mode = ATPCustLoginModeType::kFundAccountIDMode;	// ��¼ģʽ���ʽ��˺ŵ�¼
	login_msg.client_seq_id = genRequestID();							// �ͻ�ϵͳ��Ϣ��
	login_msg.order_way = '0';											// ί�з�ʽ������ί��
	login_msg.client_feature_code = productInfo;						// �ն�ʶ����

	ATPRetCodeType ec = _api->ReqCustLoginOther(&login_msg);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Login error: {}", ec);
	}
}

int TraderATP::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;
	_product = productInfo;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin(productInfo);
	return 0;
}

int TraderATP::logout()
{
	if (_api == NULL)
		return -1;

	ATPReqCustLogoutOtherMsg logout_msg;
	wt_strcpy(logout_msg.fund_account_id, _accountid.c_str());	// �ʽ��˻�ID
	logout_msg.client_seq_id = genRequestID();              // �ͻ�ϵͳ��Ϣ��
	logout_msg.client_feature_code = _product;				// �ն�ʶ����

	ATPRetCodeType ec = _api->ReqCustLogoutOther(&logout_msg);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderAPT] Logout error: {}", ec);
		return ec;
	}
	return 0;
}

int TraderATP::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	//XTPOrderInsertInfo req;
	//memset(&req, 0, sizeof(req));
	//
	//uint32_t orderref;
	//extractEntrustID(entrust->getEntrustID(), orderref);
	//req.order_client_id = orderref;
	//strcpy(req.ticker, entrust->getCode());
	//req.market = wt_stricmp(entrust->getExchg(), "SSE") == 0 ? XTP_MKT_SH_A : XTP_MKT_SZ_A;
	//req.price = entrust->getPrice();
	//req.quantity = (int64_t)entrust->getVolume();
	//req.price_type = XTP_PRICE_LIMIT;
	//req.side = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	//req.business_type = XTP_BUSINESS_TYPE_CASH;
	//req.position_effect = wrapOffsetType(entrust->getOffsetType());

	//if (strlen(entrust->getUserTag()) > 0)
	//{
	//	m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
	//		write_log(_sink, LL_WARN, message);
	//	});
	//}

	//uint64_t iResult = _api->InsertOrder(&req, _sessionid);
	//if (iResult == 0)
	//{
	//	auto error_info = _api->GetApiLastError();
	//	write_log(_sink,LL_ERROR, "[TraderATP] Order inserting failed: {}", error_info->error_msg);
	//}


	// �Ϻ��г���Ʊ�޼�ί��
	thread_local static ATPReqCashAuctionOrderMsg p;

	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);

	wt_strcpy(p.security_id, entrust->getCode());                   // ֤ȯ����
	p.market_id = strcmp(entrust->getExchg(), "SSE") == 0? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;             // �г�ID���Ϻ�
	wt_strcpy(p.cust_id, _cust_id.c_str());                 // �ͻ���ID
	wt_strcpy(p.fund_account_id, _accountid.c_str());       // �ʽ��˻�ID
	wt_strcpy(p.account_id, _user.c_str());                 // �˻�ID
	p.side = (entrust->getOffsetType() == WOT_OPEN) ? ATPSideConst::kBuy : ATPSideConst::kSell;     // ����������
	p.order_type = ATPOrdTypeConst::kFixedNew;				// �������ͣ��޼�
	p.price = (int32_t)(entrust->getPrice()*10000);         // ί�м۸� N13(4)��21.0000Ԫ
	p.order_qty = (int32_t)entrust->getVolume();            // �걨����N15(2)����ƱΪ�ɡ�����Ϊ�ݡ��Ϻ�ծȯĬ��Ϊ�ţ�ʹ��ʱ�������ȯ��ȷ�ϣ�������Ϊ�ţ�1000.00��
	p.client_seq_id = genRequestID();						// �û�ϵͳ��Ϣ���
	p.order_way = '0';										// ί�з�ʽ������ί��
	p.client_feature_code = _product;						// �ն�ʶ����
	fmt::format_to(p.user_info, "{}", orderref);

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	ATPRetCodeType ec = _api->ReqCashAuctionOrder(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Order inserting failed: {}", ec);
	}

	return ec;
}

int TraderATP::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	thread_local static ATPReqCancelOrderMsg p;
	p.market_id = strcmp(action->getExchg(), "SSE") == 0 ? ATPMarketIDConst::kShangHai : ATPMarketIDConst::kShenZhen;
	wt_strcpy(p.cust_id, _cust_id.c_str());                    // �ͻ���ID
	wt_strcpy(p.fund_account_id, _accountid.c_str());          // �ʽ��˻�ID
	wt_strcpy(p.account_id, _user.c_str());                   // �˻�ID
	p.client_seq_id = genRequestID();
	p.order_way = '0';
	p.orig_cl_ord_no = strtoull(action->getOrderID(), NULL, 10);

	ATPRetCodeType ec = _api->ReqCancelOrder(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Order cancelling failed: {}", ec);
	}

	return 0;
}

uint32_t TraderATP::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderATP::queryAccount()
{
	ATPReqFundQueryMsg p;

	strncpy(p.cust_id, _cust_id.c_str(), 17);
	strncpy(p.fund_account_id, _accountid.c_str(), 17);
	strncpy(p.account_id, _user.c_str(), 13);
	p.client_seq_id = genRequestID();

	ATPRetCodeType ec = _api->ReqFundQuery(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Query account failed: {}", ec);
	}
	return 0;
}

int TraderATP::queryPositions()
{
	ATPReqShareQueryMsg p;
	strncpy(p.cust_id, _cust_id.c_str(), 17);
	strncpy(p.fund_account_id, _accountid.c_str(), 17);
	strncpy(p.account_id, _user.c_str(), 13);
	p.client_seq_id = genRequestID();

	ATPRetCodeType ec = _api->ReqShareQuery(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Query positions failed: {}", ec);
	}

	return 0;
}

int TraderATP::queryOrders()
{
	ATPReqOrderQueryMsg p;
	strncpy(p.cust_id, _cust_id.c_str(), 17);
	strncpy(p.fund_account_id, _accountid.c_str(), 17);
	strncpy(p.account_id, _user.c_str(), 13);
	p.client_seq_id = genRequestID();
	p.business_abstract_type = 1;

	ATPRetCodeType ec = _api->ReqOrderQuery(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Query orders failed: {}", ec);
	}

	return 0;
}

int TraderATP::queryTrades()
{
	ATPReqTradeOrderQueryMsg p;
	strncpy(p.cust_id, _cust_id.c_str(), 17);
	strncpy(p.fund_account_id, _accountid.c_str(), 17);
	strncpy(p.account_id, _user.c_str(), 13);
	p.client_seq_id = genRequestID();
	p.business_abstract_type = 1;

	ATPRetCodeType ec = _api->ReqTradeOrderQuery(&p);
	if (ec != ErrorCode::kSuccess)
	{
		write_log(_sink, LL_ERROR, "[TraderXTP] Query trades failed: {}", ec);
	}

	return 0;
}
#pragma endregion "ITraderApi"