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

#ifndef _3DSEMU_SYSTEM_H
#define _3DSEMU_SYSTEM_H

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char  uint8;
typedef unsigned char  Uint8;
typedef unsigned short uint16;
typedef unsigned short Uint16;
typedef unsigned long  uint32;
typedef unsigned long  Uint32;


typedef unsigned char  u8;
typedef unsigned short u16;
//typedef unsigned int   u32;

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

typedef signed char s8;
typedef signed short s16;
//typedef signed int s32;

#ifdef _MSC_VER
typedef signed __int64 s64;
#else
typedef signed long long s64;
#endif

#ifndef boolean
    #define boolean int
#endif
#ifndef bool
    #define bool    int
#endif

#ifndef FALSE
	#define FALSE 0
#endif
#ifndef TRUE
	#define TRUE  1
#endif


#endif //_3DSEMU_SYSTEM_H
