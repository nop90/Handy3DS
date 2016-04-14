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

#ifndef _HANDY_3DS_CONFIG_H_
#define _HANDY_3DS_CONFIG_H_

#define HANDY_3DS_LANGUAGE_STRING_MAX_SIZE 100

enum handy_3ds_string_hashes 
    { 
        HANDY_STR_Language_ID, 
        HANDY_STR_Wait,
        HANDY_STR_Pause,
        HANDY_STR_Exit_menu,
        HANDY_STR_Show_Menu,
        HANDY_STR_Back_to_Game,
        HANDY_STR_Back_to_Main,
        HANDY_STR_Back_to_Previous,
        HANDY_STR_Load_Rom,
        HANDY_STR_Load_State,
        HANDY_STR_Save_State,
        HANDY_STR_Reset_rom,
        HANDY_STR_Config,
        HANDY_STR_Exit,
        HANDY_STR_Screen_Config,
        HANDY_STR_Language,
        HANDY_STR_Sound,
        HANDY_STR_Limit_FPS,
        HANDY_STR_Swap_AB,
        HANDY_STR_Save_Config,
        HANDY_STR_Screen_Size,
        HANDY_STR_Bottom_Screen,
        HANDY_STR_Show_FPS,
        HANDY_STR_Show_Background,
        HANDY_STR_Background_Color,
        HANDY_STR_1x,
        HANDY_STR_2x,
        HANDY_STR_Fit,
        HANDY_STR_Stretch,
        HANDY_STR_Yes,
        HANDY_STR_No,
		HANDY_STR_Black,
		HANDY_STR_Blue,
		HANDY_STR_Green,
		HANDY_STR_Red
	};    



void set_language(void);

void do_config(char *file);
void parse_args(int argc, char **argv);
int parse_file(const char *filename, int *argc, char **argv);
void set_option_defaults(void);
void save_config(char *file);

#endif /* _HANDY_CONFIG_H_ */
