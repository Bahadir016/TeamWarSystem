//Search:

	if (rkInstDst.m_dwEmpireID == m_dwEmpireID)
		return TRUE;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	if ((strstr(CPythonBackground::Instance().GetWarpMapName(), "metin2_map_team_war")))
		return TRUE;
#endif

//Search:
		SetHair(c_rkCreateData.m_dwHair);

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		SetTeam(c_rkCreateData.m_bTeam);
#endif

//Search:
	m_dwEmoticonTime = 0;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	m_bTeam = 0;
#endif