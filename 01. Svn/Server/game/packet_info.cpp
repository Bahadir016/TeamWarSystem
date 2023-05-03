//Search:
	Set(HEADER_GG_CHECK_AWAKENESS, sizeof(TPacketGGCheckAwakeness), "CheckAwakeness");

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	Set(HEADER_GG_TEAM_WAR, sizeof(TPacketGGTeamWar), "TeamWar");
#endif
