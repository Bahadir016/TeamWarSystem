//Search:
protected:
	CPacketInfoGG 	m_packetInfoGG;

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	auto		TeamWar(const char *c_pData, size_t uiBytes) -> int;
#endif