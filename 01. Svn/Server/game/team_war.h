enum ETeamWarSettings
{
	TEAM_WAR_TEAM_BLUE,
	TEAM_WAR_TEAM_RED,
	TEAM_WAR_TEAM_MAX,
	TEAM_WAR_MAP_IDX = 114,
	TEAM_WAR_MAX_PLAYER_COUNT = 8,
	TEAM_WAR_TIME_OUT = 60*10,
	TEAM_WAR_END_TIME = 60*10,
	TEAM_WAR_P2P_PORT = 14199,
};

class CTeamWar
{
public:
	CTeamWar(DWORD dwTeamWarNo, bool bPrivate, long lMapIndex);
	virtual ~CTeamWar(void);

	typedef struct STeamWarPlayerData
	{
		char szName[CHARACTER_NAME_MAX_LEN + 1];
		BYTE bKill;
		BYTE bDead;
		int iDamage;
		BYTE bTeam;
	} TTeamWarPlayerData;

	typedef std::map<DWORD, TTeamWarPlayerData*> TeamWarPlayerDataMap;
	typedef std::set<std::string> TeamWarPlayerHddSerialNoSet;

	auto GetTeamWarPlayer(DWORD dwPID) -> TTeamWarPlayerData*;

	auto IncMember(LPCHARACTER ch) -> void;
	auto DecMember(LPCHARACTER ch) -> void;
	auto JoinTeam(LPCHARACTER ch, BYTE bTeam) -> bool;
	auto NewPlayerData(LPCHARACTER ch, BYTE bTeam) -> void;
	auto DeletePlayerData(DWORD dwPID) -> void;
	auto CheckEndTeamWar() -> void;

	auto OnKill(LPCHARACTER pkKiller, LPCHARACTER pkKilled) -> void;
	auto OnDamage(LPCHARACTER pkAttacker, int iDamage) -> void;

	auto SendPlayerDataPacket(LPCHARACTER ch) -> void;
	auto SendLoginPacket(LPCHARACTER ch) -> void;

	auto StartDestroyTeamWarEvent() -> void;

	auto Notice(const char* psz) -> void;
	auto GoHome() -> void;

	auto SendKillNotice(const char* c_szKillerName, const char* c_szVictimName, BYTE bKillerRace, BYTE bVictimRace) -> void;
	auto SendScorePacket() -> void;
	auto SendPlayerCountPacket() -> void;
	auto SendDurationTimePacket(LPCHARACTER ch = nullptr) -> void;

	auto Destroy() -> void;

	auto GetMapIndex() { return m_lMapIndex; }
	auto IsPrivateTeamWar() { return m_bPrivate; }
	auto GetTeamWarNo() { return m_dwTeamWarNo; }

	auto SecondToMin(int iSecond) { return (iSecond / 60); }

	auto GetBlueTeamPlayerCount() { return m_TeamData[TEAM_WAR_TEAM_BLUE].bMemberCount; }
	auto GetRedTeamPlayerCount() { return m_TeamData[TEAM_WAR_TEAM_RED].bMemberCount; }

	auto GetWinnerTeam() { return m_TeamData[TEAM_WAR_TEAM_BLUE].wScore > m_TeamData[TEAM_WAR_TEAM_RED].wScore ? true : false; }

	auto SetStartTeamWar(bool bSet) { m_bTeamStartWar = bSet; }

	auto SetEndTeamWar(bool bSet) { m_bEndTeamWar = bSet; }
	auto IsEndTeamWar() { return m_bEndTeamWar; }

	BYTE GetPlayerTotalCount() { return m_set_pkChr.size(); }

	auto SetEndTime(DWORD dwTime) { m_dwEndTime = dwTime; }

private:
	DWORD	m_dwTeamWarNo;
	bool	m_bPrivate;
	long	m_lMapIndex;
	LPEVENT	m_pkTeamWarEvent;
	bool	m_bTeamStartWar;
	bool	m_bEndTeamWar;
	LPEVENT m_pkDestroyTeamWar;
	DWORD	m_dwEndTime;
	TeamWarPlayerDataMap	m_TeamWarPlayerData;
	CHARACTER_SET m_set_pkChr;
	TeamWarPlayerHddSerialNoSet m_set_hddSerialNo;

	typedef struct STeamWarData
	{
		BYTE	bMemberCount;
		WORD	wScore;

		auto	Initialize() -> void;
	} TeamData;

	TeamData	m_TeamData[TEAM_WAR_TEAM_MAX];
};

class CTeamWarManager : public singleton<CTeamWarManager>
{
public:
	CTeamWarManager();
	virtual ~CTeamWarManager();

	typedef std::map<DWORD, CTeamWar*> TeamWar;

	auto TeamWarNo(long lMapIndex) -> DWORD;

	auto CreateTeamWarMap(bool bPrivate, DWORD dwPID) -> long;
	auto JoinTeamWar(DWORD dwTeamWarNo, DWORD dwPID, long &lMapIndex) -> bool;
	auto WarpTeamWar(DWORD dwPID, long lMapIndex) -> void;
	auto ChatTeamWar(DWORD dwPID, std::string strChat) -> void;
	auto SendTeamWarInfo(const std::string strName, DWORD dwTeamWarNo) -> void;
	auto SendTeamWarInfoPacket(const std::string strName, DWORD dwTeamWarNo) -> void;
	auto DestroyTeamWar(CTeamWar *pTeamWar) -> void;

	auto Find(DWORD dwTeamWarNo) -> CTeamWar*;

protected:
	TeamWar			m_TeamWar;
};