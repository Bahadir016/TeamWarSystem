//Add in includes
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//Search:
CGuildManager::instance().Kill(pkKiller, this);

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
			auto& rkTeamWarMgr = CTeamWarManager::Instance();
			auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(GetMapIndex()));
			if (pTeamWar && !pTeamWar->IsEndTeamWar())
			{
				pTeamWar->OnKill(pkKiller, this);
				pTeamWar->SendKillNotice(pkKiller->GetName(), GetName(), pkKiller->GetRaceNum(), GetRaceNum());
			}
#endif

//Search:
if (!isAgreedPVP && !isUnderGuildWar && !IsKillerMode() && GetAlignment() >= 0 && !isDuel)

//Replace with:

#ifdef ENABLE_TEAM_WAR_SYSTEM
				auto& rkTeamWarMgr = CTeamWarManager::Instance();
				auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(pkKiller->GetMapIndex()));
				if (!isAgreedPVP && !isUnderGuildWar && !IsKillerMode() && GetAlignment() >= 0 && !isDuel && !pTeamWar)
#else
				if (!isAgreedPVP && !isUnderGuildWar && !IsKillerMode() && GetAlignment() >= 0 && !isDuel)
#endif

//Search:
PointChange(POINT_HP, -dam, false);

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		auto& rkTeamWarMgr = CTeamWarManager::Instance();
		auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(GetMapIndex()));
		if (pTeamWar)
			pTeamWar->OnDamage(pAttacker, dam);
#endif
