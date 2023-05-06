/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "santiago.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "stage/stage.h"
#include "psx/main.h"

//Santiago character structure
enum
{
  Santiago_ArcMain_Santiago0,
  Santiago_ArcMain_Santiago1,
  Santiago_ArcMain_Santiago2,
  Santiago_ArcMain_Santiago3,
  Santiago_ArcMain_Santiago4,
  Santiago_ArcMain_Santiago5,
  Santiago_ArcMain_Santiago6,
  Santiago_ArcMain_Santiago7,
  Santiago_ArcMain_Santiago8,
	
	Santiago_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Santiago_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Santiago;

//Santiago character definitions
static const CharFrame char_santiago_frame[] = {

  {Santiago_ArcMain_Santiago0, {  0,  0, 93,149}, {208,186}}, //0 idle 1
  {Santiago_ArcMain_Santiago0, { 93,  0, 92,149}, {207,186}}, //1 idle 2
  {Santiago_ArcMain_Santiago1, {  0,  0, 91,146}, {205,183}}, //2 idle 3
  {Santiago_ArcMain_Santiago1, { 91,  0, 91,147}, {206,184}}, //3 idle 4
  {Santiago_ArcMain_Santiago2, {  0,  0,131,149}, {206,185}}, //4 idle 5

  {Santiago_ArcMain_Santiago3, {  0,  0,130,135}, {223,174}}, //5 left 1
  {Santiago_ArcMain_Santiago4, {  0,  0,129,135}, {222,174}}, //6 left 2

  {Santiago_ArcMain_Santiago5, {  0,  0,140,124}, {222,161}}, //7 down 1
  {Santiago_ArcMain_Santiago5, {  0,124,138,126}, {221,163}}, //8 down 2
  {Santiago_ArcMain_Santiago6, {  0,  0,135,127}, {220,164}}, //9 down 3

  {Santiago_ArcMain_Santiago6, {135,  0,108,153}, {211,190}}, //10 up 1
  {Santiago_ArcMain_Santiago7, {  0,  0,110,151}, {213,188}}, //11 up 2

  {Santiago_ArcMain_Santiago7, {110,  0,120,142}, {196,181}}, //12 right 1
  {Santiago_ArcMain_Santiago8, {  0,  0,120,142}, {196,181}}, //13 right 2
};

static const Animation char_santiago_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8,  9, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){12, 13, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt

	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
};

//Santiago character functions
void Char_Santiago_SetFrame(void *user, u8 frame)
{
	Char_Santiago *this = (Char_Santiago*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_santiago_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Santiago_Tick(Character *character)
{
	Char_Santiago *this = (Char_Santiago*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character, 8);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Santiago_SetFrame);
	Character_Draw(character, &this->tex, &char_santiago_frame[this->frame]);
}

void Char_Santiago_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Santiago_Free(Character *character)
{
	Char_Santiago *this = (Char_Santiago*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Santiago_New(fixed_t x, fixed_t y)
{
	//Allocate santiago object
	Char_Santiago *this = Mem_Alloc(sizeof(Char_Santiago));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Santiago_New] Failed to allocate santiago object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Santiago_Tick;
	this->character.set_anim = Char_Santiago_SetAnim;
	this->character.free = Char_Santiago_Free;
	
	Animatable_Init(&this->character.animatable, char_santiago_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	//Health Icon
	this->character.health_i = 1;

	//Health Bar
	this->character.health_b = 0xFFAE67D0;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(-75,1);
	this->character.focus_y = FIXED_DEC(-148,1);
	this->character.focus_zoom = FIXED_DEC(9,10);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SANTI.ARC;1");
	
	const char **pathp = (const char *[]){
	  "san0.tim",
	  "san1.tim",
	  "san2.tim",
	  "san3.tim",
	  "san4.tim",
	  "san5.tim",
	  "san6.tim",
	  "san7.tim",
	  "san8.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
