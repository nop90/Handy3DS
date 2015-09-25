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
#include "handy_3ds_gui.h"
#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>
#include "Hack_ttf.h"

sftd_font *font;

sf2d_texture *screentex1, *screentex2, *currentscreen, *prevscreen, *previewtex, *background, *emptyslot, *insertgame; 

float stretchx, stretchy, stretchr, bgstretchx, bgstretchy;
int Handy_rotation = 0;

extern int Handy_cfg_scalemode;
extern int Handy_cfg_ShowBackground;
extern int Handy_cfg_BottomScr;
extern int Handy_cfg_BGColor;
extern char config_skin_path[];
extern int BT_LEFT, BT_RIGHT, BT_UP, BT_DOWN;
extern float fps_counter;

void handy_3ds_video_changeScaling() {
    Handy_cfg_scalemode++;
    if (Handy_cfg_scalemode == 4) Handy_cfg_scalemode = 0;
	
	if(Handy_rotation && Handy_cfg_scalemode) Handy_cfg_scalemode = 3;
}

void handy_3ds_video_rotate() {
    Handy_rotation++;
    if (Handy_rotation == 4) Handy_rotation = 0;
    if (Handy_cfg_scalemode) Handy_cfg_scalemode = 3;
	
	switch (Handy_rotation) {
		case 3:
			BT_LEFT = KEY_DOWN;
			BT_RIGHT = KEY_UP;
			BT_UP = KEY_LEFT;
			BT_DOWN = KEY_RIGHT;
			break;
		case 2:
			BT_LEFT = KEY_RIGHT;
			BT_RIGHT = KEY_LEFT;
			BT_UP = KEY_DOWN;
			BT_DOWN = KEY_UP;
			break;
		case 1:
			BT_LEFT = KEY_UP;
			BT_RIGHT = KEY_DOWN;
			BT_UP = KEY_RIGHT;
			BT_DOWN = KEY_LEFT;
			break;
		case 0:
			BT_LEFT = KEY_LEFT;
			BT_RIGHT = KEY_RIGHT;
			BT_UP = KEY_UP;
			BT_DOWN = KEY_DOWN;
		default:
		break;
	}
}

void handy_3ds_video_init(void)
{
    sf2d_init();

    switch (Handy_cfg_BGColor) {
		case 0:
		    sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
			break;
		case 1:
		    sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x44, 0xFF));
			break;
		case 2:
		    sf2d_set_clear_color(RGBA8(0x00, 0x44, 0x00, 0xFF));
			break;
		case 3:
		    sf2d_set_clear_color(RGBA8(0x44, 0x00, 0x00, 0xFF));
			break;
	}
	
