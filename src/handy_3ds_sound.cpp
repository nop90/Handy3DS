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
#include <malloc.h>
#include "system.h"
#include "handy_3ds_main.h"
#include "handy_3ds_sound.h"

//u8 *stream;
u8 *stream0, *stream1, *stream2, *stream3;

int soundstate=0;

/*  
    Name                :   handy_sdl_audio_callback
    Parameters          :   userdata (NULL, not used)
                            stream   (sample data)
                            len      (sampel length)
    Function            :   Our SDL audio callback/output function
    
    Uses                :   gAudioBuffer        (Lynx Audio Buffer)
                            gAudioBufferPointer (Filled size of gAudioBuffer)
    
    Information         :   Only when gAudioBufferPointer is equal or larger 
                            then len, then we want to output the audio.
    
                            gAudioBufferPointer increases everytime the 
                            mpLynx->Update() function is called. It is 
                            possible that gAudioBufferPointer exceeds the
                            number of the sample length. So we allways reset 
                            the gAudioBuffer and gAudioBufferPointer based 
                            upon the differences between len en gAudioBufferPointer
*/
void handy_3ds_audio_callback(int start, int len)
{
//int buffertail;

//    if( ( (int)gAudioBufferPointer >= (len)) && (gAudioBufferPointer != 0) && (!gSystemHalt) ) {
//        memcpy(stream, gAudioBuffer, len);
//        memmove(gAudioBuffer, gAudioBuffer+len, gAudioBufferPointer - len);


/*
    if(!gSystemHalt && soundstate) {
        if (start+len<HANDY_AUDIO_BUFFER_SIZE) {
			GSPGPU_FlushDataCache(NULL, gAudioBuffer0+start, len);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer1+start, len);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer2+start, len);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer3+start, len);
		} else {
			buffertail = HANDY_AUDIO_BUFFER_SIZE - start;
			GSPGPU_FlushDataCache(NULL, gAudioBuffer0+start, buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer1+start, buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer2+start, buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer3+start, buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer0, len-buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer1, len-buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer2, len-buffertail);
			GSPGPU_FlushDataCache(NULL, gAudioBuffer3, len-buffertail);
		}
    }
*/
		GSPGPU_FlushDataCache(gAudioBuffer0, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer1, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer2, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer3, HANDY_AUDIO_BUFFER_SIZE);
}

void handy_3ds_audio_start(int freq, int start)
{
Uint8 *buff[HANDY_AUDIO_BUFFER_SIZE];

    if( !gSystemHalt ) {
	
	if (start) {
        memcpy(buff, gAudioBuffer0, start);
		memmove(gAudioBuffer0, gAudioBuffer0+start, HANDY_AUDIO_BUFFER_SIZE - start);
        memcpy(gAudioBuffer0+HANDY_AUDIO_BUFFER_SIZE - start, buff, start);
        
		memcpy(buff, gAudioBuffer1, start);
		memmove(gAudioBuffer1, gAudioBuffer1+start, HANDY_AUDIO_BUFFER_SIZE - start);
        memcpy(gAudioBuffer1+HANDY_AUDIO_BUFFER_SIZE - start, buff, start);
        
		memcpy(buff, gAudioBuffer2, start);
		memmove(gAudioBuffer2, gAudioBuffer2+start, HANDY_AUDIO_BUFFER_SIZE - start);
        memcpy(gAudioBuffer2+HANDY_AUDIO_BUFFER_SIZE - start, buff, start);
        
		memcpy(buff, gAudioBuffer3, start);
		memmove(gAudioBuffer3, gAudioBuffer3+start, HANDY_AUDIO_BUFFER_SIZE - start);
        memcpy(gAudioBuffer3+HANDY_AUDIO_BUFFER_SIZE - start, buff, start);
	}
	
		GSPGPU_FlushDataCache(gAudioBuffer0, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer1, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer2, HANDY_AUDIO_BUFFER_SIZE);
		GSPGPU_FlushDataCache(gAudioBuffer3, HANDY_AUDIO_BUFFER_SIZE);

		csndPlaySound(0x8, SOUND_REPEAT | SOUND_FORMAT_8BIT, freq, 1.0, 0.0, (u32*)gAudioBuffer0, (u32*)gAudioBuffer0, HANDY_AUDIO_BUFFER_SIZE);
		csndPlaySound(0x9, SOUND_REPEAT | SOUND_FORMAT_8BIT, freq, 1.0, 0.0, (u32*)gAudioBuffer1, (u32*)gAudioBuffer1, HANDY_AUDIO_BUFFER_SIZE);
		csndPlaySound(0xa, SOUND_REPEAT | SOUND_FORMAT_8BIT, freq, 1.0, 0.0, (u32*)gAudioBuffer2, (u32*)gAudioBuffer2, HANDY_AUDIO_BUFFER_SIZE);
		csndPlaySound(0xb, SOUND_REPEAT | SOUND_FORMAT_8BIT, freq, 1.0, 0.0, (u32*)gAudioBuffer3, (u32*)gAudioBuffer3, HANDY_AUDIO_BUFFER_SIZE);
		soundstate=1;
    }
}


void handy_3ds_audio_pause(void)
{
	if (soundstate) {
		CSND_SetPlayState(0x8, 0);//Stop audio playback.
		CSND_SetPlayState(0x9, 0);
		CSND_SetPlayState(0xa, 0);
		CSND_SetPlayState(0xb, 0);
		csndExecCmds(0);
		
		gAudioBufferPointer=0; // !!!!!!

		soundstate=0;
	}
}

int handy_3ds_audio_getstate(void)
{
	return soundstate;
}

int handy_3ds_audio_init(void)
{
    
	if(csndInit()) return 0;
    return 1;
}

void handy_3ds_audio_quit(void)
{
	CSND_SetPlayState(0x8, 0);//Stop audio playback.
	CSND_SetPlayState(0x9, 0);
	CSND_SetPlayState(0xa, 0);
	CSND_SetPlayState(0xb, 0);
	csndExecCmds(0);
	csndExit();
}

