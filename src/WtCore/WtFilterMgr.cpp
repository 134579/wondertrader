#include "WtFilterMgr.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/BoostFile.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
namespace rj = rapidjson;

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif

USING_NS_OTP;

void WtFilterMgr::load_filters(const char* fileName)
{
	if (_filter_file.empty() && strlen(fileName) == 0)
		return;

	_filter_file = fileName;

	if (!StdFile::exists(_filter_file.c_str()))
	{
		WTSLogger::error("�����������ļ�%s������", _filter_file.c_str());
		return;
	}

	uint64_t lastModTime = boost::filesystem::last_write_time(boost::filesystem::path(_filter_file));
	if (lastModTime <= _filter_timestamp)
		return;

	if(_filter_timestamp != 0)
		WTSLogger::info("�����������ļ�%s���޸�, ��Ҫ���¼���", _filter_file.c_str());

	std::string content;
	StdFile::read_file_content(_filter_file.c_str(), content);
	if (content.empty())
	{
		WTSLogger::error("�����������ļ�%sΪ��", _filter_file.c_str());
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		WTSLogger::error("�����������ļ�%s����ʧ��", _filter_file.c_str());
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		WTSLogger::error("�����������ļ�%sת��ʧ��", _filter_file.c_str());
		return;
	}

	_filter_timestamp = lastModTime;

	_stra_filters.clear();
	_exec_filters.clear();

	//�����Թ�����
	WTSVariant* filterStra = cfg->get("strategy_filters");
	if (filterStra)
	{
		auto keys = filterStra->memberNames();
		for (const std::string& key : keys)
		{
			WTSVariant* cfgItem = filterStra->get(key.c_str());
			const char* action = cfgItem->getCString("action");
			FilterAction fAct = FA_None;
			if (my_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (my_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("���Թ�����%s����%s����ʶ��", key.c_str(), action);
				continue;
			}

			FilterItem& fItem = _stra_filters[key];
			fItem._key = key;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("���Թ�����%s�Ѽ���", key.c_str());
		}
	}

	//��ִ����������
	WTSVariant* filterCodes = cfg->get("code_filters");
	if (filterCodes)
	{
		auto codes = filterCodes->memberNames();
		for (const std::string& stdCode : codes)
		{

			WTSVariant* cfgItem = filterCodes->get(stdCode.c_str());
			const char* action = cfgItem->getCString("action");
			FilterAction fAct = FA_None;
			if (my_stricmp(action, "ignore") == 0)
				fAct = FA_Ignore;
			else if (my_stricmp(action, "redirect") == 0)
				fAct = FA_Redirect;

			if (fAct == FA_None)
			{
				WTSLogger::error("���������%s����%s����ʶ��", stdCode.c_str(), action);
				continue;
			}

			FilterItem& fItem = _exec_filters[stdCode];
			fItem._key = stdCode;
			fItem._action = fAct;
			fItem._target = cfgItem->getDouble("target");

			WTSLogger::info("���������%s�Ѽ���", stdCode.c_str());
		}
	}

	cfg->release();
}

const char* FLTACT_NAMEs[] =
{
	"����",
	"�ض���"
};

bool WtFilterMgr::is_filtered_by_strategy(const char* straName, double& targetPos, bool isDiff /* = false */)
{
	auto it = _stra_filters.find(straName);
	if (it != _stra_filters.end())
	{
		const FilterItem& fItem = it->second;
		WTSLogger::info("[������] ���Թ�����%s����, ���˲���: %s", straName, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "δ֪");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect && !isDiff)
		{
			//ֻ�в���������ʱ�򣬲���Ч
			targetPos = fItem._target;
		}

		return false;
	}

	return false;
}

bool WtFilterMgr::is_filtered_by_code(const char* stdCode, double& targetPos)
{
	auto cit = _exec_filters.find(stdCode);
	if (cit != _exec_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[������] ���������%s����, ���˲���: %s", stdCode, fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "δ֪");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect)
		{
			targetPos = fItem._target;
		}

		return false;
	}

	std::string stdPID = CodeHelper::stdCodeToStdCommID(stdCode);
	cit = _exec_filters.find(stdPID);
	if (cit != _exec_filters.end())
	{
		const FilterItem& fItem = cit->second;
		WTSLogger::info("[������] Ʒ�ֹ�����%s����, ���˲���: %s", stdPID.c_str(), fItem._action <= FA_Redirect ? FLTACT_NAMEs[fItem._action] : "δ֪");
		if (fItem._action == FA_Ignore)
		{
			return true;
		}
		else if (fItem._action == FA_Redirect)
		{
			targetPos = fItem._target;
		}

		return false;
	}

	return false;
}

