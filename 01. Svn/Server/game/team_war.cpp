//Author: Kio

#include "stdafx.h"
#include "char.h"
#include "char_manager.h"
#include "sectree_manager.h"
#include "config.h"
#include "p2p.h"
#include "buffer_manager.h"
#include "desc.h"
#include "desc_manager.h"
#include "desc_client.h"
#include "utils.h"
#include "party.h"
#include "regen.h"
#include "team_war.h"

EVENTINFO(team_war_map_info)
{
	CTeamWar* pTeamWar;
	BYTE bState;
	DWORD dwTimeOut;
	DWORD dwStartWaitTime;

	team_war_map_info() : pTeamWar(nullptr), bState(0), dwTimeOut(0), dwStartWaitTime(0) {}
};

EVENTFUNC(team_war_event)
{
	team_war_map_info* info = dynamic_cast<team_war_map_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("team_war_map_info> <Factor> Null pointer");
		return 0;
	}

	auto pTeamWar = info->pTeamWar;
	if (info->bState == 0)
	{
		DWORD dwGlobalTime = get_global_time();
		if (dwGlobalTime <= info->dwTimeOut)
		{
			char szNotice[128];
			auto dwRemainingTime = info->dwTimeOut - dwGlobalTime;
			if (dwRemainingTime != 0 && dwRemainingTime % 60 == 0)
			{
				snprintf(szNotice, sizeof(szNotice), "<Takým Savaþý> Takýmlar %d dakika içerisinde dolmazsa savaþ otomatik olarak iptal edilecektir.", pTeamWar->SecondToMin(dwRemainingTime));
				pTeamWar->Notice(szNotice);
				pTeamWar->Notice("<Takým Savaþý> Takýmlar dolduðunda savaþ baþlayacaktýr.");
			}
		}
		else
		{
			if (!pTeamWar->IsEndTeamWar())
			{
				pTeamWar->Notice("<Takým Savaþý> Takýmlar dolmadýðý için savaþ iptal edilmiþtir. 10 saniye sonra savaþ alaný temizlenecektir.");
				pTeamWar->SetEndTeamWar(true);
				pTeamWar->StartDestroyTeamWarEvent();
			}
			return 0;
		}

		if (pTeamWar->GetBlueTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT && pTeamWar->GetRedTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT)
		{
			info->bState++;
			info->dwStartWaitTime = get_global_time() + 16;
		}
	}
	else if (info->bState == 1)
	{
		DWORD dwGlobalTime = get_global_time();
		if (dwGlobalTime > info->dwStartWaitTime)
		{
			info->bState++;
			pTeamWar->SetStartTeamWar(true);
			pTeamWar->Notice("<Takým Savaþý> Savaþ baþladý! 15 dakika sonra bitecektir.");
			SECTREE_MANAGER::Instance().PurgeNPCsInMap(pTeamWar->GetMapIndex());
			pTeamWar->SetEndTime(dwGlobalTime + TEAM_WAR_END_TIME);
			pTeamWar->SendDurationTimePacket();

			return PASSES_PER_SEC(TEAM_WAR_END_TIME);
		}
		else
		{
			char szNotice[64];
			auto dwRemainingTime = info->dwStartWaitTime - dwGlobalTime;
			if (dwRemainingTime != 0)
			{
				snprintf(szNotice, sizeof(szNotice), "<Takým Savaþý> Savaþ %d saniye sonra baþlayacak.", dwRemainingTime);
				pTeamWar->Notice(szNotice);
			}
		}
	}
	else if (info->bState == 2)
	{
		if (!pTeamWar->IsEndTeamWar())
		{
			pTeamWar->Notice("<Takým Savaþý> Savaþ süresi dolmuþtur. 10 saniye sonra savaþ alaný temizlenecektir.");
			pTeamWar->SetEndTeamWar(true);
			pTeamWar->StartDestroyTeamWarEvent();
		}

		std::vector<const char*> teams = { "|cff0000ff[Mavi]|r", "|cffff0000[Kýrmýzý]|r" };
		char szNotice[128];
		snprintf(szNotice, sizeof(szNotice), "<Takým Savaþý> %d numaralý Takým Savaþýnýn kazananý %s takým olmuþtur.", pTeamWar->GetTeamWarNo(), teams[pTeamWar->GetWinnerTeam() ? TEAM_WAR_TEAM_BLUE : TEAM_WAR_TEAM_RED]);
		BroadcastNotice(szNotice);
		return 0;
	}

	return PASSES_PER_SEC(1);
}

