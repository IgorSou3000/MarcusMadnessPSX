#pragma once

#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11295)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3840)},  //XA_GameOver
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(12200)}, //XA_Halluciogens
	{XA_Week1A, XA_LENGTH(11700)}, //XA_Mansion
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(11900)}, //XA_Mushroom
	//WEEK2A.XA
	{XA_Week2A, XA_LENGTH(8100)}, //XA_Shining_Blue
	{XA_Week2A, XA_LENGTH(12200)}, //XA_Santiagonist
	//WEEK2B.XA
	{XA_Week2B, XA_LENGTH(9800)}, //XA_Rat_Trap
	{XA_Week2B, XA_LENGTH(12000)}, //XA_Blue_Blood
};

static const char *xa_paths[] = {
	"\\SONGS\\MENU.MUS;1",   //XA_Menu
	"\\SONGS\\WEEK1A.MUS;1", //XA_Week1A
	"\\SONGS\\WEEK1B.MUS;1", //XA_Week1B
	"\\SONGS\\WEEK2A.MUS;1", //XA_Week2A
	"\\SONGS\\WEEK2B.MUS;1", //XA_Week2B
	NULL,
};
