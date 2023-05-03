import uiScriptLocale
ROOT_PATH = "d:/ymir work/ui/public/"
window = {
	"name" : "TeamWarWindow",
	"x" : 360,
	"y" : 200,
	"style" : ("movable", "float", "animate",),
	"width" : 500,
	"height" : 150,
	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : 500,
			"height" : 150,
			"children" :
			(
				{
					"name" : "titlebar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 8,
					"y" : 8,
					"width" : 488,
					"color" : "gray",
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
							"x" : 488/2,
							"y" : 3,
							"text" : "Takým Savaþý",
							"text_horizontal_align":"center"
						},
					),
				},
				{
					"name" : "team_war_image",
					"type" : "image",

					"x" : 7,
					"y" : 30,

					"image" : "team_war.png",
				},
				{
					"name" : "team_war_image",
					"type" : "button",

					"x" : 25,
					"y" : 110,

					"text" : "Savaþ Oluþtur",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "create_team_war",
					"type" : "button",

					"x" : 25,
					"y" : 110,

					"text" : "Savaþ Oluþtur",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "join_team_war",
					"type" : "button",

					"x" : 145,
					"y" : 110,

					"text" : "Savaþ Bul",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "join_team_war_private",
					"type" : "button",

					"x" : 265,
					"y" : 110,

					"text" : "Özel Savaþa Katýl",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				{
					"name" : "join_team_war_observer",
					"type" : "button",

					"x" : 385,
					"y" : 110,

					"text" : "Savaþa Ýzleyici Olarak Katýl",

					"default_image" : ROOT_PATH + "large_Button_01.sub",
					"over_image" : ROOT_PATH + "large_Button_02.sub",
					"down_image" : ROOT_PATH + "large_Button_03.sub",
				},
				## Cancel
				{
					"name" : "cancel_button",
					"type" : "button",
					"x" : 0,
					"y" : 185,
					"text" : uiScriptLocale.CANCEL,
					"horizontal_align" : "center",
					"default_image" : ROOT_PATH + "Middle_Button_01.sub",
					"over_image" : ROOT_PATH + "Middle_Button_02.sub",
					"down_image" : ROOT_PATH + "Middle_Button_03.sub",
				},
			),
		},
	),
}
