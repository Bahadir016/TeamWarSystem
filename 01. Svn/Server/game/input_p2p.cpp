//Search:
int CInputP2P::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
auto CInputP2P::TeamWar(const char *c_pData, size_t uiBytes) -> int
{
	auto p = reinterpret_cast<const TPacketGGTeamWar*>(c_pData);
	uiBytes -= sizeof(TPacketGGTeamWar);
	c_pData += sizeof(TPacketGGTeamWar);
	auto& rkTeamWarMgr = CTeamWarManager::Instance();

	switch (p->bSubHeader)
	{
		case TEAM_WAR_SUBHEADER_GG_CREATE:
		{
			if (uiBytes < sizeof(BYTE))
				return -1;

			auto bPrivateTeamWar = *reinterpret_cast<const BYTE*>(c_pData);
			auto lMapIndex = rkTeamWarMgr.CreateTeamWarMap(bPrivateTeamWar, p->dwPID);
			
			if (lMapIndex)
				rkTeamWarMgr.WarpTeamWar(p->dwPID, lMapIndex);
		}
		break;

		case TEAM_WAR_SUBHEADER_GG_WARP:
		{
			if (uiBytes < sizeof(long))
				return -1;
			
			auto ch = CHARACTER_MANAGER::Instance().FindByPID(p->dwPID);
			if (ch)
			{
				auto lMapIndex = *reinterpret_cast<const long*>(c_pData);
				ch->WarpSet(2815900, 228300, lMapIndex);
			}
		}
		break;

		case TEAM_WAR_SUBHEADER_GG_JOIN:
		{
			if (uiBytes < sizeof(DWORD))
				return -1;
			
			auto dwTeamWarNo = *reinterpret_cast<const DWORD*>(c_pData);
			long lMapIndex;

			if (rkTeamWarMgr.JoinTeamWar(dwTeamWarNo, p->dwPID, lMapIndex))
				rkTeamWarMgr.WarpTeamWar(p->dwPID, lMapIndex);
		}
		break;

		case TEAM_WAR_SUBHEADER_GG_CHAT:
		{
			auto ch = CHARACTER_MANAGER::Instance().FindByPID(p->dwPID);
			if (ch)
			{
				auto strChat = std::string(c_pData, uiBytes);
				ch->ChatPacket(CHAT_TYPE_INFO, strChat.c_str());
			}
		}
		break;

		case TEAM_WAR_SUBHEADER_GG_INFO:
		{	
			auto strName = std::string(c_pData, uiBytes);
			rkTeamWarMgr.SendTeamWarInfoPacket(strName, p->dwPID);
		}
		break;

		default:
			return -1;
	}

	return uiBytes;
}
#endif

//Search:
	case HEADER_GG_CHECK_AWAKENESS:
		IamAwake(d, c_pData);
		break;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	case HEADER_GG_TEAM_WAR:
		if ((iExtraLen = TeamWar(c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;
#endif