EVENTFUNC(team_war_destroy_event)
{
	team_war_map_info* info = dynamic_cast<team_war_map_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("team_war_map_info> <Factor> Null pointer");
		return 0;
	}

	auto pTeamWar = info->pTeamWar;
	if (pTeamWar->GetPlayerTotalCount() <= 0)
	{
		pTeamWar->Destroy();
		CTeamWarManager::Instance().DestroyTeamWar(pTeamWar);
		return 0;
	}
	else
		pTeamWar->GoHome();

	return PASSES_PER_SEC(5);
}

auto CTeamWar::STeamWarData::Initialize() -> void
{
	bMemberCount = 0;
	wScore = 0;
}

CTeamWar::CTeamWar(DWORD dwTeamWarNo, bool bPrivate, long lMapIndex)
{
	m_dwTeamWarNo = dwTeamWarNo;
	m_bPrivate = bPrivate;
	m_lMapIndex = lMapIndex;

	for (auto &team : m_TeamData)
		team.Initialize();

	auto info = AllocEventInfo<team_war_map_info>();
	info->pTeamWar = this;
	info->dwTimeOut = get_global_time() + TEAM_WAR_TIME_OUT + 5;

	m_pkTeamWarEvent = event_create(team_war_event, info, 1);

	m_bTeamStartWar = false;
	m_bEndTeamWar = false;

	auto szFilename = "locale/turkey/map/metin2_map_team_war/cit.txt";
	auto pkMap = SECTREE_MANAGER::Instance().GetMap(lMapIndex);

	if (pkMap)
		regen_load_in_file(szFilename, lMapIndex, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
}

CTeamWar::~CTeamWar() = default;

auto CTeamWar::IncMember(LPCHARACTER ch) -> void
{
	SendLoginPacket(ch);
	ch->StartSelectTeamEvent();

	if (m_bTeamStartWar)
		SendDurationTimePacket(ch);

	m_set_pkChr.insert(ch);
	m_set_hddSerialNo.insert(ch->GetHddSerialNo());
}

auto CTeamWar::DecMember(LPCHARACTER ch) -> void
{
	m_set_pkChr.erase(ch);
	m_set_hddSerialNo.erase(ch->GetHddSerialNo());

	DWORD dwPID = ch->GetPlayerID();
	auto pTeamWarPlayerData = GetTeamWarPlayer(dwPID);

	if (pTeamWarPlayerData)
	{
		m_TeamData[pTeamWarPlayerData->bTeam].bMemberCount--;
		DeletePlayerData(dwPID);
		SendPlayerCountPacket();
	}

	if (!m_bEndTeamWar)
		CheckEndTeamWar();
}

auto CTeamWar::GetTeamWarPlayer(DWORD dwPID) -> CTeamWar::TTeamWarPlayerData* 
{
	auto it = m_TeamWarPlayerData.find(dwPID);
	return it != m_TeamWarPlayerData.end() ? it->second : nullptr;
}

auto CTeamWar::JoinTeam(LPCHARACTER ch, BYTE bTeam) -> bool
{
	if (!ch || m_bEndTeamWar || GetTeamWarPlayer(ch->GetPlayerID()))
		return false;

	if (m_TeamData[bTeam].bMemberCount >= TEAM_WAR_MAX_PLAYER_COUNT)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Takým Savaþý> %s takým þuanda dolu." , bTeam == TEAM_WAR_TEAM_BLUE ? "Mavi" : "Kýrmýzý");
		return false;
	}

	if (ch->GetParty())
	{
		if (ch->GetParty()->GetMemberCount() == 2)
			CPartyManager::Instance().DeleteParty(ch->GetParty());
		else
			ch->GetParty()->Quit(ch->GetPlayerID());
	}

	NewPlayerData(ch, bTeam);
	m_TeamData[bTeam].bMemberCount++;
	SendPlayerCountPacket();
	return true;
}

