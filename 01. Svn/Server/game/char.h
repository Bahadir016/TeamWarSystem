//Search:
};

ESex GET_SEX(LPCHARACTER ch);

//Below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
public:
	auto StartSelectTeamEvent() -> void;
	auto DestroySelectTeamEvent() -> void;
	auto SetTeam(BYTE bTeam) { m_bTeam = bTeam; }
	auto GetTeam() { return m_bTeam; }
private:
	BYTE m_bTeam;
	LPEVENT m_pkSelectTeamEvent;
#endif