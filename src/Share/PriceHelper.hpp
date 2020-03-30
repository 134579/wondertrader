/*!
 * \file PriceHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief �۸��ʽ��������
 */
#pragma once
#include "WTSMarcos.h"
#include <stdint.h>
#include <string>

namespace PriceHelper
{
	/*
	 *	˫����ת�ַ���
	 */
	static std::string toString(double val, uint32_t prec = 2)
	{
		char fmt[16] = { 0 };
		sprintf(fmt, "%%.%uf", prec);

		char buf[64] = { 0 };
		sprintf(buf, fmt, val);
		return buf;
	}

	/*
	 *	����ת�ַ���
	 */
	static std::string toString(int32_t val)
	{
		if (val == 0)
			return " - ";

		char buf[64] = { 0 };
		sprintf(buf, "%d", val);
		return buf;
	}

	/*
	 *	�޷�������ת�ַ���
	 */
	static std::string toString(uint32_t val)
	{
		if (val == 0)
			return " - ";

		char buf[64] = { 0 };
		sprintf(buf, "%u", val);
		return buf;
	}

	/*
	 *	64λ����ת�ַ���
	 */
	static std::string toString(int64_t val)
	{
		if (val == 0)
			return " - ";

		char buf[64] = { 0 };
		sprintf(buf, INT64_FMT, val);
		return buf;
	}

	/*
	 *	�޷���64λ����ת�ַ���
	 */
	static std::string toString(uint64_t val)
	{
		if (val == 0)
			return " - ";

		char buf[64] = { 0 };
		sprintf(buf, UINT64_FMT, val);
		return buf;
	}

	static std::string formatValue(uint32_t value, uint32_t prec = 2)
	{
		if (value == 0)
			return " - ";

		return toString(PRICE_INT_TO_DOUBLE(value), prec);
	}

	static std::string formatValue(uint64_t value, uint32_t prec = 2)
	{
		if (value == 0)
			return " - ";

		return toString(PRICE_INT_TO_DOUBLE(value), prec);
	}

	static std::string formatValue(int32_t value, uint32_t prec = 2)
	{
		if (value == 0)
			return " - ";

		return toString(PRICE_INT_TO_DOUBLE(value), prec);
	}

	static std::string formatValue(int64_t value, uint32_t prec = 2)
	{
		if (value == 0)
			return " - ";

		return toString(PRICE_INT_TO_DOUBLE(value), prec);
	}
};