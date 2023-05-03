import uiScriptLocale

window = {
	"name" : "TeamWarDurationWindow",
	"style" : ("movable", "float", "animate",),

	"x" : 0,
	"y" : 60,

	"width" : 230,
	"height" : 50,

	"children" :
	(
		{
			"name" : "Thinboard",
			"type" : "thinboard",

			"x" : 0,
			"y" : 0,
			"width" : 230,
			"height" : 50,

			"children" :
			(
				{
					"name" : "teamwartitle",
					"type" : "text",

					"x" : 0,
					"y" : 17,

					"color" : 0xfff8d090,

					"text" : "Takým Savaþý",

					"horizontal_align" : "center",
					"text_horizontal_align" : "center",
					"text_vertical_align" : "center",
				},
				{
					"name" : "teamwartitleimage",
					"type" : "image",

					"x" : 0,
					"y" : 2,

					"image" : "d:/ymir work/ui/center.tga",

					"horizontal_align" : "center",
					"text_horizontal_align" : "center",
					"text_vertical_align" : "center",
				},
				{
					"name" : "message1",
					"type" : "text",

					"x" : 0,
					"y" : 36,

					"text" : uiScriptLocale.MESSAGE,

					"horizontal_align" : "center",
					"text_horizontal_align" : "center",
					"text_vertical_align" : "center",
				},
			),
		},
	),
}