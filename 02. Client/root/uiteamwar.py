import ui
import app
import playerSettingModule
import localeInfo
import uicommon
import wndMgr
import net
import chat

class CreateTeamWarWindow(ui.BoardWithTitleBar):
	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)
		self.isLoad = False
		self.isPrivateTeamWar = 0
		self.teamWarTypeButton = []
		self.teamWarTypeText = None
		self.createTeamWarButton = None
		self.cancelTeamWarButton = None
		self.__LoadDialog()
	
	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)
	
	def __LoadDialog(self):
		if self.isLoad:
			return

		self.isLoad = True
		self.AddFlag('movable')
		self.AddFlag('float')
		self.AddFlag('animate')
		self.SetSize(270, 100)
		self.SetTitleName("Takým Savaþý Oluþtur")
		self.SetCenterPosition()
		self.SetTop()

		teamWarTypeText = ui.TextLine()
		teamWarTypeText.SetText("Savaþ Türü")
		teamWarTypeText.SetParent(self)
		teamWarTypeText.SetPosition(15, 42)
		teamWarTypeText.Show()
		self.teamWarTypeText = teamWarTypeText

		textList = [["Herkese Açýk", "Herkese açýk savaþ oluþturulduðunda savaþa rastgele tüm oyuncular katýlabilir."], ["Özel", "Özel savaþ oluþturulduðunda savaþa sadece savaþ numarasýný paylaþtýðýnýz arkadaþlarýnýz katýlabilir."]]
		for i in range(2):
			teamWarTypeButton = ui.RadioButton()
			teamWarTypeButton.SetParent(self)
			teamWarTypeButton.SetPosition(80 + i*90, 40)
			teamWarTypeButton.SetUpVisual("d:/ymir work/ui/public/large_Button_01.sub")
			teamWarTypeButton.SetOverVisual("d:/ymir work/ui/public/large_Button_02.sub")
			teamWarTypeButton.SetDownVisual("d:/ymir work/ui/public/large_Button_03.sub")
			teamWarTypeButton.SetText(textList[i][0])
			teamWarTypeButton.SetToolTipText(textList[i][1], 0, -15)
			teamWarTypeButton.SetEvent(lambda arg = i : self.SelectTeamWarType(arg))
			teamWarTypeButton.Show()
			self.teamWarTypeButton.append(teamWarTypeButton)

		self.teamWarTypeButton[0].Down()

		createTeamWarButton = ui.Button()
		createTeamWarButton.SetParent(self)
		createTeamWarButton.SetPosition(40, 70)
		createTeamWarButton.SetUpVisual("d:/ymir work/ui/public/large_Button_01.sub")
		createTeamWarButton.SetOverVisual("d:/ymir work/ui/public/large_Button_02.sub")
		createTeamWarButton.SetDownVisual("d:/ymir work/ui/public/large_Button_03.sub")
		createTeamWarButton.SetText("Oluþtur")
		createTeamWarButton.SetEvent(self.CreateTeamWar)
		createTeamWarButton.Show()
		self.createTeamWarButton = createTeamWarButton

		cancelTeamWarButton = ui.Button()
		cancelTeamWarButton.SetParent(self)
		cancelTeamWarButton.SetPosition(140, 70)
		cancelTeamWarButton.SetUpVisual("d:/ymir work/ui/public/large_Button_01.sub")
		cancelTeamWarButton.SetOverVisual("d:/ymir work/ui/public/large_Button_02.sub")
		cancelTeamWarButton.SetDownVisual("d:/ymir work/ui/public/large_Button_03.sub")
		cancelTeamWarButton.SetText("Vazgeç")
		cancelTeamWarButton.SetEvent(self.Close)
		cancelTeamWarButton.Show()
		self.cancelTeamWarButton = cancelTeamWarButton

	def SelectTeamWarType(self, arg):
		self.teamWarTypeButton[self.isPrivateTeamWar].SetUp()
		self.isPrivateTeamWar = arg
		self.teamWarTypeButton[arg].Down()
	
	def CreateTeamWar(self):
		net.SendChatPacket("/start_team_war %d" % self.isPrivateTeamWar)
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Close(self):
		self.Hide()


class TeamWarWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/teamwarwindow.py")
		except:
			import exception
			exception.Abort("TeamWarWindow.__LoadDialog.LoadObject")

		try:
			self.titleName = self.GetChild("TitleName")
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
			self.cancelButton = self.GetChild("cancel_button")

			self.createteamwar = self.GetChild("create_team_war")
			self.jointeamwar = self.GetChild("join_team_war")
			self.jointeamwarprivate = self.GetChild("join_team_war_private")
			self.jointeamwarobserver = self.GetChild("join_team_war_observer")
		except:
			import exception
			exception.Abort("TeamWarWindow.__LoadDialog.BindObject")

		self.cancelButton.SetEvent(ui.__mem_func__(self.OnCancel))
		self.cancelButton.Hide()

		self.createteamwar.SetEvent(ui.__mem_func__(self.CreateTeamWar))
		self.jointeamwar.SetEvent(ui.__mem_func__(self.JoinTeamWar))
		self.jointeamwarprivate.SetEvent(ui.__mem_func__(self.JoinTeamWarPrive))
		self.jointeamwarobserver.SetEvent(ui.__mem_func__(self.JoinTeamWarObserver))

		self.SetCenterPosition()

	def Destroy(self):
		self.ClearDictionary()
		self.titleName = None
		self.cancelButton = None

		self.createteamwar = None
		self.jointeamwar = None
		self.jointeamwarprivate = None
		self.jointeamwarobserver = None

	def SetTitle(self, title):
		self.titleName.SetText(title)

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def CreateTeamWar(self):
		CreateTeamWar = CreateTeamWarWindow()
		CreateTeamWar.Show()
		self.CreateTeamWar = CreateTeamWar
		self.OnCancel()

	def PrivateTeamWar(self, arg):
		if arg:
			net.SendChatPacket("/start_team_war 1")
		else:
			net.SendChatPacket("/start_team_war")

		self.PrivateWarDialog.Close()
		self.PrivateWarDialog = None

	def JoinTeamWar(self):
		JoinWardialog = uicommon.QuestionDialog()
		JoinWardialog.SetText("Takým Savaþýna katýlmak istiyor musun ?")
		JoinWardialog.SetAcceptEvent(lambda arg = True: self.JoinWar(arg))
		JoinWardialog.SetCancelEvent(lambda arg = False: self.JoinWar(arg))
		JoinWardialog.Open()
		self.JoinWardialog = JoinWardialog
		self.Hide()

	def JoinWar(self, arg):
		if arg:
			net.SendChatPacket("/join_team_war")

		self.JoinWardialog.Close()
		self.JoinWardialog = None

	def JoinTeamWarPrive(self):
		JoinPrivateWarDialog = uicommon.MoneyInputDialog()
		JoinPrivateWarDialog.SetTitle("Savaþ Numarasý")
		JoinPrivateWarDialog.HideText()
		JoinPrivateWarDialog.SetAcceptEvent(ui.__mem_func__(self.JoinPrivateWar))
		JoinPrivateWarDialog.SetCancelEvent(ui.__mem_func__(self.CancelJoinPrivateWar))
		JoinPrivateWarDialog.SetMaxLength(6)
		JoinPrivateWarDialog.Open()
		self.JoinPrivateWarDialog = JoinPrivateWarDialog

	def JoinPrivateWar(self):
		teamwarno = int(self.JoinPrivateWarDialog.GetText())
		net.SendChatPacket("/join_team_war %d" % (teamwarno))

		self.JoinPrivateWarDialog.Close()
		self.JoinPrivateWarDialog = None

	def CancelJoinPrivateWar(self):
		self.JoinPrivateWarDialog.Close()
		self.JoinPrivateWarDialog = None

	def JoinTeamWarObserver(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Ýzleyici modu yapým aþamasýndadýr.")
		return
		JoinTeamWarObserverDialog = uicommon.MoneyInputDialog()
		JoinTeamWarObserverDialog.SetTitle("Savaþ Numarasý")
		JoinTeamWarObserverDialog.SetAcceptEvent(ui.__mem_func__(self.JoinWarObserver))
		JoinTeamWarObserverDialog.SetCancelEvent(ui.__mem_func__(self.CancelJoinObserver))
		JoinTeamWarObserverDialog.SetMaxLength(6)
		JoinTeamWarObserverDialog.Open()
		self.JoinTeamWarObserverDialog = JoinTeamWarObserverDialog

	def JoinWarObserver(self):
		teamwarno = int(self.JoinTeamWarObserverDialog.GetText())
		net.SendChatPacket("/join_team_war %d 1" % (teamwarno))

		self.JoinTeamWarObserverDialog.Close()
		self.JoinTeamWarObserverDialog = None

	def CancelJoinObserver(self):
		self.JoinTeamWarObserverDialog.Close()
		self.JoinTeamWarObserverDialog = None

	def OnPressEscapeKey(self):
		self.Hide()

	def OnCancel(self):
		self.Hide()

class SelectTeamDialog(uicommon.QuestionDialog):
	TEAM_WAR_TEAM_BLUE = 0
	TEAM_WAR_TEAM_RED = 1

	def __init__(self):
		uicommon.QuestionDialog.__init__(self)
		self.__CreateDialog()
		self.endTime = 0

	def __del__(self):
		uicommon.QuestionDialog.__del__(self)

	def __CreateDialog(self):
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "uiscript/selectteamdialog.py")

		self.board = self.GetChild("board")
		self.textLine1 = self.GetChild("message1")
		self.textLine2 = self.GetChild("message2")
		self.textLine3 = self.GetChild("message3")
		self.textLine4 = self.GetChild("message4")
		self.acceptButton = self.GetChild("accept")
		self.cancelButton = self.GetChild("cancel")

	def Open(self, blueTeamCount, redTeamCount):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

		self.textLine1.SetText("Katýlmak istediðin takýmý seç")

		if blueTeamCount >= 8:
			self.textLine2.SetText("Dolu")
			self.acceptButton.Down()
			self.acceptButton.Disable()
		else:
			self.textLine2.SetText("(Katýlýmcý sayýsý: %d)" % (blueTeamCount))

		if redTeamCount >= 8:
			self.textLine3.SetText("Dolu")
			self.cancelButton.Down()
			self.cancelButton.Disable()
		else:
			self.textLine3.SetText("(Katýlýmcý sayýsý: %d)" % (redTeamCount))

		self.endTime = app.GetTime() + 10

	def OnUpdate(self):
		leftTime = self.endTime - app.GetTime()
		self.textLine4.SetText("%d saniye içinde takýmýný seçmezsen köye ýþýnlanacaksýn." % (leftTime))

