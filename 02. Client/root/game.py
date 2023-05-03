#Add in imports:
if app.ENABLE_TEAM_WAR_SYSTEM:
	import uiteamwar

# Search:
		self.__ServerCommand_Build()
		self.__ProcessPreservedServerCommand()

#Add below:
		if app.ENABLE_TEAM_WAR_SYSTEM:
			self.teamWarPlayerDataListBoard = uiteamwar.TeamWarPlayerDataListBoard()
			self.teamWarPlayerDataListBoard.LoadDialog()
			self.teamWarPlayerDataListBoard.Hide()

			self.teamWarScoreBoard = uiteamwar.ScoreBoard()
			self.teamWarScoreBoard.LoadDialog()
			self.teamWarScoreBoard.Hide()

# Search:
		player.ClearSkillDict()
		player.ResetCameraRotation()

# Add above:
		if app.ENABLE_TEAM_WAR_SYSTEM:
			if self.teamWarPlayerDataListBoard:
				self.teamWarPlayerDataListBoard.Hide()
				self.teamWarPlayerDataListBoard = None

			if self.teamWarScoreBoard:
				self.teamWarScoreBoard.Hide()
				self.teamWarScoreBoard = None

# Search:
		onPressKeyDict[app.DIK_F4]	= lambda : self.__PressQuickSlot(7)

# Add below:
		if app.ENABLE_TEAM_WAR_SYSTEM:
			onPressKeyDict[app.DIK_F5] = lambda : self.OpenTeamWarWindow()

# Add at the end of the file:
	if app.ENABLE_TEAM_WAR_SYSTEM:
		def OpenTeamWarWindow(self):
			self.TeamWarWindow = uiteamwar.TeamWarWindow()
			self.TeamWarWindow.Open()

		def BINARY_TeamWarUpdatePlayerData(self, list, race, level, name, kill, dead, damage):
			self.teamWarPlayerDataListBoard.Update(int(list), int(race), int(level), str(name), int(kill), int(dead), int(damage))

		def BINARY_TeamWarUpdateScore(self, blueteamscore, redteamscore):
			self.teamWarPlayerDataListBoard.UpdateScore(int(blueteamscore), int(redteamscore))
			self.teamWarScoreBoard.SetScore(int(blueteamscore), int(redteamscore))

		def BINARY_TeamWarUpdatePlayerCount(self, blueteamplayercount, redteamplayercount):
			self.teamWarScoreBoard.SetPlayerCount(int(blueteamplayercount), int(redteamplayercount))

		def OpenTeamWarPlayerDataList(self):
			if self.teamWarPlayerDataListBoard:
				if not self.teamWarPlayerDataListBoard.IsShow():
					net.SendChatPacket("/team_war_load_player_data")
					self.teamWarPlayerDataListBoard.Show()
				else:
					self.teamWarPlayerDataListBoard.Hide()

		def BINARY_TeamWarUpdateDurationTime(self, duration):
			self.TeamWarDurationWindow = uiteamwar.TeamWarDurationWindow()
			self.TeamWarDurationWindow.Open(int(duration))

		def BINARY_TeamWarLogin(self, teamwarno, blueTeamPlayerCount, redTeamPlayerCount, blueTeamScore, redTeamScore):
			if self.teamWarScoreBoard:
				if not self.teamWarScoreBoard.IsShow():
					self.teamWarScoreBoard.Show()

			self.teamWarScoreBoard.SetTeamWarNo(int(teamwarno))

			SelectTeamDialog = uiteamwar.SelectTeamDialog()
			SelectTeamDialog.SetAcceptEvent(lambda arg = SelectTeamDialog.TEAM_WAR_TEAM_BLUE: self.SelectTeam(arg))
			SelectTeamDialog.SetCancelEvent(lambda arg = SelectTeamDialog.TEAM_WAR_TEAM_RED: self.SelectTeam(arg))
			SelectTeamDialog.Open(int(blueTeamPlayerCount), int(redTeamPlayerCount))
			self.SelectTeamDialog = SelectTeamDialog

			self.teamWarPlayerDataListBoard.UpdateScore(int(blueTeamScore), int(redTeamScore))
			self.teamWarScoreBoard.SetScore(int(blueTeamScore), int(redTeamScore))
			self.teamWarScoreBoard.SetPlayerCount(int(blueTeamPlayerCount), int(redTeamPlayerCount))

		def SelectTeam(self, arg):
			net.SendChatPacket("/select_team %d" % (arg))

			self.SelectTeamDialog.Close()
			self.SelectTeamDialog = None