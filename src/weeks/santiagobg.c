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
	
} Back_Santiago;

//Stage functions
void Back_Santiago_Tick(StageBack *back)
{
	//Back_Santiago *this = (Back_Santiago*)back;
	(void)back; //Only for remove warning
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
	this->back.draw_bg = NULL;
	this->back.free = Back_Santiago_Free;

	Gfx_SetClear(23, 21, 30);
	
	return (StageBack*)this;
}
