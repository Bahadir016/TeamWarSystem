//Search:
struct command_info cmd_info[] =

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
ACMD(do_start_team_war);
ACMD(do_join_team_war);
ACMD(do_select_team);
ACMD(do_team_war_load_player_data);
#endif

//Search:
{ "\n",

//Add above:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	{ "start_team_war",		do_start_team_war,	0,	POS_DEAD,	GM_PLAYER },
	{ "join_team_war",		do_join_team_war,	0,	POS_DEAD,	GM_PLAYER },
	{ "select_team",		do_select_team,	0,		POS_DEAD,	GM_PLAYER },
	{ "team_war_load_player_data",	do_team_war_load_player_data,	0,	POS_DEAD,	GM_PLAYER },
#endif