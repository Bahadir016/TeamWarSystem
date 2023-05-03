//Add in includes
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//Search:
	if (ch->IsPC() && victim->IsPC())
	{

//Add below:

#ifdef ENABLE_TEAM_WAR_SYSTEM
		else if (ch->GetMapIndex() >= 10000)
		{
			auto& rkTeamWarMgr = CTeamWarManager::Instance();
			auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));
			if (pTeamWar)
			{
				auto bTeam = ch->GetTeam() != victim->GetTeam();
				return bTeam;
			}
		}
#endif