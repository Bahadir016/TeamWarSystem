//Add in includes:
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
#endif

//Search:
m_iSyncHackCount = 0;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	m_bTeam = 0;
	m_pkSelectTeamEvent = nullptr;
#endif

//Search:
event_cancel(&m_pkMiningEvent);

//Add below:

#ifdef ENABLE_TEAM_WAR_SYSTEM
	event_cancel(&m_pkSelectTeamEvent);
#endif

//Search:

d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));

//Add above:

#ifdef ENABLE_TEAM_WAR_SYSTEM
		addPacket.bTeam = m_bTeam;
#endif

//Search:

	if (m_pWeddingMap)
	{
		SetWeddingMap(NULL);
	}

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(GetMapIndex()));
	if (pTeamWar)
		pTeamWar->DecMember(this);
#endif

//Search:
void CHARACTER::MountSummon(LPITEM mountItem)
{
	if (GetMapIndex() == 113)
		return;

//Add below:

#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(GetMapIndex()));
	if (pTeamWar)
		return;
#endif

//Search:
void CHARACTER::CheckMount()
{
	if (GetMapIndex() == 113)
		return;

//Add below:

#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto& rkTeamWarMgr = CTeamWarManager::Instance();
	auto pTeamWar = rkTeamWarMgr.Find(rkTeamWarMgr.TeamWarNo(GetMapIndex()));
	if (pTeamWar)
		return;
#endif

//at the end of the file
#ifdef ENABLE_TEAM_WAR_SYSTEM
EVENTFUNC(select_team_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("warp_npc_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == nullptr)
		return 0;

	ch->DestroySelectTeamEvent();
	ch->GoHome();

	return 0;
}

auto CHARACTER::StartSelectTeamEvent() -> void
{
	auto info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkSelectTeamEvent = event_create(select_team_event, info, PASSES_PER_SEC(10));
}

auto CHARACTER::DestroySelectTeamEvent() -> void
{
	event_cancel(&m_pkSelectTeamEvent);
}
#endif