//    sf2d_set_vblank_wait(false);
    sf2d_set_vblank_wait(true);
    sftd_init();
    font = sftd_load_font_mem(Hack_ttf, Hack_ttf_size);

    screentex1 = sf2d_create_texture(160, 102, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    screentex2 = sf2d_create_texture(160, 102, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    previewtex = sf2d_create_texture(160, 102, TEXFMT_RGBA8, SF2D_PLACE_RAM);

	if(background) 
		if((background->pixel_format != TEXFMT_RGBA8) && ( background->width ==0) && ( background->height ==0)) 
			sf2d_free_texture(background);

	char filename[512];

	sprintf(filename, "%s/%s", config_skin_path, "Handy3ds_Background.png");

	background = sfil_load_PNG_file(filename, SF2D_PLACE_RAM);

	sprintf(filename, "%s/%s", config_skin_path, "emptyslot.png");

	emptyslot= sfil_load_PNG_file(filename, SF2D_PLACE_RAM);
	
	if(emptyslot) 
		if((emptyslot->pixel_format != TEXFMT_RGBA8) && ( emptyslot->width ==0) && ( emptyslot->height ==0)) 
			sf2d_free_texture(emptyslot);

	if(!emptyslot) {
		emptyslot = sf2d_create_texture(160, 102, TEXFMT_RGBA8, SF2D_PLACE_RAM);
		memset((char*)emptyslot->data, 0xff , 256 * 128 * 4);    //empty solid white texture
	}

	sprintf(filename, "%s/%s", config_skin_path, "InsertGame.png");

	insertgame= sfil_load_PNG_file(filename, SF2D_PLACE_RAM);
	
	if(insertgame) 
		if((emptyslot->pixel_format != TEXFMT_RGBA8) && ( emptyslot->width !=160) && ( emptyslot->height =102)) 
			sf2d_free_texture(emptyslot);

    stretchx = (float)400 / (float)160;
    stretchy = (float)240 / (float)102;
    stretchr = (float)240 / (float)160;

    bgstretchx = (float)400 / (float)background->width;
    bgstretchy = (float)240 / (float)background->height;


}

void handy_3ds_video_quit(void)
{
    sf2d_free_texture(screentex1);
    sf2d_free_texture(screentex2);
    sf2d_free_texture(previewtex);
    sf2d_free_texture(emptyslot);
	if (background) sf2d_free_texture(background);
	if (insertgame) sf2d_free_texture(insertgame);

    sftd_free_font(font);
    sftd_fini();
    sf2d_fini();

}

void handy_3ds_video_flip(void) {

    GSPGPU_FlushDataCache(NULL, (u8*)currentscreen->data, 102*256*4);

    currentscreen->tiled = 0;
    sf2d_texture_tile32(currentscreen);
        
    sf2d_start_frame(GFX_TOP, GFX_LEFT);

	if(background && Handy_cfg_ShowBackground && ((Handy_cfg_scalemode <3) || (Handy_rotation&1)) )
		sf2d_draw_texture_part_rotate_scale(background, 200, 120, 0, 0, 0, background->width, background->height, bgstretchx, bgstretchy);

    switch (Handy_rotation) {
		case 0:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 0, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 0, 0, 0, 160, 102, 2, 2); //2x
					break;
				case 2:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 0, 0, 0, 160, 102, stretchy, stretchy);    //fit
					break;
				case 3:
				default:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 0, 0, 0, 160, 102, stretchx, stretchy);  //stretch
					break;
			}
			break;
		case 1:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 1.57, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
				case 2:
				case 3:
				default:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 1.57, 0, 0, 160, 102, stretchr, stretchr);    //fit
					break;
			}
			break;
		case 2:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 3.14, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 3.14, 0, 0, 160, 102, 2, 2); //2x
					break;
				case 2:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 3.14, 0, 0, 160, 102, stretchy, stretchy);    //fit
					break;
				case 3:
				default:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 200, 120, 3.14, 0, 0, 160, 102, stretchx, stretchy);  //stretch
					break;
			}
			break;
		case 3:
		default:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 400, 120, 4.71, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
				case 2:
				case 3:
				default:
					sf2d_draw_texture_part_rotate_scale(currentscreen, 400, 120, 4.71, 0, 0, 160, 102, stretchr, stretchr);    //fit
					break;
			}
			break;
	}
        
    sf2d_end_frame();
    gfxFlushBuffers();

    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	if (Handy_cfg_BottomScr) {
		static char buffer[64];

        sprintf(buffer, "FPS: %.2f", fps_counter);
	    sftd_draw_text(font, 4, 8, 0xffffffff, 10, buffer);
		sftd_draw_text(font, 24, 115, 0xffffffff, 16, "Touch screen for menu");
    } else sf2d_draw_rectangle(0, 0, 320, 240, 0x000000ff); 

	sf2d_end_frame();
    gfxFlushBuffers();

    sf2d_swapbuffers();
	
    if (currentscreen == screentex1) {
        mpLynxBuffer = (Uint32 *)screentex2->data;
        currentscreen = screentex2;
        prevscreen = screentex1;
    } else {
        mpLynxBuffer = (Uint32 *)screentex1->data;
        currentscreen = screentex1;
        prevscreen = screentex2;
    }
}

/*
    This is called also from gui when initializing for rom browser
*/
int handy_3ds_video_early_setup(int surfacewidth, int surfaceheight, int bpp_flag, int videoflags)
{
    // Setup the main surface
    {
        surfacewidth = 160;
        surfaceheight = 102;
    }

    return 1;
}

