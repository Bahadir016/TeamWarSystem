//add in includes
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//At the end of the file:
#ifdef ENABLE_TEAM_WAR_SYSTEM
ACMD(do_start_team_war)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));

	if (pTeamWar)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Takým Savaþý> Takým Savaþýndayken baþak bir Takým Savaþý oluþturamazsýn.");
		return;
	}

	BYTE bPrivateTeamWar = 0;
	if (*arg1)
		str_to_number(bPrivateTeamWar, arg1);

	TPacketGGTeamWar p2p_packet;
	p2p_packet.bHeader = HEADER_GG_TEAM_WAR;
	p2p_packet.bSubHeader = TEAM_WAR_SUBHEADER_GG_CREATE;
	p2p_packet.dwPID = ch->GetPlayerID();

	TEMP_BUFFER buf;
	buf.write(&p2p_packet, sizeof(p2p_packet));
	buf.write(&bPrivateTeamWar, sizeof(bPrivateTeamWar));

	//Send to P2P
	P2P_MANAGER::Instance().SendTeamWar(buf.read_peek(), buf.size());
}

ACMD(do_join_team_war)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));

	if (pTeamWar)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Takým Savaþý> Takým Savaþýndayken baþka bir Takým Savaþýna katýlamazýn.");
		return;
	}

	DWORD dwTeamWarNo = 0;
	if (*arg1)
		str_to_number(dwTeamWarNo, arg1);

	TPacketGGTeamWar p2p_packet;
	p2p_packet.bHeader = HEADER_GG_TEAM_WAR;
	p2p_packet.bSubHeader = TEAM_WAR_SUBHEADER_GG_JOIN;
	p2p_packet.dwPID = ch->GetPlayerID();

	TEMP_BUFFER buf;
	buf.write(&p2p_packet, sizeof(p2p_packet));
	buf.write(&dwTeamWarNo, sizeof(dwTeamWarNo));

	//Send to P2P
	P2P_MANAGER::Instance().SendTeamWar(buf.read_peek(), buf.size());

}

ACMD(do_select_team)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));

	BYTE bTeam = 0;
	if(*arg1)
		str_to_number(bTeam, arg1);

	if (pTeamWar && pTeamWar->JoinTeam(ch, bTeam))
	{
		ch->DestroySelectTeamEvent();
		ch->SetTeam(bTeam);
		ch->Show(ch->GetMapIndex(), bTeam == TEAM_WAR_TEAM_BLUE ? 2809800 : 2822700, 236600);
		ch->Stop();
	}
}

ACMD(do_team_war_load_player_data)
{
	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(ch->GetMapIndex()));

	if (pTeamWar)
		pTeamWar->SendPlayerDataPacket(ch);
}
#endif