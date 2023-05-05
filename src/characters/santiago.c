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
  Santiago_ArcMain_Santiago9,
  Santiago_ArcMain_Santiago10,
  Santiago_ArcMain_Santiago11,
  Santiago_ArcMain_Santiago12,
	
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
  {Santiago_ArcMain_Santiago0, {  0,  0,148,238}, {236,203}}, //0 idle 1
  {Santiago_ArcMain_Santiago1, {  0,  0,147,237}, {235,202}}, //1 idle 2
  {Santiago_ArcMain_Santiago2, {  0,  0,144,233}, {231,198}}, //2 idle 3
  {Santiago_ArcMain_Santiago3, {  0,  0,145,234}, {232,199}}, //3 idle 4
  {Santiago_ArcMain_Santiago4, {  0,  0,145,238}, {233,202}}, //4 idle 5

  {Santiago_ArcMain_Santiago5, {  0,  0,207,215}, {260,183}}, //5 left 1
  {Santiago_ArcMain_Santiago6, {  0,  0,206,215}, {259,183}}, //6 left 2

  {Santiago_ArcMain_Santiago7, {  0,  0,224,198}, {259,163}}, //7 down 1
  {Santiago_ArcMain_Santiago8, {  0,  0,220,201}, {257,166}}, //8 down 2

  {Santiago_ArcMain_Santiago9, {  0,  0,173,244}, {241,209}}, //9 up 1
  {Santiago_ArcMain_Santiago10, {  0,  0,176,242}, {244,207}}, //10 up 2

  {Santiago_ArcMain_Santiago11, {  0,  0,191,227}, {217,194}}, //11 right 1
  {Santiago_ArcMain_Santiago12, {  0,  0,191,227}, {217,194}}, //12 right 2
};

static const Animation char_santiago_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},         //CharAnim_Right
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
		Character_PerformIdle(character);
	
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
	this->character.scale = FIXED_DEC(8,10);
	
	this->character.focus_x = FIXED_DEC(5,1);
	this->character.focus_y = FIXED_DEC(-35,1);
	this->character.focus_zoom = FIXED_DEC(8,10);
	
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
	  "san9.tim",
	  "san10.tim",
	  "san11.tim",
	  "san12.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
