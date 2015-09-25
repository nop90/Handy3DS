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
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cctype>
#include <unistd.h>
#include <dirent.h>


#include "handy_3ds_main.h"
#include "handy_3ds_graphics.h"
#include "handy_3ds_handling.h"
#include "handy_3ds_sound.h"
#include "handy_3ds_config.h"
#include "handy_3ds_gui.h"

#include "3dsemu_system.h"

#include <3ds.h>
#include <sf2d.h>

#define TICKS_PER_SEC (268123480)
#define TICKS_PER_NSEC (0.268123480)
#define TICKS_PER_FRAME (TICKS_PER_SEC/60.0)

#define MAX__PATH 1024

char bios_path_and_name[512]; // full path of lynxboot.img
char rom_name_with_no_ext[128]; // rom name with no extension, used for savestates
char savename[512];

float fps_counter = 60.0;

char config_roms_path[MAX__PATH];
char config_base_path[MAX__PATH];
char config_bios_path[MAX__PATH];
char config_skin_path[MAX__PATH];
char config_save_path[MAX__PATH];

FS_archive sdmcArchive;
Handle dirHandle;
FS_dirent entry;

bool mydebug = false;

/* Handy declarations */
Uint32          *mpLynxBuffer;
CSystem   		*mpLynx;
int             mFrameSkip = 0;
int             mpBpp;                    // Lynx rendering bpp

int              LynxWidth;                // Lynx screen width
int              LynxHeight;              // Lynx screen height
int              LynxScale = 1;            // Factor to scale the display
int              LynxLCD = 1;            // Emulate LCD Display
int              LynxFormat;                // Lynx ROM format type
int              LynxRotate;                // Lynx ROM rotation type

int              emulation = 0;
int              rendertype = 1;
int              stype = 1;                // Scaling/Scanline routine.
int              filter = 0;                // Scaling/Scanline routine.

u8 isN3DS = 0;

extern int 		Handy_cfg_Throttle; // Throttle to 60FPS

extern int		Handy_cfg_Sound;

void get_config_path()
{
	if(strlen(config_base_path) == 0) {
        sdmcArchive = (FS_archive){ARCH_SDMC, (FS_path){PATH_EMPTY, 1, (u8*)""}};
        FSUSER_OpenArchive(NULL, &sdmcArchive);
        
        sprintf(config_base_path, "/lynx");

        FS_path dirPath = (FS_path){PATH_CHAR, strlen(config_base_path)+1, (u8*)config_base_path};
        FSUSER_OpenDirectory(NULL, &dirHandle, sdmcArchive, dirPath);
        
        if(!dirHandle) sprintf(config_base_path, "/"); //!!
        FSDIR_Close(dirHandle);
		
		sprintf(config_roms_path, "%s/%s", config_base_path, "Roms");
		sprintf(config_bios_path, "%s/%s", config_base_path, "Bios");
		sprintf(config_skin_path, "%s/%s", config_base_path, "Skin");
		sprintf(config_save_path, "%s/%s", config_base_path, "Save");
	}

}


/*
    Name                :     handy_3ds_update
    Parameters          :     N/A
    Function            :   Update/Throttle function for Handy 3ds.

    Uses                :   N/A

    Information            :    This function is basicly the Update() function from
                            Handy WIN32 with minor tweaks. It is used for
                            basic throttle of the Handy core.
*/
inline    int handy_3ds_update(void)
{
        // Throttling code
        //
        if(gSystemCycleCount>gThrottleNextCycleCheckpoint)
        {
            static int limiter=0;
            static int flipflop=0;
            int overrun=gSystemCycleCount-gThrottleNextCycleCheckpoint;
            int nextstep=(((HANDY_SYSTEM_FREQ/HANDY_BASE_FPS)*gThrottleMaxPercentage)/100);

            // We've gone thru the checkpoint, so therefore the
            // we must have reached the next timer tick, if the
            // timer hasnt ticked then we've got here early. If
            // so then put the system to sleep by saying there
            // is no more idle work to be done in the idle loop

            if(gThrottleLastTimerCount==gTimerCount)
            {
                // All we know is that we got here earlier than expected as the
                // counter has not yet rolled over
                if(limiter<0) limiter=0; else limiter++;
                if(limiter>40 && mFrameSkip>0)
                {
                    mFrameSkip--;
                    limiter=0;
                }
                flipflop=1;
                return 0;
            }

            // Frame Skip adjustment
            if(!flipflop)
            {
                if(limiter>0) limiter=0; else limiter--;
                if(limiter<-7 && mFrameSkip<10)
                {
                    mFrameSkip++;
                    limiter=0;
                }
            }

            flipflop=0;

            //Set the next control point
            gThrottleNextCycleCheckpoint+=nextstep;

            // Set next timer checkpoint
            gThrottleLastTimerCount=gTimerCount;

            // Check if we've overstepped the speed limit
            if(overrun>nextstep)
            {
                // We've exceeded the next timepoint, going way too
                // fast (sprite drawing) so reschedule.
                return 0;
            }

        }

        return 1;

}

