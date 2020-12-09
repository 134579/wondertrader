#include "WtExecMgr.h"
#include "WtFilterMgr.h"

#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;

//////////////////////////////////////////////////////////////////////////
#pragma region "WtExecuterMgr"
void WtExecuterMgr::set_positions(std::unordered_map<std::string, double> target_pos)
{
	if(_filter_mgr != NULL)
	{
		std::unordered_map<std::string, double> des_port;
		for(auto& m : target_pos)
		{
			const std::string& stdCode = m.first;
			double& desVol = m.second;
			double oldVol = desVol;

			bool isFltd = _filter_mgr->is_filtered_by_code(stdCode.c_str(), desVol);
			if (!isFltd)
			{
				if (!decimal::eq(desVol, oldVol))
				{
					//�����־
					WTSLogger::info(fmt::format("[������] ���{}��Ŀ���λ���������������: {} -> {}", stdCode.c_str(), oldVol, desVol).c_str());
				}

				des_port[stdCode] = desVol;
			}
			else
			{
				//�����־
				WTSLogger::info("[������] ���%s��Ŀ���λ�����Թ���������", stdCode.c_str());
			}
		}

		des_port.swap(target_pos);
	}

	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		executer->set_position(target_pos);
	}
}

void WtExecuterMgr::handle_pos_change(const char* stdCode, double targetPos)
{
	if(_filter_mgr != NULL)
	{
		double oldVol = targetPos;
		bool isFltd = _filter_mgr->is_filtered_by_code(stdCode, targetPos);
		if (!isFltd)
		{
			if (!decimal::eq(targetPos, oldVol))
			{
				//�����־
				WTSLogger::info(fmt::format("[������] ���{}��Ŀ���λ���������������: {} -> {}", stdCode, oldVol, targetPos).c_str());
			}
		}
		else
		{
			//�����־
			WTSLogger::info("[������] ���%s��Ŀ���λ�����Թ���������", stdCode);
			return;
		}
	}
	
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		executer->on_position_changed(stdCode, targetPos);
	}
}

void WtExecuterMgr::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		executer->on_tick(stdCode, curTick);
	}
}
#pragma endregion "WtExecuterMgr"
