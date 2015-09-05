/////////////////////////////////////////////////////////////////////////////////
//                       Handy 3DS - An Atari Lynx Emulator                    //
//							Ported to 3DS by nop90 - 2015                      //
//                    Everything not covered by the following                  //
//                          Copyrights is Public Domain                        //
//                                                                             //
//                          Based upon Handy/SDL v0.5                          //
//                             Copyright (c) 2005                              //
//                                SDLemu Team                                  //
//                          Based upon Handy v0.95 WIN32                       //
//                            Copyright (c) 1996,1997                          //
//                                  K. Wilkins                                 //
/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
// Copyright (c) 2004 SDLemu Team                                              //
//                                                                             //
// This software is provided 'as-is', without any express or implied warranty. //
// In no event will the authors be held liable for any damages arising from    //
// the use of this software.                                                   //
//                                                                             //
// Permission is granted to anyone to use this software for any purpose,       //
// including commercial applications, and to alter it and redistribute it      //
// freely, subject to the following restrictions:                              //
//                                                                             //
// 1. The origin of this software must not be misrepresented; you must not     //
//    claim that you wrote the original software. If you use this software     //
//    in a product, an acknowledgment in the product documentation would be    //
//    appreciated but is not required.                                         //
//                                                                             //
// 2. Altered source versions must be plainly marked as such, and must not     //
//    be misrepresented as being the original software.                        //
//                                                                             //
// 3. This notice may not be removed or altered from any source distribution.  //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>

#include "handy_3ds_main.h"
#include "handy_3ds_graphics.h"
#include "handy_3ds_handling.h"

int BT_A = KEY_A;
int BT_B = KEY_B;

int BT_LEFT = KEY_LEFT;
int BT_RIGHT = KEY_RIGHT;
int BT_UP = KEY_UP;
int BT_DOWN = KEY_DOWN;

    #define BT_PAUSE    KEY_START       // START
    #define BT_OPT1     KEY_Y           // Y
    #define BT_OPT2     KEY_X           // X

bool LHeld = false, RHeld = false;
extern char savename[512];

int  handy_3ds_on_key_down(u32 key, int mask)
{

        if (key & BT_LEFT)  // Lynx LEFT
            mask|=BUTTON_LEFT;
        else mask&= ~BUTTON_LEFT;

        if (key & BT_RIGHT) // Lynx RIGHT
            mask|=BUTTON_RIGHT;
        else mask&= ~BUTTON_RIGHT;

        if (key & BT_UP) // Lynx UP
            mask|=BUTTON_UP;
        else mask&= ~BUTTON_UP;

        if (key & BT_DOWN) // Lynx DOWN
            mask|=BUTTON_DOWN;
        else mask&= ~BUTTON_DOWN;

        if (key & BT_PAUSE) // Lynx PAUSE
            mask|=BUTTON_PAUSE;
        else mask&= ~BUTTON_PAUSE;

        if (key & BT_B) // Lynx B
            mask|=BUTTON_B;
        else mask&= ~BUTTON_B;

        if (key & BT_A) // Lynx A
            mask|=BUTTON_A;
        else mask&= ~BUTTON_A;

        if (key & BT_OPT1) // Lynx Option 1
            mask|=BUTTON_OPT1;
        else mask&= ~BUTTON_OPT1;

        if (key & BT_OPT2) // Lynx Option 2
            mask|=BUTTON_OPT2;
        else mask&= ~BUTTON_OPT2;
    
    if (key & KEY_L) {
        LHeld = true;
    } else {
        if (LHeld == true)
            handy_3ds_video_changeScaling();
        LHeld = false;
    }

    if (key & KEY_R) {
        RHeld = true;
    } else {
        if (RHeld == true)
            handy_3ds_video_rotate();
        RHeld = false;
    }
    return mask;

}

int  handy_3ds_on_key_up(u32 key, int mask)
{
        if (key & BT_LEFT)  // Lynx LEFT
            mask&= ~BUTTON_LEFT;

        if (key & BT_RIGHT) // Lynx RIGHT
            mask&= ~BUTTON_RIGHT;
        
        if (key & BT_UP) // Lynx UP
            mask&= ~BUTTON_UP;
        
        if (key & BT_DOWN) // Lynx DOWN
            mask&= ~BUTTON_DOWN;
        
        if (key & BT_PAUSE) // Lynx PAUSE
            mask&= ~BUTTON_PAUSE;
        
        if (key & BT_B) // Lynx B
            mask&= ~BUTTON_B;
        
        if (key & BT_A) // Lynx A
           mask&= ~BUTTON_A;
        
        if (key & BT_OPT1)  // Lynx Option1
            mask&= ~BUTTON_OPT1;

        if (key & BT_OPT2)  // Lynx Option2
            mask&= ~BUTTON_OPT2;

    return mask;
}