auto CTeamWar::NewPlayerData(LPCHARACTER ch, BYTE bTeam) -> void
{
	if (!ch)
		return;

	auto pTeamWarPlayerData = new TTeamWarPlayerData;
	strlcpy(pTeamWarPlayerData->szName, ch->GetName(), sizeof(pTeamWarPlayerData->szName));
	pTeamWarPlayerData->bKill = 0;
	pTeamWarPlayerData->bDead = 0;
	pTeamWarPlayerData->iDamage = 0;
	pTeamWarPlayerData->bTeam = bTeam;
	m_TeamWarPlayerData.emplace(ch->GetPlayerID(), pTeamWarPlayerData);
}

auto CTeamWar::DeletePlayerData(DWORD dwPID) -> void
{
	auto it = m_TeamWarPlayerData.find(dwPID);
	if (it != m_TeamWarPlayerData.end())
	{
		delete it->second;
		m_TeamWarPlayerData.erase(it);
	}
}

auto CTeamWar::CheckEndTeamWar() -> void
{
	if (m_TeamData[TEAM_WAR_TEAM_BLUE].bMemberCount <= 0 && m_TeamData[TEAM_WAR_TEAM_RED].bMemberCount <= 0)
	{
		m_bEndTeamWar = true;
		StartDestroyTeamWarEvent();
	}
}

auto CTeamWar::OnKill(LPCHARACTER pkKiller, LPCHARACTER pkKilled) -> void
{
	if (!pkKiller || !pkKilled)
		return;

	auto pTeamWarPlayerData = GetTeamWarPlayer(pkKiller->GetPlayerID());
	if (pTeamWarPlayerData)
	{
		pTeamWarPlayerData->bKill += 1;
		m_TeamData[pTeamWarPlayerData->bTeam].wScore++;
	}

	pTeamWarPlayerData = GetTeamWarPlayer(pkKilled->GetPlayerID());
	if (pTeamWarPlayerData)
		pTeamWarPlayerData->bDead += 1;

	SendScorePacket();
}

auto CTeamWar::OnDamage(LPCHARACTER pkAttacker, int iDamage) -> void
{
	if (!pkAttacker || m_bEndTeamWar)
		return;

	auto pTeamWarPlayerData = GetTeamWarPlayer(pkAttacker->GetPlayerID());
	if (pTeamWarPlayerData)
		pTeamWarPlayerData->iDamage += iDamage;
}

auto CTeamWar::SendPlayerDataPacket(LPCHARACTER ch) -> void
{
	if (!ch || m_TeamWarPlayerData.empty())
		return;

	auto d = ch->GetDesc();
	if (!d)
		return;

	std::vector<TTeamWarPlayerData*> sendLists;

	for (const auto& it : m_TeamWarPlayerData)
		sendLists.emplace_back(it.second);

	std::sort(sendLists.begin(), sendLists.end(), [](auto a, auto b) { return (a->bKill != b->bKill ? a->bKill > b->bKill : a->iDamage > b->iDamage); });

	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_PLAYER_DATA;
	p.size = sizeof(p);

	TPacketGCTeamWarPlayerData player_data_packet;
	TEMP_BUFFER buf;

	BYTE bList = 1;
	for (auto pPlayer : sendLists)
	{
		if (pPlayer->bTeam == TEAM_WAR_TEAM_BLUE)
		{
			auto ch = CHARACTER_MANAGER::instance().FindPC(pPlayer->szName);
			if (ch)
			{
				player_data_packet.bList = bList++;
				player_data_packet.bRace = ch->GetRaceNum();
				player_data_packet.bLevel = ch->GetLevel();
				strlcpy(player_data_packet.szName, pPlayer->szName, sizeof(player_data_packet.szName));
				player_data_packet.bKill = pPlayer->bKill;
				player_data_packet.bDead = pPlayer->bDead;
				player_data_packet.iDamage = pPlayer->iDamage;
				buf.write(&player_data_packet, sizeof(player_data_packet));
			}
		}
	}

	bList = 9;
	for (auto pPlayer : sendLists)
	{
		if (pPlayer->bTeam == TEAM_WAR_TEAM_RED)
		{
			auto ch = CHARACTER_MANAGER::instance().FindPC(pPlayer->szName);
			if (ch)
			{
				player_data_packet.bList = bList++;
				player_data_packet.bRace = ch->GetRaceNum();
				player_data_packet.bLevel = ch->GetLevel();
				strlcpy(player_data_packet.szName, pPlayer->szName, sizeof(player_data_packet.szName));
				player_data_packet.bKill = pPlayer->bKill;
				player_data_packet.bDead = pPlayer->bDead;
				player_data_packet.iDamage = pPlayer->iDamage;
				buf.write(&player_data_packet, sizeof(player_data_packet));
			}
		}
	}

	p.size += buf.size();
	d->BufferedPacket(&p, sizeof(p));
	d->Packet(buf.read_peek(), buf.size());
}

