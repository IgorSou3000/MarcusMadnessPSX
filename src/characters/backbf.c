/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "bf.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "stage/stage.h"
#include "psx/random.h"
#include "psx/main.h"

//Boyfriend player types
enum
{
  BackBF_ArcMain_Idle0,
  BackBF_ArcMain_Idle1,
  BackBF_ArcMain_Idle2,
  BackBF_ArcMain_Idle3,
  BackBF_ArcMain_Idle4,
  BackBF_ArcMain_Left0,
  BackBF_ArcMain_Down0,
  BackBF_ArcMain_Up0,
  BackBF_ArcMain_Right0,
	
	BF_ArcMain_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[BF_ArcMain_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_BF;

//Boyfriend player definitions
static const CharFrame char_bf_frame[] = {
  {BackBF_ArcMain_Idle0, {  0,  0,130,142}, {227,105}}, //0 idle 1
  {BackBF_ArcMain_Idle1, {  0,  0,131,138}, {228,101}}, //1 idle 2
  {BackBF_ArcMain_Idle2, {  0,  0,132,138}, {228,102}}, //2 idle 3
  {BackBF_ArcMain_Idle3, {  0,  0,131,140}, {228,104}}, //3 idle 4
  {BackBF_ArcMain_Idle4, {  0,  0,131,141}, {227,105}}, //4 idle 5

  {BackBF_ArcMain_Left0, {  0,  0,122,143}, {231,107}}, //5 left 1
  {BackBF_ArcMain_Left0, {122,  0,122,142}, {230,106}}, //6 left 2

  {BackBF_ArcMain_Down0, {  0,  0,122,138}, {228,102}}, //7 down 1
  {BackBF_ArcMain_Down0, {122,  0,121,139}, {229,103}}, //8 down 2

  {BackBF_ArcMain_Up0, {  0,  0,116,151}, {227,115}}, //9 up 1
  {BackBF_ArcMain_Up0, {116,  0,117,149}, {227,113}}, //10 up 2

  {BackBF_ArcMain_Right0, {  0,  0,121,140}, {225,105}}, //11 right 1
  {BackBF_ArcMain_Right0, {121,  0,121,141}, {226,106}}, //12 right 2
};

static const Animation char_bf_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 1}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt

	{2, (const u8[]){13, 14, 15, ASCR_BACK, 1}},         //CharAnim_Special1
	{2, (const u8[]){16, 17, 18, 19, ASCR_REPEAT}},      //CharAnim_Special2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Special3
	
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //PlayerAnim_LeftMiss
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //PlayerAnim_DownMiss
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //PlayerAnim_UpMiss
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //PlayerAnim_RightMiss
};

//Boyfriend player functions
void Char_BackBF_SetFrame(void *user, u8 frame)
{
	Char_BF *this = (Char_BF*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bf_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_BackBF_Tick(Character *character)
{
	Char_BF *this = (Char_BF*)character;
	
	Character_CheckAnimationUpdate(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
		Character_PerformIdle(character);
	
	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_BackBF_SetFrame);
	Character_Draw(character, &this->tex, &char_bf_frame[this->frame]);
}

void Char_BackBF_SetAnim(Character *character, u8 anim)
{
	Char_BF *this = (Char_BF*)character;
	
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_BackBF_Free(Character *character)
{
	Char_BF *this = (Char_BF*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_BackBF_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_BF *this = Mem_Alloc(sizeof(Char_BF));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_BackBF_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_BackBF_Tick;
	this->character.set_anim = Char_BackBF_SetAnim;
	this->character.free = Char_BackBF_Free;
	
	Animatable_Init(&this->character.animatable, char_bf_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = (CHAR_SPEC_MISSANIM | CHAR_SPEC_ISPLAYER);

	//Health Icon
	this->character.health_i = 0;

	//Health Bar
	this->character.health_b = 0xFF28B0D1;

	//Character scale
	this->character.scale = FIXED_DEC(1,1);
	
	this->character.focus_x = FIXED_DEC(-50,1);
	this->character.focus_y = FIXED_DEC(-65,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BACKBF.ARC;1");

	//Load gameover texture
	sprintf(stage.gameover_path, "\\CHAR\\BFDEAD.TIM;1");
	stage.gameover_tim = IO_Read(stage.gameover_path);
	
	const char **pathp = (const char *[]){
	  "idle0.tim",
	  "idle1.tim",
	  "idle2.tim",
	  "idle3.tim",
	  "idle4.tim",
	  "left0.tim",
	  "down0.tim",
	  "up0.tim",
	  "right0.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
