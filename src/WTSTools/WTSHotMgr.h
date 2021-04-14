/*!
 * \file WTSHotMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ������Լ������ʵ��
 */
#pragma once
#include "../Includes/IHotMgr.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"
#include <string>

NS_OTP_BEGIN
	class IMarketMgr;
	class WTSHotItem;
NS_OTP_END

USING_NS_OTP;

//��������ӳ��
typedef WTSMap<uint32_t>		WTSDateHotMap;
//Ʒ������ӳ��
typedef WTSMap<std::string>		WTSProductHotMap;
//���г�����ӳ��
typedef WTSMap<std::string>		WTSExchgHotMap;

class WTSHotMgr : public IHotMgr
{
public:
	WTSHotMgr();
	~WTSHotMgr();

public:
	bool loadHots(const char* filename);
	void release();

	void getHotCodes(const char* exchg, std::map<std::string, std::string> &mapHots);
	bool hasHotCodes();

	inline bool isInitialized() const {return m_bInitialized;}

public:
	virtual const char* getRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getHotCode(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool	isHot(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool	splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

private:
	WTSExchgHotMap*	m_pExchgHotMap;
	faster_hashmap<std::string, std::string>	m_curHotCodes;
	bool			m_bInitialized;
};

