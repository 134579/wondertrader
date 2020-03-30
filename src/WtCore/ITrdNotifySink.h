#pragma once
#include <stdint.h>
#include "../Share/WTSMarcos.h"

NS_OTP_BEGIN

class ITrdNotifySink
{
public:
	/*
	 *	�ɽ��ر�
	 */
	virtual void on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price) = 0;

	/*
	 *	�����ر�
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled = false) = 0;

	/*
	 *	�ֲָ��»ص�
	 */
	virtual void on_position(const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail) = 0;

	/*
	 *	����ͨ������
	 */
	virtual void on_channel_ready() = 0;

	/*
	 *	����ͨ����ʧ
	 */
	virtual void on_channel_lost() = 0;

	/*
	 *	�µ��ر�
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message){}
};

NS_OTP_END