auto CTeamWar::StartDestroyTeamWarEvent() -> void
{
	auto info = AllocEventInfo<team_war_map_info>();
	info->pTeamWar = this;

	m_pkDestroyTeamWar = event_create(team_war_destroy_event, info, PASSES_PER_SEC(10));
}

auto CTeamWar::Notice(const char* psz) -> void
{
	for (auto ch : m_set_pkChr)
	{
		if (ch)
			ch->ChatPacket(CHAT_TYPE_NOTICE, psz);
	}
}

auto CTeamWar::GoHome() -> void
{
	for (auto ch : m_set_pkChr)
	{
		if (ch)
			ch->GoHome();
	}
}

auto CTeamWar::SendLoginPacket(LPCHARACTER ch) -> void
{
	if (!ch)
		return;

	auto d = ch->GetDesc();
	if (!d)
		return;

	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_LOGIN;
	p.size = sizeof(p) + sizeof(DWORD) + sizeof(TPacketGCTeamWarPlayerCount) + sizeof(TPacketGCTeamWarScore);

	auto teamBlue = m_TeamData[TEAM_WAR_TEAM_BLUE];
	auto teamRed = m_TeamData[TEAM_WAR_TEAM_RED];

	TPacketGCTeamWarPlayerCount player_count_packet;
	player_count_packet.bTeamBluePlayerCount = teamBlue.bMemberCount;
	player_count_packet.bTeamRedPlayerCount = teamRed.bMemberCount;

	TPacketGCTeamWarScore score_packet;
	score_packet.wTeamBlueScore = teamBlue.wScore;
	score_packet.wTeamRedScore = teamRed.wScore;

	TEMP_BUFFER buf;
	buf.write(&m_dwTeamWarNo, sizeof(m_dwTeamWarNo));
	buf.write(&player_count_packet, sizeof(player_count_packet));
	buf.write(&score_packet, sizeof(score_packet));

	d->BufferedPacket(&p, sizeof(p));
	d->Packet(buf.read_peek(), buf.size());
}

auto CTeamWar::SendDurationTimePacket(LPCHARACTER ch) -> void
{
	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_DURATION_TIME;
	p.size = sizeof(p) + sizeof(m_dwEndTime);

	TEMP_BUFFER buf;
	buf.write(&m_dwEndTime, sizeof(m_dwEndTime));

	if (ch)
	{
		auto d = ch->GetDesc();
		if (d)
		{
			d->BufferedPacket(&p, sizeof(p));
			d->Packet(buf.read_peek(), buf.size());
		}
	}
	else
	{
		for (auto ch : m_set_pkChr)
		{
			if (ch)
			{
				auto d = ch->GetDesc();
				if (d)
				{
					d->BufferedPacket(&p, sizeof(p));
					d->Packet(buf.read_peek(), buf.size());
				}
			}
		}
	}
}

auto CTeamWar::SendKillNotice(const char* c_szKillerName, const char* c_szVictimName, BYTE bKillerRace, BYTE bVictimRace) -> void
{
	char buf[64];
	snprintf(buf, sizeof(buf), "GW_Kill_Update %s %u %s %u", c_szKillerName, bKillerRace, c_szVictimName, bVictimRace);

	for (auto ch : m_set_pkChr)
	{
		if (ch)
			ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
	}
}

auto CTeamWar::SendScorePacket() -> void
{
	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_SCORE;
	p.size = sizeof(p) + sizeof(TPacketGCTeamWarScore);

	TPacketGCTeamWarScore score_packet;
	score_packet.wTeamBlueScore = m_TeamData[TEAM_WAR_TEAM_BLUE].wScore;
	score_packet.wTeamRedScore =  m_TeamData[TEAM_WAR_TEAM_RED].wScore;

	TEMP_BUFFER buf;
	buf.write(&score_packet, sizeof(score_packet));

	for (auto ch : m_set_pkChr)
	{
		auto d = ch->GetDesc();
		if (d)
		{
			d->BufferedPacket(&p, sizeof(p));
			d->Packet(buf.read_peek(), buf.size());
		}
	}
}

