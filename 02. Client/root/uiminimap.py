# Add in imports:
if app.ENABLE_TEAM_WAR_SYSTEM:
	import uiteamwar

# Search:
		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

# Add above:
		if app.ENABLE_TEAM_WAR_SYSTEM:
			self.teamWarButton = ui.Button()
			self.teamWarButton.SetPosition(wndMgr.GetScreenWidth()-350, 5)
			self.teamWarButton.SetUpVisual("team_war_button.png")
			self.teamWarButton.SetOverVisual("team_war_button.png")
			self.teamWarButton.SetDownVisual("team_war_button.png")
			self.teamWarButton.SetEvent(ui.__mem_func__(self.OpenTeamWar))
			self.teamWarButton.Show()

# Add at the end of the file:
	if app.ENABLE_TEAM_WAR_SYSTEM:
		def OpenTeamWar(self):
			self.TeamWarWindow = uiteamwar.TeamWarWindow()
			self.TeamWarWindow.Open()