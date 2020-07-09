/*!
 * \file CodeHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ���븨���࣬��װ��һ�𷽱�ʹ��
 */
#pragma once
#include "StrUtil.hpp"
#include "../Includes/WTSTypes.h"

#include <boost/xpressive/xpressive_dynamic.hpp>

USING_NS_OTP;

class CodeHelper
{
public:
	typedef struct _CodeInfo
	{
		char _code[MAX_INSTRUMENT_LENGTH];		//��Լ����
		char _exchg[MAX_INSTRUMENT_LENGTH];		//����������
		char _product[MAX_INSTRUMENT_LENGTH];	//Ʒ�ִ���

		ContractCategory	_category;		//��Լ����
		union
		{
			bool	_hot;		//�Ƿ���������Լ
			bool	_exright;	//�Ƿ��Ǹ�Ȩ���룬��SH600000Q
		};

		_CodeInfo()
		{
			memset(this, 0, sizeof(_CodeInfo));
			_category = CC_Future;
		}
	} CodeInfo;

public:
	static bool	isStdStkCode(const char* code)
	{
		using namespace boost::xpressive;
		/* ����������ʽ */
		cregex reg_stk = cregex::compile("^[A-Z]+.([A-Z]+.)?\\d{6}Q?$");
		return 	regex_match(code, reg_stk);
	}

	static std::string stdCodeToStdCommID(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
			return stdStkCodeToStdCommID(stdCode);
		else
			return stdFutCodeToStdCommID(stdCode);
	}

	static std::string stdStkCodeToStdCommID(const char* stdCode)
	{
		//�����SSE.600000��ʽ�ģ�Ĭ��ΪSTKƷ��
		//�����SSE.STK.600000��ʽ�ģ��ͽ���Ʒ�ֳ���
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string str = ay[0];
		str += ".";
		if (ay.size() == 2)
			str += "STK";
		else
			str += ay[1];
		return str;
	}

	/*
	 *	�ӻ�����Լ������ȡ����Ʒ�ִ���
	 *	��ag1912 -> ag
	 */
	static std::string bscFutCodeToBscCommID(const char* code)
	{
		std::string strRet;
		int nLen = 0;
		while ('A' <= code[nLen] && code[nLen] <= 'z')
		{
			strRet += code[nLen];
			nLen++;
		}

		return strRet;
	}

	/*
	 *	��׼��Լ����ת��׼Ʒ�ִ���
	 *	��SHFE.ag.1912 -> SHFE.ag
	 */
	static std::string stdFutCodeToStdCommID(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string str = ay[0];
		str += ".";
		str += ay[1];
		return str;
	}

	/*
	 *	������Լ����ת��׼��
	 *	��ag1912ת��ȫ��
	 */
	static std::string bscFutCodeToStdCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = bscFutCodeToBscCommID(code);

