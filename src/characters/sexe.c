/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sexe.h"

#include "psx/mem.h"
#include "psx/archive.h"
#include "stage/stage.h"
#include "psx/random.h"
#include "psx/main.h"

//Sexe character structure
enum
{
    Sexe_ArcMain_Idle1,
    Sexe_ArcMain_Idle2,
    Sexe_ArcMain_Idle3,
    Sexe_ArcMain_Idle4,
    Sexe_ArcMain_Idle5,
    Sexe_ArcMain_Idle6,
    Sexe_ArcMain_Idle7,
    Sexe_ArcMain_Left1,
    Sexe_ArcMain_Left2,
    Sexe_ArcMain_Left3,
    Sexe_ArcMain_Left4,
    Sexe_ArcMain_Down1,
    Sexe_ArcMain_Down2,
    Sexe_ArcMain_Down3,
    Sexe_ArcMain_Down4,
    Sexe_ArcMain_Up1,
    Sexe_ArcMain_Up2,
    Sexe_ArcMain_Up3,
    Sexe_ArcMain_Up4,
    Sexe_ArcMain_Right1,
    Sexe_ArcMain_Right2,
    Sexe_ArcMain_Right3,
    Sexe_ArcMain_Right4,
    Sexe_ArcMain_Laugh1,
    Sexe_ArcMain_Laugh2,
    Sexe_ArcMain_Laugh3,
    Sexe_ArcMain_Laugh4,
    
    Sexe_Arc_Max,
};

typedef struct
{
    //Character base structure
    Character character;
    
    //Render data and state
    IO_Data arc_main;
    IO_Data arc_ptr[Sexe_Arc_Max];
    
    Gfx_Tex tex;
    u8 frame, tex_id;
} Char_Sexe;

//Sexe character definitions
static const CharFrame char_sexe_frame[] = {
    {Sexe_ArcMain_Idle1, {  0,   0, 148, 167}, { 42, 181}}, //0 idle 1
    {Sexe_ArcMain_Idle2, {  0,   0, 148, 167}, { 42, 181}}, //1 idle 2
    {Sexe_ArcMain_Idle3, {  0,   0, 148, 167}, { 42, 181}}, //2 idle 3
    {Sexe_ArcMain_Idle4, {  0,   0, 148, 167}, { 42, 181}}, //3 idle 4
    {Sexe_ArcMain_Idle5, {  0,   0, 148, 167}, { 42, 181}}, //4 idle 5
    {Sexe_ArcMain_Idle6, {  0,   0, 148, 167}, { 42, 181}}, //5 idle 6
    {Sexe_ArcMain_Idle7, {  0,   0, 148, 167}, { 42, 181}}, //6 idle 7
    
    //done offsets
    {Sexe_ArcMain_Left1, {  0,   0,  160, 174}, { 54+19, 188-2}}, //7 left 1
    {Sexe_ArcMain_Left2, {  0,   0,  160, 174}, { 54+19, 188-2}}, //8 left 2
    {Sexe_ArcMain_Left3, {  0,   0,  160, 174}, { 54+19, 188-2}}, //9 left 3
    {Sexe_ArcMain_Left4, {  0,   0,  160, 174}, { 54+19, 188-2}}, //10 left 4
    
    //done offsets
    {Sexe_ArcMain_Down1, {  0,   0, 160, 149}, { 54-4, 163+8}}, //11 down 1
    {Sexe_ArcMain_Down2, {  0,   0, 160, 149}, { 54-4, 163+8}}, //12 down 2
    {Sexe_ArcMain_Down3, {  0,   0, 160, 149}, { 54-4, 163+8}}, //13 down 3
    {Sexe_ArcMain_Down4, {  0,   0, 160, 149}, { 54-4, 163+8}}, //14 down 4
    
    
    {Sexe_ArcMain_Up1, {  0,   0, 172, 177}, { 66-6, 191+6}}, //15 up 1
    {Sexe_ArcMain_Up2, {  0,   0, 172, 177}, { 66-6, 191+6}}, //16 up 2
    {Sexe_ArcMain_Up3, {  0,   0, 172, 177}, { 66-6, 191+6}}, //17 up 3
    {Sexe_ArcMain_Up4, {  0,   0, 172, 177}, { 66-6, 191+6}}, //18 up 4
    
    {Sexe_ArcMain_Right1, {  0,   0, 132, 167}, { 26-3, 181+1}}, //19 right 1
    {Sexe_ArcMain_Right2, {  0,   0, 132, 167}, { 26-3, 181+1}}, //20 right 2
    {Sexe_ArcMain_Right3, {  0,   0, 132, 167}, { 26-3, 181+1}}, //21 right 3
    {Sexe_ArcMain_Right4, {  0,   0, 132, 167}, { 26-3, 181+1}}, //22 right 4
    
    {Sexe_ArcMain_Laugh1, {  0,   0, 172, 174}, { 66-16, 188+6}}, //23 laugh 1
    {Sexe_ArcMain_Laugh2, {  0,   0, 172, 174}, { 66-16, 188+6}}, //24 laugh 2
    {Sexe_ArcMain_Laugh3, {  0,   0, 172, 174}, { 66-16, 188+6}}, //25 laugh 3
    {Sexe_ArcMain_Laugh4, {  0,   0, 172, 174}, { 66-16, 188+6}}, //26 laugh 4
};