/*
    Name                :     handy_3ds_video_setup
    Parameters          :     rendertype ( 1 = SDL, 2 = OpenGL, 3 = YUV )
                            fsaa ( 0 = off, 1 = on ) -> OpenGL specific
                            fullscreen ( 0 = off, 1 = on )
    Function            :   Initialisation of the video using the SDL libary.

    Uses                :   N/A

    Information            :    This is our setup function for getting our desired
                            video setup.
*/
int handy_3ds_video_setup(int rendertype, int fsaa, int fullscreen, int bpp, int scale, int accel, int sync)
{
//    const    SDL_VideoInfo     *info;
//            Uint32             videoflags;
//            int              value;
            int                 bpp_flag;
//            int                 surfacewidth;
//            int                 surfaceheight;

    // Since we first checked the rotation, based upon that information
    // We setup the width and height of the display.
    //
    // If OpenGL rendering is selected, we choose 640x480 or 480x640 as
    // output. OpenGL rendering can be funky with non-standard resolutions
    // with buggy OpenGL drivers and/or videocards.
    //
    // This is really ugly but good enough for a first version :)
    switch(LynxRotate) {
        case MIKIE_NO_ROTATE:
            LynxWidth  = 160;
            LynxHeight = 102;
            break;
        case MIKIE_ROTATE_L:
        case MIKIE_ROTATE_R:
            LynxWidth  = 102;
            LynxHeight = 160;
            break;
    }

    if( bpp != 0 )
    {
            bpp_flag = bpp;
    }
    else
    {/*
            switch(info->vfmt->BitsPerPixel)
            {
                case 8:
                    sdl_bpp_flag = 8;
                    break;
                case 16:
                    sdl_bpp_flag = 16;
                    break;
                case 24:
                    sdl_bpp_flag = 24;
                    break;
                case 32:
                    sdl_bpp_flag = 32;
                    break;
                default:
                    sdl_bpp_flag = 8;  // Default : 8bpp
                    break;
            }*/
    }
    mpBpp = bpp_flag;

    printf("Rendering : %dBPP\n", bpp_flag);



    return 1;
}


//UBYTE *handy_3ds_display_fake_callback(ULONG objref);

Uint16 CalculatePitch(void)
{
    Uint16 pitch;
    
    /* Surface should be 4-byte aligned for speed */
    pitch = 1024;    //480;  //surface->w*surface->format->BytesPerPixel; !!or 3 (RGB)?

    /*switch (surface->format->BitsPerPixel) {  //!!BitsPP = 32?
        case 1:
            pitch = (pitch+7)/8;
            break;
        case 4:
            pitch = (pitch+1)/2;
            break;
        default:
            break;
    }*/
    pitch = (pitch + 3) & ~3;	/* 4-byte aligning */
    return(pitch);
}


/*
    Name                :     handy_3ds_display_callback
    Parameters          :     N/A
    Function            :   Handy 3DS display rendering function.

    Uses                :   HandyBuffer  ( Handy/SDL display buffer )
                            mainSurface     ( Handy/SDL primary display )

    Information            :    Renders the graphics from HandyBuffer to
                            the main surface.
*/

/*
UBYTE *handy_3ds_display_fake_callback(ULONG objref)
{
    return (UBYTE *)mpLynxBuffer;
}
*/

UBYTE *handy_3ds_display_callback(ULONG objref)
{
 
	handy_3ds_video_flip();  //mainSurface );
    return (UBYTE *)mpLynxBuffer;
}


void handy_3ds_video_reinit(int bpp)
{

    printf("Initialising Handy Display... ");
    switch(bpp)
    {
        case 8:
            LynxFormat = MIKIE_PIXEL_FORMAT_8BPP;
            break;
        case 15:
            LynxFormat = MIKIE_PIXEL_FORMAT_16BPP_555;
            break;
        case 16:
            LynxFormat = MIKIE_PIXEL_FORMAT_16BPP_565;
            break;
        case 24:
            LynxFormat = MIKIE_PIXEL_FORMAT_24BPP;
            break;
        case 32:
            LynxFormat = MIKIE_PIXEL_FORMAT_32BPP;
            break;
        default:
            LynxFormat = MIKIE_PIXEL_FORMAT_16BPP_565; // Default 16BPP bpp
            break;
    }

    // screentext1 must be initialized previously
    mpLynxBuffer = (Uint32 *)screentex1->data;  
    currentscreen = screentex1;
    prevscreen = screentex2;

    mpLynx->DisplaySetAttributes( LynxRotate, LynxFormat, (ULONG)CalculatePitch(), handy_3ds_display_callback, (ULONG)mpLynxBuffer);

    printf("[DONE]\n");
}





