//Search:
	m_stName = src.m_stName;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	m_bTeam = src.m_bTeam;
#endif

//Search:
	m_kAffectFlags.Clear();

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	m_bTeam = 0;
#endif

//Search:
	kCreateData.m_isMain = __IsMainActorVID(dwVID);

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	kCreateData.m_bTeam = rkNetActorData.m_bTeam;
#endif