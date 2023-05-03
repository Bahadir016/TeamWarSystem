//Search:
	DWORD dwGuildIDSrc = GetGuildID();
	DWORD dwGuildIDDst = rkInstSel.GetGuildID();

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	BYTE dwTeamSrc = GetTeam();
	BYTE dwTeamDst = rkInstSel.GetTeam();
#endif

//Search:
	return __FindPVPKey(dwVIDSrc, dwVIDDst) || __FindGVGKey(dwGuildIDSrc, dwGuildIDDst);

//Replace with:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	return __FindPVPKey(dwVIDSrc, dwVIDDst) || __FindGVGKey(dwGuildIDSrc, dwGuildIDDst) || (dwTeamSrc != dwTeamDst);
#else
	return __FindPVPKey(dwVIDSrc, dwVIDDst) || __FindGVGKey(dwGuildIDSrc, dwGuildIDDst);
#endif

//Search:
				DWORD dwGuildIDMain = pkInstMain->GetGuildID();
				DWORD dwGuildIDSelf = GetGuildID();
				if (__FindGVGKey(dwGuildIDMain, dwGuildIDSelf))
				{
					return NAMECOLOR_PVP;
				}

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
				BYTE bTeamMain = pkInstMain->GetTeam();
				BYTE bTeamSelf = GetTeam();
				if (bTeamMain != bTeamSelf)
					return NAMECOLOR_PVP;
#endif