auto CTeamWar::SendPlayerCountPacket() -> void
{
	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_PLAYER_COUNT;
	p.size = sizeof(p) + sizeof(TPacketGCTeamWarPlayerCount);

	TPacketGCTeamWarPlayerCount player_count_packet;
	player_count_packet.bTeamBluePlayerCount = m_TeamData[TEAM_WAR_TEAM_BLUE].bMemberCount;
	player_count_packet.bTeamRedPlayerCount = m_TeamData[TEAM_WAR_TEAM_RED].bMemberCount;

	TEMP_BUFFER buf;
	buf.write(&player_count_packet, sizeof(player_count_packet));

	for (auto ch : m_set_pkChr)
	{
		auto d = ch->GetDesc();
		if (d)
		{
			d->BufferedPacket(&p, sizeof(p));
			d->Packet(buf.read_peek(), buf.size());
		}
	}
}

auto CTeamWar::Destroy() -> void
{
	event_cancel(&m_pkTeamWarEvent);
	event_cancel(&m_pkDestroyTeamWar);

	for (const auto& it : m_TeamWarPlayerData)
		delete it.second;

	m_TeamWarPlayerData.clear();
	m_set_pkChr.clear();
}

CTeamWarManager::CTeamWarManager() = default;
CTeamWarManager::~CTeamWarManager() = default;

auto CTeamWarManager::TeamWarNo(long lMapIndex) -> DWORD
{
	std::mt19937 rng(lMapIndex);
	std::uniform_int_distribution<std::mt19937::result_type> random(100000, 999999);

	return random(rng);
}

auto CTeamWarManager::CreateTeamWarMap(bool bPrivate, DWORD dwPID) -> long
{
	auto lMapIndex = SECTREE_MANAGER::Instance().CreatePrivateMap(TEAM_WAR_MAP_IDX);

	if (lMapIndex)
	{
		DWORD dwTeamWarNo = TeamWarNo(lMapIndex);
		auto pTeamWar = Find(dwTeamWarNo);

		if (pTeamWar)
		{
			sys_err("TEAM_WAR: CreateTeamWarMap: TeamWarNo(%u) already exists", dwTeamWarNo);
			SECTREE_MANAGER::Instance().DestroyPrivateMap(lMapIndex);
			return 0;
		}

		m_TeamWar.emplace(dwTeamWarNo, new CTeamWar(dwTeamWarNo, bPrivate, lMapIndex));

		auto pkCCI = P2P_MANAGER::instance().FindByPID(dwPID);
		if (pkCCI)
		{
			char szNotice[128];
			snprintf(szNotice, sizeof(szNotice), "<Takým Savaþý> %s adlý oyuncu takým savaþý oluþturdu.", pkCCI->szName);
			BroadcastNotice(szNotice);

			if (!bPrivate)
				SendTeamWarInfo(pkCCI->szName, dwTeamWarNo);
		}
	}

	return lMapIndex;
}

auto CTeamWarManager::JoinTeamWar(DWORD dwTeamWarNo, DWORD dwPID, long &lMapIndex) -> bool
{
	if (dwTeamWarNo)
	{
		auto pTeamWar = Find(dwTeamWarNo);

		if (!pTeamWar || pTeamWar->IsEndTeamWar())
		{
			ChatTeamWar(dwPID, "<Takým Savaþý> Savaþ bulunamadý.");
			return false;
		}

		if (!pTeamWar->IsPrivateTeamWar())
		{
			ChatTeamWar(dwPID, "<Takým Savaþý> Bu savaþ özel olmadýðý için savaþ numarasýyla katýlamazsýn.");
			return false;
		}

		if (pTeamWar->GetBlueTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT && pTeamWar->GetRedTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT)
		{
			ChatTeamWar(dwPID, "<Takým Savaþý> Bu savaþ þuanda dolu olduðu için katýlamazsýn.");
			return false;
		}

		lMapIndex = pTeamWar->GetMapIndex();
		return true;
	}
	else
	{
		std::vector<CTeamWar*> vec_team_war;

		for (const auto &it : m_TeamWar)
			vec_team_war.emplace_back(it.second);

		std::sort(vec_team_war.begin(), vec_team_war.end(), [](auto a, auto b) { return (a->GetPlayerTotalCount() > b->GetPlayerTotalCount()); });

		for (auto pTeamWar : vec_team_war)
		{
			if (pTeamWar && !pTeamWar->IsEndTeamWar() && !pTeamWar->IsPrivateTeamWar() && !(pTeamWar->GetBlueTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT && pTeamWar->GetRedTeamPlayerCount() >= TEAM_WAR_MAX_PLAYER_COUNT))
			{	
				lMapIndex = pTeamWar->GetMapIndex();
				return true;
			}
		}
	}

	ChatTeamWar(dwPID, "<Takým Savaþý> Uygun bir savaþ bulunamadý daha sonra tekrar deneyiniz.");
	return false;
}

