/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "marcus.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "stage/stage.h"
#include "psx/main.h"

//Marcus character structure
enum
{
 	Marcus_ArcMain_Idle0,
  Marcus_ArcMain_Idle1,
  Marcus_ArcMain_Idle2,
  Marcus_ArcMain_Left0,
  Marcus_ArcMain_Left1,
  Marcus_ArcMain_Left2,
  Marcus_ArcMain_Left3,
  Marcus_ArcMain_Down0,
  Marcus_ArcMain_Down1,
  Marcus_ArcMain_Up0,
  Marcus_ArcMain_Up1,
  Marcus_ArcMain_Right0,
  Marcus_ArcMain_Right1,
  Marcus_ArcMain_Right2,
  Marcus_ArcMain_Right3,
	
	Marcus_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Marcus_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Marcus;

//Marcus character definitions
static const CharFrame char_marcus_frame[] = {
  {Marcus_ArcMain_Idle0, {  0,  0,100,173}, {166,  -127}}, //0 idle 1
  {Marcus_ArcMain_Idle0, {100,  0,101,169}, {163,  -131}}, //1 idle 2
  {Marcus_ArcMain_Idle1, {  0,  0,101,170}, {162,  -130}}, //2 idle 3
  {Marcus_ArcMain_Idle1, {101,  0,101,171}, {164,  -129}}, //3 idle 4
  {Marcus_ArcMain_Idle2, {  0,  0,100,173}, {166,  -128}}, //4 idle 5

  {Marcus_ArcMain_Left0, {  0,  0,129,163}, {166,  -138}}, //5 left 1
  {Marcus_ArcMain_Left1, {  0,  0,127,163}, {162,  -138}}, //6 left 2
  {Marcus_ArcMain_Left2, {  0,  0,129,163}, {162,  -138}}, //7 left 3
  {Marcus_ArcMain_Left3, {  0,  0,129,163}, {162,  -138}}, //8 left 4

  {Marcus_ArcMain_Down0, {  0,  0,113,149}, {160,  -152}}, //9 down 1
  {Marcus_ArcMain_Down0, {113,  0,113,152}, {161,  -149}}, //10 down 2
  {Marcus_ArcMain_Down1, {  0,  0,113,153}, {160,  -148}}, //11 down 3
  {Marcus_ArcMain_Down1, {113,  0,112,153}, {160,  -148}}, //12 down 4

  {Marcus_ArcMain_Up0, {  0,  0, 94,173}, {165,  -128}}, //13 up 1
  {Marcus_ArcMain_Up0, { 94,  0, 95,169}, {163,  -131}}, //14 up 2
  {Marcus_ArcMain_Up1, {  0,  0, 95,168}, {163,  -132}}, //15 up 3
  {Marcus_ArcMain_Up1, { 95,  0, 95,168}, {163,  -132}}, //16 up 4

  {Marcus_ArcMain_Right0, {  0,  0,175,159}, {151,  -142}}, //17 right 1
  {Marcus_ArcMain_Right1, {  0,  0,179,158}, {152,  -142}}, //18 right 2
  {Marcus_ArcMain_Right2, {  0,  0,179,158}, {152,  -143}}, //19 right 3
  {Marcus_ArcMain_Right3, {  0,  0,179,158}, {153,  -143}}, //20 right 4
};

static const Animation char_marcus_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6,  7,  8, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 9, 10, 11, 12, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){13, 14, 15, 16, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){17, 18, 19, 20, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt

	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
};

//Marcus character functions
void Char_Marcus_SetFrame(void *user, u8 frame)
{
	Char_Marcus *this = (Char_Marcus*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_marcus_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Marcus_Tick(Character *character)
{
	Char_Marcus *this = (Char_Marcus*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character, 8);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Marcus_SetFrame);
	Character_Draw(character, &this->tex, &char_marcus_frame[this->frame]);
}

void Char_Marcus_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Marcus_Free(Character *character)
{
	Char_Marcus *this = (Char_Marcus*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Marcus_New(fixed_t x, fixed_t y)
{
	//Allocate marcus object
	Char_Marcus *this = Mem_Alloc(sizeof(Char_Marcus));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Marcus_New] Failed to allocate marcus object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Marcus_Tick;
	this->character.set_anim = Char_Marcus_SetAnim;
	this->character.free = Char_Marcus_Free;
	
	Animatable_Init(&this->character.animatable, char_marcus_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	//Health Icon
	this->character.health_i = 1;

	//Health Bar
	this->character.health_b = 0xFFAE67D0;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(5,1);
	this->character.focus_y = FIXED_DEC(169,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MARCUS.ARC;1");
	
	const char **pathp = (const char *[]){
	  "idle0.tim",
	  "idle1.tim",
	  "idle2.tim",
	  "left0.tim",
	  "left1.tim",
	  "left2.tim",
	  "left3.tim",
	  "down0.tim",
	  "down1.tim",
	  "up0.tim",
	  "up1.tim",
	  "right0.tim",
	  "right1.tim",
	  "right2.tim",
	  "right3.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