class ScoreBoard(ui.ImageBox):
	def __init__(self):
		ui.ImageBox.__init__(self)
		self.Initialize()

	def __del__(self):
		ui.ImageBox.__del__(self)

	def Initialize(self):
		self.isLoad = False
		self.blueteamscore = None
		self.redteamscore = None
		self.fark = None
		self.blueteamplayercount = None
		self.redteamplayercount = None
		self.teamwarno = None

	def LoadDialog(self):
		if self.isLoad:
			return

		self.isLoad = True
		self.LoadImage("skorboard.png")
		self.AddFlag("movable")
		self.SetPosition(10, wndMgr.GetScreenHeight() - 112)

		self.blueteamscore = ui.TextLine()
		self.blueteamscore.SetParent(self)
		self.blueteamscore.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		self.blueteamscore.SetFontColor(0, 0, 200)
		self.blueteamscore.SetWindowHorizontalAlignCenter()
		self.blueteamscore.SetHorizontalAlignCenter()
		self.blueteamscore.SetPosition(-83,30)
		self.blueteamscore.SetText("0")
		self.blueteamscore.Show()

		self.redteamscore = ui.TextLine()
		self.redteamscore.SetParent(self)
		self.redteamscore.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		self.redteamscore.SetFontColor(200, 0, 0)
		self.redteamscore.SetWindowHorizontalAlignCenter()
		self.redteamscore.SetHorizontalAlignCenter()
		self.redteamscore.SetPosition(81,30)
		self.redteamscore.SetText("0")
		self.redteamscore.Show()

		self.fark = ui.TextLine()
		self.fark.SetParent(self)
		self.fark.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		self.fark.SetOutline()
		self.fark.SetWindowHorizontalAlignCenter()
		self.fark.SetHorizontalAlignCenter()
		self.fark.SetPosition(2,32)
		self.fark.SetText("0")
		self.fark.Show()

		self.blueteamplayercount = ui.TextLine()
		self.blueteamplayercount.SetParent(self)
		self.blueteamplayercount.SetWindowHorizontalAlignCenter()
		self.blueteamplayercount.SetHorizontalAlignCenter()
		self.blueteamplayercount.SetPosition(-82,49)
		self.blueteamplayercount.SetText("(Katýlýmcý sayýsý: 0)")
		self.blueteamplayercount.Show()

		self.redteamplayercount = ui.TextLine()
		self.redteamplayercount.SetParent(self)
		self.redteamplayercount.SetWindowHorizontalAlignCenter()
		self.redteamplayercount.SetHorizontalAlignCenter()
		self.redteamplayercount.SetPosition(82,49)
		self.redteamplayercount.SetText("(Katýlýmcý sayýsý: 0)")
		self.redteamplayercount.Show()

		self.teamwarno = ui.TextLine()
		self.teamwarno.SetParent(self)
		self.teamwarno.SetWindowHorizontalAlignCenter()
		self.teamwarno.SetPosition(-37,60)
		self.teamwarno.SetText("Savaþ no: 0")
		self.teamwarno.Show()

	def SetTeamWarNo(self, teamwarno):
		self.teamwarno.SetText("Savaþ no: %d" % (teamwarno))

	def SetScore(self, blueteamscore, redteamscore):
		self.blueteamscore.SetText("%d" % (blueteamscore))
		self.redteamscore.SetText("%d" % (redteamscore))
		self.fark.SetText("%d" % (abs(blueteamscore - redteamscore)))

	def SetPlayerCount(self, blueteamplayer, redteamplayer):
		self.blueteamplayercount.SetText("(Katýlýmcý sayýsý: %d)" % (blueteamplayer))
		self.redteamplayercount.SetText("(Katýlýmcý sayýsý: %d)" % (redteamplayer))

class TeamWarDurationWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.durationtime = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/teamwardurationwindow.py")
		except:
			import exception
			exception.Abort("TeamWarDurationWindow.LoadWindow.LoadObject")
		try:
			self.board = self.GetChild("Thinboard")
			self.textLine1 = self.GetChild("message1")
		except:
			import exception
			exception.Abort("TeamWarDurationWindow.LoadWindow.LoadObject")

	def Open(self, time):
		self.SetTop()
		self.Show()
		self.durationtime = time

	def Close(self):
		self.Hide()

	def SecondToHM(self, time):
		second = int(time % 60)
		minute = int((time / 60) % 60)

		if time % 60 == 0:
			return "%d dakika" % (minute)
		else:
			return "%d dakika %02d saniye" % (minute, second)

	def OnUpdate(self):
		remain_time = self.durationtime - app.GetGlobalTimeStamp()
		if remain_time > 0:
			self.textLine1.SetText("Kalan zaman: %s" % (self.SecondToHM(remain_time)))
		else:
			self.Hide()

class TeamWarPlayerDataListBoard(ui.Board):
	FACE_IMAGE_DICT = {
						playerSettingModule.RACE_WARRIOR_M : "icon/shipwarfaces/warrior_m.tga",
						playerSettingModule.RACE_WARRIOR_W : "icon/shipwarfaces/warrior_w.tga",
						playerSettingModule.RACE_ASSASSIN_M : "icon/shipwarfaces/assassin_m.tga",
						playerSettingModule.RACE_ASSASSIN_W : "icon/shipwarfaces/assassin_w.tga",
						playerSettingModule.RACE_SURA_M : "icon/shipwarfaces/sura_m.tga",
						playerSettingModule.RACE_SURA_W : "icon/shipwarfaces/sura_w.tga",
						playerSettingModule.RACE_SHAMAN_M : "icon/shipwarfaces/shaman_m.tga",
						playerSettingModule.RACE_SHAMAN_W : "icon/shipwarfaces/shaman_w.tga",
	}

	def __init__(self):
		ui.Board.__init__(self)
		self.Initialize()

	def __del__(self):
		ui.Board.__del__(self)

	def Initialize(self):
		self.enemyDataDict = {1:{}, 2:{}, 3:{}, 4:{}, 5:{}, 6:{}, 7:{}, 8:{}, 9:{}, 10:{}, 11:{}, 12:{}, 13:{}, 14:{}, 15:{}, 16:{}}
		self.isLoad = False
		self.titleBar = None
		self.titleName = None
		self.image = None
		self.time = None
		self.blueteamscore = None
		self.redteamscore = None
		self.ship_war_time = 0

	def LoadDialog(self):
		if self.isLoad:
			return

		self.isLoad = True
		self.AddFlag('movable')
		self.AddFlag('float')
		self.SetSize(615, 561)
		self.SetCenterPosition()
		self.SetTop()

		self.titleBar = ui.TitleBar()
		self.titleBar.SetParent(self)
		self.titleBar.MakeTitleBar(601, "red")
		self.titleBar.CloseButtonHide()
		self.titleBar.SetPosition(8, 7)
		self.titleBar.Show()

		self.titleName = ui.TextLine()
		self.titleName.SetText("Skor Tablosu")
		self.titleName.SetParent(self.titleBar)
		self.titleName.SetPosition(0, 4)
		self.titleName.SetWindowHorizontalAlignCenter()
		self.titleName.SetHorizontalAlignCenter()
		self.titleName.Show()

		self.image = ui.ImageBox()
		self.image.SetParent(self)
		self.image.LoadImage("select.png")
		self.image.SetPosition(8, 30)
		self.image.Show()

		self.blueteamscore = ui.TextLine()
		self.blueteamscore.SetParent(self)
		self.blueteamscore.SetFontName(localeInfo.UI_DEF_FONT_BIG_LARGE)
		self.blueteamscore.SetHorizontalAlignCenter()
		self.blueteamscore.SetFontColor(0, 0, 200)
		self.blueteamscore.SetPosition(73, 117)
		self.blueteamscore.SetText("0")
		self.blueteamscore.Show()

		self.redteamscore = ui.TextLine()
		self.redteamscore.SetParent(self)
		self.redteamscore.SetFontName(localeInfo.UI_DEF_FONT_BIG_LARGE)
		self.redteamscore.SetHorizontalAlignCenter()
		self.redteamscore.SetFontColor(200, 0, 0)
		self.redteamscore.SetPosition(76, 385)
		self.redteamscore.SetText("0")
		self.redteamscore.Show()

		for i in xrange(8):
			race = ui.ImageBox()
			race.SetParent(self)
			race.SetPosition(193, 59 + 26*i)
			race.Hide()

			level = ui.TextLine()
			level.SetParent(self)
			level.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			level.SetHorizontalAlignCenter()
			level.SetOutline()
			level.SetPosition(244, 61 + 26*i)
			level.Hide()

			name = ui.TextLine()
			name.SetParent(self)
			name.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			name.SetHorizontalAlignCenter()
			name.SetOutline()
			name.SetPosition(315, 61 + 26*i)
			name.Hide()

			kill = ui.TextLine()
			kill.SetParent(self)
			kill.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			kill.SetHorizontalAlignCenter()
			kill.SetOutline()
			kill.SetPosition(405, 61 + 26*i)
			kill.Hide()

			dead = ui.TextLine()
			dead.SetParent(self)
			dead.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			dead.SetHorizontalAlignCenter()
			dead.SetOutline()
			dead.SetPosition(478, 61 + 26*i)
			dead.Hide()

			damage = ui.TextLine()
			damage.SetParent(self)
			damage.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			damage.SetHorizontalAlignCenter()
			damage.SetOutline()
			damage.SetPosition(556, 61 + 26*i)
			damage.Hide()

			self.enemyDataDict[i+1]["RACE"] = race
			self.enemyDataDict[i+1]["LEVEL"] = level
			self.enemyDataDict[i+1]["NAME"] = name
			self.enemyDataDict[i+1]["KILL"] = kill
			self.enemyDataDict[i+1]["DEAD"] = dead
			self.enemyDataDict[i+1]["DAMAGE"] = damage


		for i in xrange(8):
			race = ui.ImageBox()
			race.SetParent(self)
			race.SetPosition(193, 348 + 26*i)
			race.Hide()

			level = ui.TextLine()
			level.SetParent(self)
			level.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			level.SetHorizontalAlignCenter()
			level.SetOutline()
			level.SetPosition(244, 350 + 26*i)
			level.Hide()

			name = ui.TextLine()
			name.SetParent(self)
			name.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			name.SetHorizontalAlignCenter()
			name.SetOutline()
			name.SetPosition(315, 350 + 26*i)
			name.Hide()

			kill = ui.TextLine()
			kill.SetParent(self)
			kill.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			kill.SetHorizontalAlignCenter()
			kill.SetOutline()
			kill.SetPosition(405, 350 + 26*i)
			kill.Hide()

			dead = ui.TextLine()
			dead.SetParent(self)
			dead.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			dead.SetHorizontalAlignCenter()
			dead.SetOutline()
			dead.SetPosition(478, 350 + 26*i)
			dead.Hide()

			damage = ui.TextLine()
			damage.SetParent(self)
			damage.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			damage.SetHorizontalAlignCenter()
			damage.SetOutline()
			damage.SetPosition(556, 350 + 26*i)
			damage.Hide()

			self.enemyDataDict[i+9]["RACE"] = race
			self.enemyDataDict[i+9]["LEVEL"] = level
			self.enemyDataDict[i+9]["NAME"] = name
			self.enemyDataDict[i+9]["KILL"] = kill
			self.enemyDataDict[i+9]["DEAD"] = dead
			self.enemyDataDict[i+9]["DAMAGE"] = damage

	def Update(self, list, race, level, name, kill, dead, damage):
		self.enemyDataDict[list]["RACE"].LoadImage(self.FACE_IMAGE_DICT[race])
		if not self.enemyDataDict[list]["RACE"].IsShow():
			self.enemyDataDict[list]["RACE"].Show()

		self.enemyDataDict[list]["LEVEL"].SetText("%d" % (level))
		if not self.enemyDataDict[list]["LEVEL"].IsShow():
			self.enemyDataDict[list]["LEVEL"].Show()

		self.enemyDataDict[list]["NAME"].SetText("%s" % (name))
		if not self.enemyDataDict[list]["NAME"].IsShow():
			self.enemyDataDict[list]["NAME"].Show()

		self.enemyDataDict[list]["KILL"].SetText("%d" % (kill))
		if not self.enemyDataDict[list]["KILL"].IsShow():
			self.enemyDataDict[list]["KILL"].Show()

		self.enemyDataDict[list]["DEAD"].SetText("%d" % (dead))
		if not self.enemyDataDict[list]["DEAD"].IsShow():
			self.enemyDataDict[list]["DEAD"].Show()

		self.enemyDataDict[list]["DAMAGE"].SetText(localeInfo.NumberToString(damage))
		if not self.enemyDataDict[list]["DAMAGE"].IsShow():
			self.enemyDataDict[list]["DAMAGE"].Show()

	def UpdateScore(self, blueteamscore, redteamscore):
		self.blueteamscore.SetText("%d" % (blueteamscore))
		self.redteamscore.SetText("%d" % (redteamscore))