auto CTeamWarManager::WarpTeamWar(DWORD dwPID, long lMapIndex) -> void
{
	TPacketGGTeamWar p2p_packet;
	p2p_packet.bHeader = HEADER_GG_TEAM_WAR;
	p2p_packet.bSubHeader = TEAM_WAR_SUBHEADER_GG_WARP;
	p2p_packet.dwPID = dwPID;

	TEMP_BUFFER buf;
	buf.write(&p2p_packet, sizeof(p2p_packet));
	buf.write(&lMapIndex, sizeof(lMapIndex));

	//Send to P2P
	P2P_MANAGER::Instance().Send(buf.read_peek(), buf.size());
}

auto CTeamWarManager::ChatTeamWar(DWORD dwPID, std::string strChat) -> void
{
	TPacketGGTeamWar p2p_packet;
	p2p_packet.bHeader = HEADER_GG_TEAM_WAR;
	p2p_packet.bSubHeader = TEAM_WAR_SUBHEADER_GG_CHAT;
	p2p_packet.dwPID = dwPID;

	TEMP_BUFFER buf;
	buf.write(&p2p_packet, sizeof(p2p_packet));
	buf.write(strChat.c_str(), strChat.size());

	//Send to P2P
	P2P_MANAGER::Instance().Send(buf.read_peek(), buf.size());
}

auto CTeamWarManager::SendTeamWarInfo(const std::string strName, DWORD dwTeamWarNo) -> void
{
	TPacketGGTeamWar p2p_packet;
	p2p_packet.bHeader = HEADER_GG_TEAM_WAR;
	p2p_packet.bSubHeader = TEAM_WAR_SUBHEADER_GG_INFO;
	p2p_packet.dwPID = dwTeamWarNo; //Not PID because this packet is send to all players.

	TEMP_BUFFER buf;
	buf.write(&p2p_packet, sizeof(p2p_packet));
	buf.write(strName.c_str(), strName.size());

	//Send to P2P
	P2P_MANAGER::Instance().Send(&p2p_packet, sizeof(p2p_packet));
}

auto CTeamWarManager::SendTeamWarInfoPacket(const std::string strName, DWORD dwTeamWarNo) -> void
{
	TPacketGCTeamWar p;
	p.header = HEADER_GC_TEAM_WAR;
	p.subheader = TEAM_WAR_SUBHEADER_GC_INFO;
	p.size = sizeof(p) + sizeof(TPacketGCTeamWarInfo);

	TPacketGCTeamWarInfo info_packet;
	strlcpy(info_packet.szName, strName.c_str(), sizeof(info_packet.szName));
	info_packet.dwTeamWarNo = dwTeamWarNo;

	TEMP_BUFFER buf;
	buf.write(&info_packet, sizeof(info_packet));

	const auto& c_ref_set = DESC_MANAGER::Instance().GetClientSet();
	for (auto d : c_ref_set)
	{
		if (d)
		{
			d->BufferedPacket(&p, sizeof(p));
			d->Packet(buf.read_peek(), buf.size());
		}
	}
}

auto CTeamWarManager::DestroyTeamWar(CTeamWar *pTeamWar) -> void
{
	long lMapIndex = pTeamWar->GetMapIndex();

	m_TeamWar.erase(pTeamWar->GetTeamWarNo());
	delete pTeamWar;

	SECTREE_MANAGER::Instance().DestroyPrivateMap(lMapIndex);
}

auto CTeamWarManager::Find(DWORD dwTeamWarNo) -> CTeamWar*
{
	auto it = m_TeamWar.find(dwTeamWarNo);
	return it != m_TeamWar.end() ? it->second : nullptr;
}