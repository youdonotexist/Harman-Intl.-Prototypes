#ifndef CONSTANTS_H
#define CONSTANTS_H

#endif // CONSTANTS_H

#define COLOR_TEXT_RED			(0xff0000)

#define COLOR_RED				(0xff3300)
#define COLOR_YELLOW			(0xffd200)
#define COLOR_DARK_GREEN		(0x339900)
#define COLOR_BLUE				(0x0000ff)
#define COLOR_ORANGE			(0xff6600)
#define COLOR_PURPLE			(0x6600ff)
#define COLOR_CYAN				(0x00e9ff)
#define COLOR_GREEN				(0x00ff12)

#define COLOR_GRAPH_RED				(0xf34444)
#define COLOR_GRAPH_LIGHT_YELLOW	(0xf7f792)
#define COLOR_GRAPH_LIGHT_GREEN		(0x83f683)
#define COLOR_GRAPH_LAVENDER		(0xcf8eea)
#define COLOR_GRAPH_ORANGE			(0xff861e)
#define COLOR_GRAPH_LIGHT_BLUE		(0x00e9ff)
#define COLOR_GRAPH_GREEN			(0x339900)
#define COLOR_GRAPH_DARK_BLUE		(0x0064ff)
#define COLOR_GRAPH_DARK_MAGENTA	(0xb72467)
#define COLOR_GRAPH_PURPLE			(0x7f00ff)
#define COLOR_GRAPH_YELLOW			(0xffe800)
#define COLOR_GRAPH_PEACHISH		(0xff8080)

#define COLOR_1		COLOR_GRAPH_RED
#define COLOR_2     COLOR_GRAPH_LIGHT_YELLOW
#define COLOR_3     COLOR_GRAPH_LIGHT_GREEN
#define COLOR_4     COLOR_GRAPH_LAVENDER
#define COLOR_5     COLOR_GRAPH_ORANGE
#define COLOR_6     COLOR_GRAPH_LIGHT_BLUE
#define COLOR_7     COLOR_GRAPH_GREEN
#define COLOR_8     COLOR_GRAPH_DARK_BLUE
#define COLOR_9		COLOR_GRAPH_DARK_MAGENTA
#define COLOR_10	COLOR_GRAPH_PURPLE
#define COLOR_11	COLOR_GRAPH_YELLOW
#define COLOR_12	COLOR_GRAPH_PEACHISH

#define COLOR_INT_COMP(c)		(((c)>>16)&0xff),(((c)>>8)&0xff),(((c))&0xff)
#define COLOR_FLOAT_COMP(c)		((float)(((c)>>16)&0xff)/255.0f),((float)(((c)>>8)&0xff)/255.0f),((float)(((c))&0xff)/255.0f)
