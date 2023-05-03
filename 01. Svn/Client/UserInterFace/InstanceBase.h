//Search:

		bool	m_isMain;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		BYTE	m_bTeam;
#endif

//Search:
	void					SetRotationSpeed(float fRotSpd);

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	void					SetTeam(BYTE bTeam) { m_bTeam = bTeam; }
#endif

//Search:
	BYTE					GetPKMode();

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto					GetTeam() { return m_bTeam; }
#endif

//Search:
	struct SMoveAfterFunc
	{
		UINT eFunc;
		UINT uArg;

		// For Emotion Function
		UINT uArgExpanded;
		TPixelPosition kPosDst;
	};

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	BYTE					m_bTeam;
#endif