/*
    Name                :     handy_3ds_rom_info
    Parameters          :     N/A
    Function            :   Game Image information function for Handy 3ds

    Uses                :   N/A

    Information            :    Basic function for getting information of the
                            Atari Lynx game image and for setting up the
                            Handy core concerning rotation.
*/
void handy_3ds_rom_info(void)
{

    printf("Atari Lynx ROM Information\n");

    /* Retrieving Game Image information */
    printf("Cartname      : %s\n"   , mpLynx->CartGetName()         );
    printf("ROM Size      : %d kb\n", (int)mpLynx->CartSize()      );
    printf("Manufacturer  : %s\n"   , mpLynx->CartGetManufacturer() );

    /* Retrieving Game Image Rotatation */
    printf("Lynx Rotation : ");
    switch(mpLynx->CartGetRotate())
    {
        case CART_NO_ROTATE:
            LynxRotate = MIKIE_NO_ROTATE;
            printf("NO\n");
            break;
        case CART_ROTATE_LEFT:
            LynxRotate = MIKIE_ROTATE_L;
            printf("LEFT\n");
            break;
        case CART_ROTATE_RIGHT:
            LynxRotate = MIKIE_ROTATE_R;
            printf("RIGHT\n");
            break;
        default:
            // Allright, this shouldn't be necassary. But in case the user is using a
            // bad dump, we use the default rotation as in no rotation.
            LynxRotate = MIKIE_NO_ROTATE;
            printf("NO (forced)\n");
            break;
    }

    /* Retrieving Game Image type */
    printf("ROM Type      : ");
    switch(mpLynx->mFileType)
    {
        case HANDY_FILETYPE_HOMEBREW:
            printf("Homebrew\n");
            break;
        case HANDY_FILETYPE_LNX:
            printf("Commercial and/or .LNX-format\n");
            break;
        case HANDY_FILETYPE_SNAPSHOT:
            printf("Snapshot\n");
            break;
        default:
            // Allright, this shouldn't be necessary, but just in case.
            printf("Unknown format!\n");
            if (mydebug) do {
                hidScanInput();
            } while (!(hidKeysHeld() & KEY_TOUCH));
            exit(EXIT_FAILURE);
            break;
    }

}
void handy_3ds_quit(void)
{
    emulation   = -1;

	handy_3ds_video_quit();
	handy_3ds_audio_quit();

    hidExit();
    aptExit();
    srvExit();

    exit(EXIT_SUCCESS);

}

void handy_3ds_core_init(char *romname)
{
    int i;

    // strip rom name from full path and cut off the extension
    for(i = strlen(romname) - 1; i >= 0; i--) {
        if(romname[i] == '/' || i == 0) { 
            memcpy((void *)rom_name_with_no_ext, (void *)(romname + i + (i?1:0)), strlen(romname) - i);
            rom_name_with_no_ext[strlen(rom_name_with_no_ext)-4] = 0; // cut off extension.
            break;
        }
    }
    

    printf("Initialising Handy Core...    ");
//    try {
        mpLynx = new CSystem(romname, bios_path_and_name);
/*    } catch (CLynxException &err) {
        cerr << err.mMsg.str() << ": " << err.mDesc.str() << endl;
        if (mydebug) do {
            hidScanInput();
        } while (!(hidKeysHeld() & KEY_TOUCH));
        exit(EXIT_FAILURE);
    }
*/
    printf("[DONE]\n\n");

    // Query Rom Image information
    handy_3ds_rom_info();

}

/*
    Reinit routine used to reload roms on the fly
*/
void handy_3ds_core_reinit(char *romname)
{
    delete mpLynx;
    handy_3ds_core_init(romname);
    handy_3ds_video_reinit(32);

}

void system_checkPolls() {
    APP_STATUS status;

	while((status=aptGetStatus()) != APP_RUNNING) {

        if(status == APP_SUSPENDING)
        {
            aptReturnToMenu();
        }
        else if(status == APP_PREPARE_SLEEPMODE)
        {
			aptSignalReadyForSleep();
            aptWaitStatusEvent();
        }
        else if (status == APP_SLEEPMODE) {
        }
        else if (status == APP_EXITING) {
			handy_3ds_quit();
        }

    }
}