		std::string ret = StrUtil::printf("%s.%s", exchg, pid.c_str());
		if (!isComm)
		{
			ret += ".";

			char* s = (char*)code;
			s += pid.size();
			if(strlen(s) == 4)
			{
				ret += s;
			}
			else
			{
				if (s[0] == '9')
					ret += "1";
				else
					ret += "2";

				ret += s;
			}
		}
		return ret;
	}

	static std::string bscStkCodeToStdCode(const char* code, const char* exchg)
	{
		return StrUtil::printf("%s.%s", exchg, code);
	}

	static bool	isStdFutOptCode(const char* code)
	{
		using namespace boost::xpressive;
		/* ����������ʽ */
		cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+\\d{4}.(C|P).\\d+$");	//CFFEX.IO2007.C.4000
		return 	regex_match(code, reg_stk);
	}


	/*
	 *	�ڻ���Ȩ�����׼��
	 */
	static std::string bscFutOptCodeToStdCode(const char* code, const char* exchg)
	{
		using namespace boost::xpressive;
		/* ����������ʽ */
		cregex reg_stk = cregex::compile("^[A-Z]+\\d{4}-(C|P)-\\d+$");	//�н�������������ʽIO2013-C-4000
		bool bMatch = regex_match(code, reg_stk);
		if(bMatch)
		{
			std::string s = StrUtil::printf("%s.%s", exchg, code);
			StrUtil::replace(s, "-", ".");
			return s;
		}
		else
		{
			//֣������������Ȩ�����ʽZC2010P11600

			//�ȴӺ���ǰ��λ��P��C��λ��
			int idx = strlen(code) - 1;
			for(; idx >= 0; idx--)
			{
				if(!isdigit(code[idx]))
					break;
			}
			
			std::string s = exchg;
			s.append(".");
			s.append(code, idx);
			s.append(".");
			s.append(&code[idx], 1);
			s.append(".");
			s.append(&code[idx + 1]);
			return s;
		}
	}

	/*
	 *	ͨ��Ʒ�ִ����ȡ������Լ����
	 */
	static std::string bscCodeToStdHotCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = bscFutCodeToBscCommID(code);

		std::string ret = StrUtil::printf("%s.%s.HOT", exchg, pid.c_str());
		return ret;
	}

	static std::string stdCodeToStdHotCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string stdHotCode = ay[0];
		stdHotCode += ".";
		stdHotCode += ay[1];
		stdHotCode += ".HOT";
		return stdHotCode;
	}

	static std::string stdFutOptCodeToBscCode(const char* stdCode)
	{
		std::string ret = stdCode;
		auto pos = ret.find(".");
		ret = ret.substr(pos + 1);
		if (strncmp(stdCode, "CFFEX", 5) == 0 || strncmp(stdCode, "DCE", 3) == 0)
			StrUtil::replace(ret, ".", "-");
		else
			StrUtil::replace(ret, ".", "");
		return ret;
	}

	static std::string stdFutCodeToBscCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string exchg = ay[0];
		std::string bscCode = ay[1];
		if (exchg.compare("CZCE") == 0 && ay[2].size() == 4)
			bscCode += ay[2].substr(1);
		else
			bscCode += ay[2];
		return bscCode;
	}

	static std::string stdStkCodeToBscCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string exchg = ay[0];
		std::string bscCode;
		if (exchg.compare("SSE") == 0)
			bscCode = "SH";
		else
			bscCode = "SZ";

		if (ay.size() == 2)
			bscCode += ay[1];
		else
			bscCode += ay[2];
		return bscCode;
	}

	static std::string stdCodeToBscCode(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
			return stdStkCodeToBscCode(stdCode);
		else if (isStdFutOptCode(stdCode))
			return stdFutOptCodeToBscCode(stdCode);
		else
			return stdFutCodeToBscCode(stdCode);
	}

	static void extractStdFutCode(const char* stdCode, CodeInfo& codeInfo)
	{
		codeInfo._hot = StrUtil::endsWith(stdCode, ".HOT", false);
		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
		strcpy(codeInfo._code, ay[1].c_str());
		codeInfo._category = CC_Future;
		if (!codeInfo._hot)
		{
			if (strcmp(codeInfo._exchg, "CZCE") == 0 && ay[2].size() == 4)
			{
				//bscCode += ay[2].substr(1);
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].substr(1).c_str());
			}
			else
			{
				//bscCode += ay[2];
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].c_str());
			}
		}
		//commID = ay[1];
		strcpy(codeInfo._product, ay[1].c_str());
	}

	static void extractStdStkCode(const char* stdCode, CodeInfo& codeInfo)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		codeInfo._category = CC_Stock;
		//exchg = ay[0];
		strcpy(codeInfo._exchg, ay[0].c_str());
		if (ay.size() > 2)
		{
			//commID = ay[1];
			strcpy(codeInfo._product, ay[1].c_str());
			//bscCode = ay[2];
			if (ay[2].back() == 'Q')
			{
				strcpy(codeInfo._code, ay[2].substr(0, ay[2].size() - 1).c_str());
				codeInfo._exright = true;
			}
			else
			{
				strcpy(codeInfo._code, ay[2].c_str());
				codeInfo._exright = false;
			}
		}
		else
		{
			//commID = "STK";
			bool isSH = strcmp(codeInfo._exchg, "SSE") == 0;
			bool isIdx = (isSH && ay[1][0] == '0') || (!isSH && strncmp(ay[1].c_str(), "39", 2) == 0);//�Ƿ���ָ�����Ͻ�����ָ����'0'��ͷ�������'39'��ͷ
			strcpy(codeInfo._product, isIdx ? "IDX" : "STK");
			//bscCode = ay[1];
			if (ay[1].back() == 'Q')
			{
				strcpy(codeInfo._code, ay[1].substr(0, ay[1].size() - 1).c_str());
				codeInfo._exright = true;
			}
			else
			{
				strcpy(codeInfo._code, ay[1].c_str());
				codeInfo._exright = false;
			}
		}
	}


	static int indexCodeMonth(const char* code)
	{
		if (strlen(code) == 0)
			return -1;

		int idx = 0;
		int len = strlen(code);
		while(idx < len)
		{
			if (isdigit(code[idx]))
				return idx;

			idx++;
		}
		return -1;
	}

	static void extractStdFutOptCode(const char* stdCode, CodeInfo& codeInfo)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
		codeInfo._category = CC_Option;
		if(strcmp(codeInfo._exchg, "SHFE") == 0 || strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			sprintf(codeInfo._code, "%s%s%s", ay[1].c_str(), ay[2].c_str(), ay[3].c_str());
		}
		else
		{
			sprintf(codeInfo._code, "%s-%s-%s", ay[1].c_str(), ay[2].c_str(), ay[3].c_str());
		}

		int mpos = indexCodeMonth(ay[1].c_str());

		if(strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			strncpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, ay[2].c_str());
		}
		else
		{
			strncpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, "_o");
		}
	}

	static void extractStdCode(const char* stdCode, CodeInfo& codeInfo)
	{
		if (isStdStkCode(stdCode))
		{
			extractStdStkCode(stdCode, codeInfo);
		}
		else if(isStdFutOptCode(stdCode))
		{
			extractStdFutOptCode(stdCode, codeInfo);
		}
		else
		{
			extractStdFutCode(stdCode, codeInfo);
		}
	}

	static bool	isStdFutHotCode(const char* stdCode)
	{
		return StrUtil::endsWith(stdCode, ".HOT", false);
	}
};

