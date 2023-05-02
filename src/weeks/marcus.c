/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "marcus.h"

#include "psx/archive.h"
#include "psx/mem.h"
#include "stage/stage.h"

//Marcus background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
} Back_Marcus;

//Stage functions
void Back_Marcus_Tick(StageBack *back)
{
	//Back_Marcus *this = (Back_Marcus*)back;
	(void)back; //Only for remove warning
}

void Back_Marcus_Free(StageBack *back)
{
	Back_Marcus *this = (Back_Marcus*)back;
}

StageBack *Back_Marcus_New(void)
{
	//Allocate background structure
	Back_Marcus *this = (Back_Marcus*)Mem_Alloc(sizeof(Back_Marcus));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.tick = Back_Marcus_Tick;
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = NULL;
	this->back.free = Back_Marcus_Free;

	Gfx_SetClear(23, 21, 30);
	
	return (StageBack*)this;
}
