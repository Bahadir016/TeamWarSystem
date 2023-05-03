//Search:
	//HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS = 152,
	HEADER_GC_HYBRIDCRYPT_SDB = 153, // SDB means Supplmentary Data Blocks
	//HYBRID CRYPT

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	HEADER_GC_TEAM_WAR = 159,
#endif

//Search:
	HEADER_GG_CHECK_AWAKENESS = 29,

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	HEADER_GG_TEAM_WAR = 32,
#endif

//Search:
} TPacketGCCharacterAdditionalInfo;

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	BYTE	bTeam;
#endif

//Search:
#pragma pack()
#endif

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
enum ETeamWarGGSubHeader
{
	TEAM_WAR_SUBHEADER_GG_CREATE,
	TEAM_WAR_SUBHEADER_GG_WARP,
	TEAM_WAR_SUBHEADER_GG_JOIN,
	TEAM_WAR_SUBHEADER_GG_CHAT,
	TEAM_WAR_SUBHEADER_GG_INFO
};

enum ETeamWarGCSubHeader
{
	TEAM_WAR_SUBHEADER_GC_LOGIN,
	TEAM_WAR_SUBHEADER_GC_DURATION_TIME,
	TEAM_WAR_SUBHEADER_GC_SCORE,
	TEAM_WAR_SUBHEADER_GC_PLAYER_COUNT,
	TEAM_WAR_SUBHEADER_GC_PLAYER_DATA,
	TEAM_WAR_SUBHEADER_GC_INFO
};

typedef struct SPacketGGTeamWar
{
	BYTE bHeader;
	BYTE bSubHeader;
	DWORD dwPID;
} TPacketGGTeamWar;

typedef struct SPacketGCTeamWar
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCTeamWar;

typedef struct SPacketGCTeamWarScore
{
	WORD wTeamBlueScore;
	WORD wTeamRedScore;
} TPacketGCTeamWarScore;

typedef struct SPacketGCTeamWarPlayerCount
{
	BYTE bTeamBluePlayerCount;
	BYTE bTeamRedPlayerCount;
} TPacketGCTeamWarPlayerCount;

typedef struct SPacketGCTeamWarPlayerData
{
	BYTE	bList;
	BYTE	bRace;
	BYTE	bLevel;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE	bKill;
	BYTE	bDead;
	int		iDamage;
} TPacketGCTeamWarPlayerData;

typedef struct SPacketGCTeamWarInfo
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD dwTeamWarNo;
} TPacketGCTeamWarInfo;
#endif