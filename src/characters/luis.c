/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "luis.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "stage/stage.h"
#include "psx/main.h"

//Luis character structure
enum
{
  Luis_ArcMain_Idle0,
  Luis_ArcMain_Idle1,
  Luis_ArcMain_Idle2,
  Luis_ArcMain_Idle3,
  Luis_ArcMain_Left0,
  Luis_ArcMain_Left1,
  Luis_ArcMain_Left2,
  Luis_ArcMain_Left3,
  Luis_ArcMain_Down0,
  Luis_ArcMain_Down1,
  Luis_ArcMain_Down2,
  Luis_ArcMain_Up0,
  Luis_ArcMain_Up1,
  Luis_ArcMain_Up2,
  Luis_ArcMain_Up3,
  Luis_ArcMain_Right0,
  Luis_ArcMain_Right1,
  Luis_ArcMain_Right2,
	
	Luis_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Luis_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Luis;

//Luis character definitions
static const CharFrame char_luis_frame[] = {
  {Luis_ArcMain_Idle0, {  0,  0,101,201}, {118,218}}, //0 idle 1
  {Luis_ArcMain_Idle0, {101,  0,101,201}, {118,217}}, //1 idle 2
  {Luis_ArcMain_Idle1, {  0,  0,101,199}, {117,217}}, //2 idle 3
  {Luis_ArcMain_Idle1, {101,  0,107,195}, {124,212}}, //3 idle 4
  {Luis_ArcMain_Idle2, {  0,  0,107,195}, {125,212}}, //4 idle 5
  {Luis_ArcMain_Idle2, {107,  0,104,198}, {120,215}}, //5 idle 6
  {Luis_ArcMain_Idle3, {  0,  0,102,200}, {118,217}}, //6 idle 7

  {Luis_ArcMain_Left0, {  0,  0,163,201}, {160,218}}, //7 left 1
  {Luis_ArcMain_Left1, {  0,  0,163,201}, {159,218}}, //8 left 2
  {Luis_ArcMain_Left2, {  0,  0,162,201}, {159,218}}, //9 left 3
  {Luis_ArcMain_Left3, {  0,  0,163,201}, {159,218}}, //10 left 4

  {Luis_ArcMain_Down0, {  0,  0,131,177}, {129,196}}, //11 down 1
  {Luis_ArcMain_Down1, {  0,  0,129,179}, {128,198}}, //12 down 2
  {Luis_ArcMain_Down2, {  0,  0,128,180}, {128,198}}, //13 down 3
  {Luis_ArcMain_Down2, {128,  0,127,180}, {128,199}}, //14 down 4

  {Luis_ArcMain_Up0, {  0,  0,129,208}, {132,225}}, //15 up 1
  {Luis_ArcMain_Up1, {  0,  0,130,206}, {132,224}}, //16 up 2
  {Luis_ArcMain_Up2, {  0,  0,129,206}, {132,223}}, //17 up 3
  {Luis_ArcMain_Up3, {  0,  0,130,206}, {132,223}}, //18 up 4

  {Luis_ArcMain_Right0, {  0,  0,129,200}, {108,217}}, //19 right 1
  {Luis_ArcMain_Right1, {  0,  0,129,201}, {109,219}}, //20 right 2
  {Luis_ArcMain_Right2, {  0,  0,128,202}, {109,219}}, //21 right 3
  {Luis_ArcMain_Right2, {128,  0,127,202}, {109,220}}, //22 right 4
};

static const Animation char_luis_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5,  6, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 7,  8,  9, 10, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){11, 12, 13, 14, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){15, 16, 17, 18, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){19, 20, 21, 22, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt

	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
};

//Luis character functions
void Char_Luis_SetFrame(void *user, u8 frame)
{
	Char_Luis *this = (Char_Luis*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_luis_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Luis_Tick(Character *character)
{
	Char_Luis *this = (Char_Luis*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character, 8);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Luis_SetFrame);
	Character_Draw(character, &this->tex, &char_luis_frame[this->frame]);
}

void Char_Luis_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Luis_Free(Character *character)
{
	Char_Luis *this = (Char_Luis*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Luis_New(fixed_t x, fixed_t y)
{
	//Allocate luis object
	Char_Luis *this = Mem_Alloc(sizeof(Char_Luis));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Luis_New] Failed to allocate luis object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Luis_Tick;
	this->character.set_anim = Char_Luis_SetAnim;
	this->character.free = Char_Luis_Free;
	
	Animatable_Init(&this->character.animatable, char_luis_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	//Health Icon
	this->character.health_i = 1;

	//Health Bar
	this->character.health_b = 0xFFAE67D0;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(61 - 222,1);
	this->character.focus_y = FIXED_DEC(-36 - 113,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\LUIS.ARC;1");
	
	const char **pathp = (const char *[]){
	  "idle0.tim",
	  "idle1.tim",
	  "idle2.tim",
	  "idle3.tim",
	  "left0.tim",
	  "left1.tim",
	  "left2.tim",
	  "left3.tim",
	  "down0.tim",
	  "down1.tim",
	  "down2.tim",
	  "up0.tim",
	  "up1.tim",
	  "up2.tim",
	  "up3.tim",
	  "right0.tim",
	  "right1.tim",
	  "right2.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
