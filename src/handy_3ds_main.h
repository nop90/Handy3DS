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

#ifndef __HANDY_3DS_MAIN_H__
#define __HANDY_3DS_MAIN_H__

#include "system.h"
#include "pixblend.h"
#include "errorhandler.h"

#include "3dsemu_system.h"



#define		HANDY_3DS_VERSION	"v1.1"
#define     HANDY_VERSION           	"Handy 0.95"
#define     HANDY_BASE_FPS              60

typedef int16_t Sint16;
typedef uint64_t Uint64;

/* Handy declarations */
extern Uint32			*mpLynxBuffer;
extern CSystem 		    *mpLynx;
extern int				 mFrameSkip;
extern int				 mpBpp;

/* Handy 3DS declarations */
extern int			 	LynxWidth;				// Lynx screen width
extern int			 	LynxHeight;      		// Lynx screen height
extern int 		 	    LynxFormat;				// Lynx ROM format type
extern int 		 	    LynxRotate;				// Lynx ROM rotation type
extern int				LynxScale;				// Scale output
extern int				LynxLCD;                // LCD/Scanline Output
extern int				emulation;              // Emulation enabled
extern int				rendertype;             // Rendertype
extern int				stype;					// Scaling/Scanline Routine
extern int              filter;					// Output Filter
extern Uint32           overlay_format;         // YUV Overlay format
extern Uint8  		   *delta;

extern int              Autoskip;     // Autoskip

inline	int 	handy_3ds_update(void);
		void 	handy_3ds_rom_info(void);
		int 	main();
		void	handy_3ds_quit(void);
		void	handy_3ds_core_reinit(char *romname);
#endif
