/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "marcus.h"

#include "psx/archive.h"
#include "psx/mem.h"
#include "stage/stage.h"

//Santiago background structure
typedef struct
{
	//Stage background base structure
	StageBack back;

	//Textures
	Gfx_Tex tex_back0; //Back part 1
	Gfx_Tex tex_back1; //Back part 2
	
} Back_Santiago;

//Stage functions
void Back_Santiago_Tick(StageBack *back)
{
	//Back_Santiago *this = (Back_Santiago*)back;
	(void)back; //Only for remove warning
}

void Back_Santiago_DrawBG(StageBack* back)
{
	Back_Santiago* this = (Back_Santiago*)back;

	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;
	
	//Draw Background
	RECT back_src = {0, 0, 255, 255};
	RECT_FIXED back_dst = {
		FIXED_DEC(-102,1) - fx,
		FIXED_DEC(-155,1) - fy,
		FIXED_DEC(416,1),
		FIXED_DEC(312,1)
	};

	Debug_MoveTexture(&back_dst, 0, "back", fx, fy);
	
	Stage_DrawTex(&this->tex_back0, &back_src, &back_dst, stage.camera.bzoom);
}

void Back_Santiago_Free(StageBack *back)
{
	Back_Santiago *this = (Back_Santiago*)back;
}

StageBack *Back_Santiago_New(void)
{
	//Allocate background structure
	Back_Santiago *this = (Back_Santiago*)Mem_Alloc(sizeof(Back_Santiago));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.tick = Back_Santiago_Tick;
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Santiago_DrawBG;
	this->back.free = Back_Santiago_Free;

	//Load background textures
	IO_Data arc_back = IO_Read("\\SANTIBG\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
