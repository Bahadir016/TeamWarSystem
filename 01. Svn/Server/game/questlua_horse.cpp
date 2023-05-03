//Add in includes:
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//Search:

		bool bFromFar = lua_isboolean(L, 1) ? lua_toboolean(L, 1) : false;

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		auto& rkTeamWarMgr = CTeamWarManager::Instance();
		auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));
		if (pTeamWar)
			return 0;
#endif