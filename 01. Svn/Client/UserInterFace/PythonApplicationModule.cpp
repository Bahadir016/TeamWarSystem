//Add modules:
#ifdef ENABLE_TEAM_WAR_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_TEAM_WAR_SYSTEM", true);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_TEAM_WAR_SYSTEM", true);
#endif