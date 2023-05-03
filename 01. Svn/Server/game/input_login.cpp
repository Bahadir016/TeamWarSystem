//Add in includes:
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//Search:
	if (ch->GetMapIndex() >= 10000)
	{

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		auto& rkTeamWarMgr = CTeamWarManager::Instance();
		auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));
		if (pTeamWar)
			pTeamWar->IncMember(ch);
#endif