int main()
{
   static char load_filename[512];
    char    *romname = NULL;
	float audioStart; // this is a float to count the half sample per frame needed ho have precise timing
	u32 audioTarget, audioEnd;

   srvInit();
    aptInit();
    hidInit(NULL);

	APT_CheckNew3DS(NULL, &isN3DS);

	get_config_path();

	sprintf(savename, "%s/Handy3ds.cfg", config_base_path);  // using savename char buffer to save memory
	do_config(savename);

	handy_3ds_video_init();

    sprintf(bios_path_and_name, "%s/%s", config_bios_path, "lynxboot.img");

    // Call filebrowser
       if(gui_LoadFile(load_filename)!= -1)  {
            romname = (char *)&load_filename;
            do {
                hidScanInput();
            } while (hidKeysHeld());
        } else {
			handy_3ds_quit();
        }

    // Primary initalise of Handy - should be called AFTER _Init() but BEFORE handy_3ds_video_setup()
    handy_3ds_core_init(romname);

    // Initialise Handy 3ds video 
    handy_3ds_video_setup(1, 0, 0, 32, LynxScale, 0, 0);

    // Initialise Handy 3ds audio
    printf("\nInitialising 3DS Audio...     ");
    gAudioEnabled = handy_3ds_audio_init();

    printf("[DONE]\n");


    // Setup of Handy Core video
    handy_3ds_video_reinit(32);

    printf("Starting Lynx Emulation...\n");
	
    bool touched = false;
    audioStart =  gAudioBufferPointer;  // both should be 0 at this point

	// initialize timers
	u64 tickcurr, syncticknext,synctickres; 
	u64 fpsticknext,fpstickres;
	int frameSyncPeriod;

	if(isN3DS) 
		frameSyncPeriod = 1;
	else	
		frameSyncPeriod = 12;

	fpstickres= TICKS_PER_SEC; 
	synctickres = TICKS_PER_FRAME * frameSyncPeriod; // on o3ds the sync is every 6 frame, i.e. 10 times every sec
	tickcurr=svcGetSystemTick();
	fpsticknext = tickcurr + fpstickres;
	syncticknext = tickcurr + synctickres;
	int fpscnt = 0, synccnt = 0;
	
	gThrottleMaxPercentage = isN3DS?100:50; //  !! TO DO: check if 50 is the best value.

    while(!emulation)
    {
        // Initialise Handy button events
        int OldKeyMask, KeyMask = mpLynx->GetButtonData();
        OldKeyMask = KeyMask;

        // Getting events for keyboard and/or joypad handling
        hidScanInput();
        if (hidKeysHeld() & KEY_TOUCH) {
            touched = true;
        } else {
            if (touched) {
				handy_3ds_audio_pause();
                gui_Run();
                KeyMask = 0;
                touched = false;
//				audioStart =  gAudioBufferPointer;
                do {
                    hidScanInput();
                } while (hidKeysHeld());

				gThrottleMaxPercentage = isN3DS?100:50; //  !! Loading a savestate overrides this value, so we force it again exiting menu

				fpscnt = 0;
				synccnt = 0;
				tickcurr=svcGetSystemTick();
				fpsticknext = tickcurr + fpstickres;
				syncticknext = tickcurr + synctickres;
            } else {
                touched = false;
            }
        }
        KeyMask = handy_3ds_on_key_down(hidKeysHeld(), KeyMask);

        // Check if there are handling events and then update the Handy button events.
        if (OldKeyMask != KeyMask)
            mpLynx->SetButtonData(KeyMask);

        // Update TimerCount
        gTimerCount++;

        while( handy_3ds_update() )
        {
            
            //!!
            hidScanInput();
            u32 held = hidKeysHeld();
            if (held & KEY_SELECT) {
				handy_3ds_video_quit();
				handy_3ds_audio_quit();
                exit(EXIT_SUCCESS);   //break;
            }
            //!!
 
           
            if(!gSystemHalt)
            {
				if (!handy_3ds_audio_getstate()) {
					audioStart=0.0; // reset counter
					gAudioBufferPointer = 0; // reset buffer. really not needed since we set it to 0 in handy_3ds_audio_pause().
				}	

				// increase sound samples counter - 367,5 samples per frame at 60 fps
                audioEnd =  (int)(audioStart + 367.5) % HANDY_AUDIO_BUFFER_SIZE;
				 // filling 1 half of the buffer, roughly enough for slow frames on o3DS without risk of buffer head to reach the tail during the 10 frames before resync.
                audioTarget =  (int)(audioStart + HANDY_AUDIO_BUFFER_SIZE/2.1) % HANDY_AUDIO_BUFFER_SIZE;
				if (audioTarget>audioStart)
				  while (gAudioBufferPointer<audioTarget) mpLynx->Update();  
                else
				  while ((gAudioBufferPointer>audioStart)||(gAudioBufferPointer<audioTarget)) mpLynx->Update();
				  
				if(Handy_cfg_Throttle && Handy_cfg_Sound && gAudioEnabled) {
					if (handy_3ds_audio_getstate())
						handy_3ds_audio_callback(audioStart, (int)(audioTarget-audioStart));  
					else
						handy_3ds_audio_start(22050, audioStart);  
				}	
				audioStart = audioEnd;
            }
            else
            {
                printf("gSystemHalt : %ld\n", gSystemHalt);
                gTimerCount++;
            }
			system_checkPolls();
        }

// Timing

		fpscnt++;
		synccnt++;

		if (Handy_cfg_Throttle) {
			if (synccnt==frameSyncPeriod){
				svcSleepThread((syncticknext - svcGetSystemTick()) / TICKS_PER_NSEC); 
			}
		}

		tickcurr=svcGetSystemTick();
 		
		if (tickcurr >= syncticknext) {
			syncticknext += synctickres;
			synccnt = 0;
		} 
		if (tickcurr >= fpsticknext) {
			fpsticknext += fpstickres;
			fps_counter = fpscnt;
			fpscnt = 0;
		} 

    }   // while(!emulation)

	return 0;
}
