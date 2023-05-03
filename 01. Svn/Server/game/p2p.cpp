//At the end of the file:
#ifdef ENABLE_TEAM_WAR_SYSTEM
#include "team_war.h"
auto P2P_MANAGER::SendTeamWar(const void *c_pvData, int iSize) -> void
{
	TR1_NS::unordered_set<LPDESC>::iterator it = m_set_pkPeers.begin();

	while (it != m_set_pkPeers.end())
	{
		LPDESC pkDesc = *it++;

		if (pkDesc->GetP2PPort() == TEAM_WAR_P2P_PORT)
			pkDesc->Packet(c_pvData, iSize);
	}
}
#endif