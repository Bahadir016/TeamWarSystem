//Search:
kNetActorData.m_dwLevel = 0;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	kNetActorData.m_bTeam = 0;
#endif

//Search:
		__RecvCharacterAppendPacket(&kNetActorData);

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		kNetActorData.m_bTeam = chrInfoPacket.bTeam;
#endif