static const Animation char_sexe_anim[CharAnim_Max] = {
    {2, (const u8[]){ 0,  1,  2,  3, 4, 5, 6, ASCR_BACK, 1}}, //CharAnim_Idle
    {2, (const u8[]){ 7,  8, 9, 10, ASCR_BACK, 1}},         //CharAnim_Left
    {0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
    {2, (const u8[]){ 11, 12, 13, 14, ASCR_BACK, 1}},         //CharAnim_Down
    {2, (const u8[]){ 23, 24, 25, 26, ASCR_BACK, 1}},    //CharAnim_DownAlt
    {2, (const u8[]){ 15,  16, 17, 18, ASCR_BACK, 1}},         //CharAnim_Up
    {0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
    {2, (const u8[]){ 19, 20, 21, 22, ASCR_BACK, 1}},         //CharAnim_Right
    {0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Sexe character functions
void Char_Sexe_SetFrame(void *user, u8 frame)
{
    Char_Sexe *this = (Char_Sexe*)user;
    
    //Check if this is a new frame
    if (frame != this->frame)
    {
        //Check if new art shall be loaded
        const CharFrame *cframe = &char_sexe_frame[this->frame = frame];
        if (cframe->tex != this->tex_id)
            Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
    }
}

void Char_Sexe_Tick(Character *character)
{
    Char_Sexe *this = (Char_Sexe*)character;
    
    Character_CheckAnimationUpdate(character);
    
    if (stage.flag & STAGE_FLAG_JUST_STEP)
        Character_PerformIdle(character, 8);
    
    //Animate and draw
    Animatable_Animate(&character->animatable, (void*)this, Char_Sexe_SetFrame);
    Character_Draw(character, &this->tex, &char_sexe_frame[this->frame]);
}

void Char_Sexe_SetAnim(Character *character, u8 anim)
{
    //Set animation
    Animatable_SetAnim(&character->animatable, anim);
    Character_CheckStartSing(character);
}

void Char_Sexe_Free(Character *character)
{
    Char_Sexe *this = (Char_Sexe*)character;
    
    //Free art
    Mem_Free(this->arc_main);
}

Character *Char_Sexe_New(fixed_t x, fixed_t y)
{
    //Allocate sexe object
    Char_Sexe *this = Mem_Alloc(sizeof(Char_Sexe));
    if (this == NULL)
    {
        sprintf(error_msg, "[Char_Sexe_New] Failed to allocate sexe object");
        ErrorLock();
        return NULL;
    }
    
    //Initialize character
    this->character.tick = Char_Sexe_Tick;
    this->character.set_anim = Char_Sexe_SetAnim;
    this->character.free = Char_Sexe_Free;
    
    Animatable_Init(&this->character.animatable, char_sexe_anim);
    Character_Init((Character*)this, x, y);
    
    //Set character information
    this->character.spec = 0;
    
    //Health Icon
    this->character.health_i = 1;

    //Health Bar
    this->character.health_b = 0xFFAE67D0;

    //Character scale
    this->character.scale = FIXED_DEC(1,1);
    
    this->character.focus_x = FIXED_DEC(-35,1);
    this->character.focus_y = FIXED_DEC(-148,1);
    this->character.focus_zoom = FIXED_DEC(9,10);
    
    //Load art
    this->arc_main = IO_Read("\\CHAR\\SEXE.ARC;1");
    
    const char **pathp = (const char *[]){
        "idle1.tim", //Sexe_ArcMain_Idle1
        "idle2.tim", //Sexe_ArcMain_Idle2
        "idle3.tim", //Sexe_ArcMain_Idle3
        "idle4.tim", //Sexe_ArcMain_Idle4
        "idle5.tim", //Sexe_ArcMain_Idle5
        "idle6.tim", //Sexe_ArcMain_Idle6
        "idle7.tim", //Sexe_ArcMain_Idle7
        "left1.tim",  //Sexe_ArcMain_Left
        "left2.tim",  //Sexe_ArcMain_Left
        "left3.tim",  //Sexe_ArcMain_Left
        "left4.tim",  //Sexe_ArcMain_Left
        "down1.tim",  //Sexe_ArcMain_Down
        "down2.tim",  //Sexe_ArcMain_Down
        "down3.tim",  //Sexe_ArcMain_Down
        "down4.tim",  //Sexe_ArcMain_Down
        "up1.tim",    //Sexe_ArcMain_Up
        "up2.tim",    //Sexe_ArcMain_Up
        "up3.tim",    //Sexe_ArcMain_Up
        "up4.tim",    //Sexe_ArcMain_Up
        "right1.tim", //Sexe_ArcMain_Right
        "right2.tim", //Sexe_ArcMain_Right
        "right3.tim", //Sexe_ArcMain_Right
        "right4.tim", //Sexe_ArcMain_Right
        "laugh1.tim", //Sexe_ArcMain_Laugh
        "laugh2.tim", //Sexe_ArcMain_Laugh
        "laugh3.tim", //Sexe_ArcMain_Laugh
        "laugh4.tim", //Sexe_ArcMain_Laugh
        NULL
    };
    IO_Data *arc_ptr = this->arc_ptr;
    for (; *pathp != NULL; pathp++)
        *arc_ptr++ = Archive_Find(this->arc_main, *pathp);
    
    //Initialize render state
    this->tex_id = this->frame = 0xFF;
    
    return (Character*)this;
}
