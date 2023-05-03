//Search:
		Set(HEADER_GC_DRAGON_SOUL_REFINE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDragonSoulRefine), STATIC_SIZE_PACKET));
//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		Set(HEADER_GC_TEAM_WAR,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTeamWar), DYNAMIC_SIZE_PACKET));
#endif