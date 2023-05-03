//Search:
	void			GetP2PHostNames(std::string& hostNames);

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto			SendTeamWar(const void *c_pvData, int iSize) -> void;
#endif