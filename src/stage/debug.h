/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_DEBUG_H
#define PSXF_GUARD_DEBUG_H

#include "psx/io.h"
#include "psx/gfx.h"
#include "psx/fixed.h"

#define DEBUG_POSITION_SIZE 5

//#define DEBUG_MODE

#ifdef DEBUG_MODE
//Debug definitions
typedef struct
{
	u8 mode, next_mode;
	u8 select;
	RECT_FIXED ogpositions[DEBUG_POSITION_SIZE];
	RECT_FIXED positions[DEBUG_POSITION_SIZE];
	char tex_names[15][DEBUG_POSITION_SIZE];
} Debug;

extern Debug debug;
#endif

void Debug_Load(void);
void Debug_MoveTexture(RECT_FIXED* src, u8 select, const char* name, fixed_t camera_x, fixed_t camera_y);
void Debug_Tick(void);

#endif
