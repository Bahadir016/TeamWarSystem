//Search:
		case HEADER_GC_DRAGON_SOUL_REFINE:
			ret = RecvDragonSoulRefine();
			break;

//Add below:
#ifdef ENABLE_TEAM_WAR_SYSTEM
		case HEADER_GC_TEAM_WAR:
			ret = RecvTeamWar();
			break;
#endif

//At the end of the file:
#ifdef ENABLE_TEAM_WAR_SYSTEM
auto CPythonNetworkStream::RecvTeamWar() -> bool
{
	TPacketGCTeamWar TeamWarPacket;
	if (!Recv(sizeof(TeamWarPacket), &TeamWarPacket))
		return false;

	BYTE bPacketSize = TeamWarPacket.size - sizeof(TeamWarPacket);
	switch (TeamWarPacket.subheader)
	{
		case TEAM_WAR_SUBHEADER_GC_LOGIN:
		{
			DWORD dwTeamWarNo = 0;
			if (!Recv(sizeof(dwTeamWarNo), &dwTeamWarNo))
				return false;

			TPacketGCTeamWarPlayerCount player_count_packet;
			if (!Recv(sizeof(player_count_packet), &player_count_packet))
				return false;

			TPacketGCTeamWarScore score_packet;
			if (!Recv(sizeof(score_packet), &score_packet))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarLogin", Py_BuildValue("(iiiii)", dwTeamWarNo, player_count_packet.bTeamBluePlayerCount, player_count_packet.bTeamRedPlayerCount, score_packet.wTeamBlueScore, score_packet.wTeamRedScore));
		}
		break;

		case TEAM_WAR_SUBHEADER_GC_DURATION_TIME:
		{
			DWORD dwDurationTime = 0;
			if (!Recv(sizeof(dwDurationTime), &dwDurationTime))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarUpdateDurationTime", Py_BuildValue("(i)", dwDurationTime));
		}
		break;

		case TEAM_WAR_SUBHEADER_GC_SCORE:
		{
			TPacketGCTeamWarScore score_packet;
			if (!Recv(sizeof(score_packet), &score_packet))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarUpdateScore", Py_BuildValue("(ii)", score_packet.wTeamBlueScore, score_packet.wTeamRedScore));
		}
		break;

		case TEAM_WAR_SUBHEADER_GC_PLAYER_COUNT:
		{
			TPacketGCTeamWarPlayerCount player_count_packet;
			if (!Recv(sizeof(player_count_packet), &player_count_packet))
				return false;

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarUpdatePlayerCount", Py_BuildValue("(ii)", player_count_packet.bTeamBluePlayerCount, player_count_packet.bTeamRedPlayerCount));
		}
		break;

		case TEAM_WAR_SUBHEADER_GC_INFO:
		{
			TPacketGCTeamWarInfo info_packet;
			if (!Recv(sizeof(info_packet), &info_packet))
				return false;
			
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarUpdateInfo", Py_BuildValue("(si)", info_packet.szName, info_packet.dwTeamWarNo));
		}

		case TEAM_WAR_SUBHEADER_GC_PLAYER_DATA:
		{
			while (bPacketSize)
			{
				TPacketGCTeamWarPlayerData player_data_packet;
				if (!Recv(sizeof(player_data_packet), &player_data_packet))
					return false;

				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_TeamWarUpdatePlayerData", Py_BuildValue("(iiisiii)", player_data_packet.bList, player_data_packet.bRace, player_data_packet.bLevel, player_data_packet.szName, player_data_packet.bKill, player_data_packet.bDead, player_data_packet.iDamage));
				bPacketSize -= sizeof(player_data_packet);
			}
		}
		break;

		default:
			TraceError("CPythonNetworkStream::RecvTeamWar - Unknown SubHeader %d", TeamWarPacket.subheader);
			return false;
	}

	return true;
}
#endif