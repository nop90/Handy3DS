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

#ifndef __HANDY_3DS_GRAPHICS_H__
#define __HANDY_3DS_GRAPHICS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


		void 	handy_3ds_video_init(void);
		void 	handy_3ds_video_quit(void);
		int 	handy_3ds_video_setup(int rendertype, int fsaa, int fullscreen, int bpp, int scale, int accel, int sysnc);
		void 	handy_3ds_video_reinit(int bpp);
		int		handy_3ds_video_early_setup(int surfacewidth, int surfaceheight, int bpp_flag, int videoflags);
		void 	handy_3ds_video_changeScaling();
		void 	handy_3ds_video_rotate();

#endif
