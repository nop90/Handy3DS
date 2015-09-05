/////////////////////////////////////////////////////////////////////////////////
//                       Handy 3DS - An Atari Lynx Emulator                    //
//							Ported to 3DS by nop90 - 2015                      //
//                    Everything not covered by the following                  //
//                          Copyrights is Public Domain                        //
//                                                                             //
//                   This file is based on Handy360 for DINGUX                 //
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
/*

	Main menu items:
	LOAD ROM
	LOAD STATE: 0
	SAVE STATE: 0
	RESET ROM
	CONFIG
	EXIT

	Config menu items;
	IMAGE SCALING: x1 / x2 / FIT / FULLSCREEN
	SOUND: YES / NO 
	SHOW FPS: YES / NO
	LIMIT FPS: YES / NO
	SWAP A/B: YES / NO
	SAVE CONFIG

*/ 

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>
#include "handy_3ds_gui.h"
#include "handy_3ds_config.h"
#include "handy_3ds_main.h"
#include "handy_3ds_graphics.h"

/* defines and macros */
#define MAX__PATH 1024
#define FILE_LIST_ROWS 20   //24
#define FILE_LIST_POSITION 2    //8
#define DIR_LIST_POSITION 104   //208

#define color16(red, green, blue) ((red << 11) | (green << 5) | blue)

#define COLOR_BG            	0x140c08ff  
#define COLOR_ROM_INFO      	0x589068ff  
#define COLOR_ACTIVE_ITEM   	0xffff00ff  
#define COLOR_INACTIVE_ITEM 	0xaaaaaaff  
#define COLOR_INACTIVE_ITEM_BG 	0x444444ff  
#define COLOR_FRAMESKIP_BAR 	0x3c7c7cff  
#define COLOR_HELP_TEXT     	0x00ffffff  

void strncpy_u2a(char* dst, u16* src, int n);

void gui_LoadState();
void gui_SaveState();
void gui_FileBrowserRun();
void gui_ConfigMenuRun();
void gui_ConfigScreenRun();
void gui_SaveConfig();
void gui_Reset();
void gui_DrawTopScreen();

int gui_LoadSlot = 0;

int loadslot = -1; // flag to reload preview screen
bool done = FALSE;  //int done = 0; // flag to indicate exit status


/* external references */
extern char rom_name_with_no_ext[128]; // name of current rom, used for load/save state

extern FS_archive sdmcArchive;
extern Handle dirHandle;
extern FS_dirent entry;

extern int emulation;
extern sftd_font *font;
extern sf2d_texture *prevscreen, *previewtex, *currentscreen, *emptyslot, *background, *insertgame;

extern int Handy_cfg_scalemode;
extern int Handy_cfg_Show_FPS;
extern int Handy_cfg_Throttle;
extern int Handy_cfg_SwapAB;
extern int Handy_cfg_ShowBackground;
extern int Handy_cfg_Sound;
extern int Handy_cfg_BottomScr;
extern int Handy_cfg_BGColor;

extern int Handy_rotation;

extern char config_roms_path[];
extern char config_base_path[];
extern char config_bios_path[];
extern char config_skin_path[];
extern char config_save_path[];

extern float stretchx, stretchy, stretchr, bgstretchx, bgstretchy;

typedef struct {
	char *itemName;
	int *itemPar;
	int itemParMaxValue;
	char **itemParName;
	void (*itemOnA)();
} MENUITEM;

typedef struct {
	int itemNum; // number of items
	int itemCur; // current item
	MENUITEM *m; // array of items
} MENU;

char *gui_ScaleNames[] = {"1x","2x", "Fit", "Stretch"}; 
char *gui_YesNo[] = {"No", "Yes"};
char *gui_ColorNames[] = {"Black","Blue", "Green", "Red"}; 


MENUITEM gui_MainMenuItems[] = {
	{(char *)"Load rom", NULL, NULL, NULL, &gui_FileBrowserRun},
	{(char *)"Load state", &gui_LoadSlot, 9, NULL, &gui_LoadState},
	{(char *)"Save state", &gui_LoadSlot, 9, NULL, &gui_SaveState},
	{(char *)"Reset rom", NULL, NULL, NULL, &gui_Reset},
	{(char *)"Config", NULL, NULL, NULL, &gui_ConfigMenuRun},
	{(char *)"Exit", NULL, NULL, NULL, &handy_3ds_quit} // extern in handy_3ds_main.cpp
};

MENU gui_MainMenu = { 6, 0, (MENUITEM *)&gui_MainMenuItems };

MENUITEM gui_ConfigMenuItems[] = {
	{(char *)"Screen config", NULL, NULL, NULL, &gui_ConfigScreenRun},
	{(char *)"Sound : ", &Handy_cfg_Sound, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Limit fps : ", &Handy_cfg_Throttle, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Swap A/B : ", &Handy_cfg_SwapAB, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Save config", NULL, NULL, NULL, &gui_SaveConfig}
};

MENU gui_ConfigMenu = { 5, 0, (MENUITEM *)&gui_ConfigMenuItems };

MENUITEM gui_ConfigScreenItems[] = {
	{(char *)"Top Screen size : ", &Handy_cfg_scalemode, 3, (char **)&gui_ScaleNames, NULL},
	{(char *)"Bottom Screen : ", &Handy_cfg_BottomScr, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Show fps : ", &Handy_cfg_Show_FPS, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Show Background : ", &Handy_cfg_ShowBackground, 1, (char **)&gui_YesNo, NULL},
	{(char *)"Background color: ", &Handy_cfg_BGColor, 3, (char **)&gui_ColorNames, NULL}	
};

MENU gui_ConfigScreen = {5, 0, (MENUITEM *)&gui_ConfigScreenItems };



void gui_SaveConfig()
{
	char savename[512];
	sprintf(savename, "%s/Handy3ds.cfg", config_base_path);  // using savename char buffer to save memory
	save_config(savename);
}


/*
	Clears mainSurface
*/
void gui_ClearScreen()
{
    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
    sftd_draw_text(font, 40, 115, 0xffffffff, 16, "Wait please ...");
    sf2d_end_frame();
    gfxFlushBuffers();
    sf2d_swapbuffers();
}

void ShowMenuItem(int x, int y, MENUITEM *m, int fg_color, int showparam)
{
	static char i_str[24];

	sftd_draw_text(font, x, y, fg_color, 10, m->itemName);


	// if parameter, show it on the left
	if((m->itemPar != NULL) && showparam){
		if(m->itemParName == NULL ) {
			// if parameter is a digit
			sprintf(i_str, "%i", *m->itemPar);
		} else {
			// if parameter is a name in array
			sprintf(i_str, "%s", *(m->itemParName + *m->itemPar));
		}
		sftd_draw_text(font, x+70, y, fg_color, 10, i_str);
	}
}

void gui_LoadState()
{
	char savename[512];

	sprintf(savename, "%s/%s.%i.sav", config_save_path, rom_name_with_no_ext, gui_LoadSlot);
	//printf("Load state: %s\n", savename);

	// check if file exists otherwise mpLynx->ContextLoad will crash
	FILE *fp = fopen(savename, "rb");
	if(!fp) return;
	fclose(fp);

	if(!mpLynx->ContextLoad(savename)) printf("Error loading state: %s\n", savename);
		else { mpLynx->SetButtonData(0); done = TRUE; }
}

void gui_SaveState()
{
	char savename[512];

	sprintf(savename, "%s/%s.%i.img", config_save_path, rom_name_with_no_ext, gui_LoadSlot);
	FILE *fp = fopen(savename, "wb");
	fwrite((void *)prevscreen->data, 1, 256 * 128 * 4, fp);
	fclose(fp);

	sprintf(savename, "%s/%s.%i.sav", config_save_path, rom_name_with_no_ext, gui_LoadSlot);

	if(!mpLynx->ContextSave(savename)) printf("Error saving state: %s\n", savename); //else done = TRUE;

	loadslot = -1; // show preview immediately
}

int sort_function(const void *dest_str_ptr, const void *src_str_ptr)
{
	char *dest_str = *((char **)dest_str_ptr);
	char *src_str = *((char **)src_str_ptr);

	if(src_str[0] == '.') return 1;

	if(dest_str[0] == '.') return -1;

	return strcasecmp(dest_str, src_str);
}

s32 load_file(char **wildcards, char *result, bool startup)
{

	char current_dir_name[MAX__PATH];
    char prev_dir_name[MAX__PATH];
	DIR *current_dir;
	struct dirent *current_file;
	struct stat file_info;
	char current_dir_short[81];
	u32 current_dir_length;
	u32 total_filenames_allocated;
	u32 total_dirnames_allocated;
	char **file_list;
	char **dir_list;
	u32 num_files;
	u32 num_dirs;
	char *file_name;
	u32 file_name_length;
	u32 ext_pos = -1;
	u32 chosen_file, chosen_dir;
	u32 dialog_result = 1;
	s32 return_value = 1;
	u32 current_file_selection;
	u32 current_file_scroll_value;
	u32 current_dir_selection;
	u32 current_dir_scroll_value;
	u32 current_file_in_scroll;
	u32 current_dir_in_scroll;
	u32 current_file_number, current_dir_number;
	u32 current_column = 0;
	u32 repeat;
	u32 i;

    //getcwd(current_dir_name, MAX__PATH);
    strcpy(current_dir_name, config_roms_path);
    strcpy(prev_dir_name, current_dir_name);
	while(return_value == 1) {
		current_file_selection = 0;
		current_file_scroll_value = 0;
		current_dir_selection = 0;
		current_dir_scroll_value = 0;
		current_file_in_scroll = 0;
		current_dir_in_scroll = 0;

		total_filenames_allocated = 32;
		total_dirnames_allocated = 32;
		file_list = (char **)malloc(sizeof(char *) * 32);
		dir_list = (char **)malloc(sizeof(char *) * 32);
		memset(file_list, 0, sizeof(char *) * 32);
		memset(dir_list, 0, sizeof(char *) * 32);

		num_files = 0;
		num_dirs = 0;
		chosen_file = 0;
		chosen_dir = 0;
        
        file_name= (char*) malloc(0x105);

        FS_path dirPath = (FS_path){PATH_CHAR, strlen(current_dir_name)+1, (u8*)current_dir_name};
        FSUSER_OpenDirectory(NULL, &dirHandle, sdmcArchive, dirPath);

		// DEBUG
		printf("Current directory: %s\n", current_dir_name);
		u32 nread = 0;
		do {
            if(dirHandle) FSDIR_Read(dirHandle, &nread, 1, &entry);

            if(nread) { //(current_file) {
               strncpy_u2a(file_name, entry.name, 0x105);  //utf-16 to ascii function yoinked from blargSNES
				file_name_length = strlen(file_name);

                if(((file_name[0] != '.') || (file_name[1] == '.'))) {
					//if(S_ISDIR(file_info.st_mode)) {    //!!!!!!!!
                    if(entry.isDirectory) {
                        if((strcmp(file_name, "filer") != 0) && (strcmp(file_name, "Nintendo 3DS") != 0) && (strcmp(file_name, "private") != 0)) {
                            dir_list[num_dirs] = (char *)malloc(file_name_length + 1);
                            strcpy(dir_list[num_dirs], file_name);

                            num_dirs++;
                        }
					} else {
					// Must match one of the wildcards, also ignore the .
						if(file_name_length >= 4) {
							if(file_name[file_name_length - 4] == '.') ext_pos = file_name_length - 4;
							else if(file_name[file_name_length - 3] == '.') ext_pos = file_name_length - 3;
							else ext_pos = 0;

							for(i = 0; wildcards[i] != NULL; i++) {
								if(!strcasecmp((file_name + ext_pos), wildcards[i])) {
									file_list[num_files] = (char *)malloc(file_name_length + 1);

									strcpy(file_list[num_files], file_name);

									num_files++;
									break;
								}
							}
						}
					}
				}

				if(num_files == total_filenames_allocated) {
					file_list = (char **)realloc(file_list, sizeof(char *) * total_filenames_allocated * 2);
					memset(file_list + total_filenames_allocated, 0, sizeof(char *) * total_filenames_allocated);
					total_filenames_allocated *= 2;
				}

				if(num_dirs == total_dirnames_allocated) {
					dir_list = (char **)realloc(dir_list, sizeof(char *) * total_dirnames_allocated * 2);
					memset(dir_list + total_dirnames_allocated, 0, sizeof(char *) * total_dirnames_allocated);
					total_dirnames_allocated *= 2;
				}
			}
        } while(nread); 

		qsort((void *)file_list, num_files, sizeof(char *), sort_function);
		qsort((void *)dir_list, num_dirs, sizeof(char *), sort_function);

        FSDIR_Close(dirHandle);

		current_dir_length = strlen(current_dir_name);

		if(current_dir_length > 80) {
			memcpy(current_dir_short, "...", 3);
			memcpy(current_dir_short + 3, current_dir_name + current_dir_length - 77, 77);
			current_dir_short[80] = 0;
		} else {
			memcpy(current_dir_short, current_dir_name, current_dir_length + 1);
		}

		repeat = 1;

		if(num_files == 0) current_column = 1;
		if(num_dirs == 0) current_column = 0;

		char print_buffer[81];

		while(repeat) {
			sf2d_start_frame(GFX_BOTTOM, GFX_LEFT); //!!
			sftd_draw_text(font, 0, 4, COLOR_ACTIVE_ITEM, 10, current_dir_short);
            if (startup) {
				sftd_draw_text(font, 50, 115, COLOR_HELP_TEXT, 10, "Press B to exit");
            } else {
				sftd_draw_text(font, 25, 115, COLOR_HELP_TEXT, 10, "Press B to return to the main menu");
            }
			for(i = 0, current_file_number = i + current_file_scroll_value; i < FILE_LIST_ROWS; i++, current_file_number++) {
				if(current_file_number < num_files) {
                    strncpy(print_buffer,file_list[current_file_number], 30);   //38);
                    print_buffer[30] = 0;   //38] = 0;
					if((current_file_number == current_file_selection) && (current_column == 0)) {
						sftd_draw_text(font, FILE_LIST_POSITION, ((i + 2) * 5), COLOR_ACTIVE_ITEM, 10, print_buffer);
					} else {
						sftd_draw_text(font, FILE_LIST_POSITION, ((i + 2) * 5), COLOR_INACTIVE_ITEM, 10, print_buffer);
					}
				}
			}
			for(i = 0, current_dir_number = i + current_dir_scroll_value; i < FILE_LIST_ROWS; i++, current_dir_number++) {
				if(current_dir_number < num_dirs) {
                    strncpy(print_buffer,dir_list[current_dir_number], 8);  //13);
                    print_buffer[9] = 0;    //14] = 0;
					if((current_dir_number == current_dir_selection) && (current_column == 1)) {
						sftd_draw_text(font, DIR_LIST_POSITION, ((i + 2) * 5), COLOR_ACTIVE_ITEM, 10, print_buffer);
					} else {
						sftd_draw_text(font, DIR_LIST_POSITION, ((i + 2) * 5), COLOR_INACTIVE_ITEM, 10, print_buffer);
					}
				}
			}

			// Catch input
			// change to read key state later
            if (aptGetStatus() == APP_PREPARE_SLEEPMODE) {
                aptSignalReadyForSleep();
                aptWaitStatusEvent();
            } else if (aptGetStatus() == APP_SUSPENDING) {
                aptReturnToMenu();
            }
            hidScanInput();
            u32 keydown = hidKeysDown();
                   if (keydown & KEY_A) {  
						if(current_column == 1) {
							if(num_dirs != 0) {
								repeat = 0;
								strcpy(prev_dir_name, current_dir_name);
								if (strlen(current_dir_name)>1) strcat(current_dir_name, "/");
								strcat(current_dir_name, dir_list[current_dir_selection]);
							}
						} else {
							if(num_files != 0) {
								repeat = 0;
								return_value = 0;
								//strcpy(result, file_list[current_file_selection]);
								sprintf(result, "%s/%s", current_dir_name, file_list[current_file_selection]);
								break;
							}
						}
					}
                    if (keydown & KEY_Y) {
                            repeat = 0;
                            char* findpath = strrchr(current_dir_name,'/');
                        if(findpath > current_dir_name) 
                            findpath[0] = '\0';
                        else 
                            findpath[1] = '\0';
                    }
					if (keydown & KEY_B ) { 
						return_value = -1;
						repeat = 0;
						break;
					}
					if (keydown & KEY_UP) {  
						if(current_column == 0) {
							if(current_file_selection) {
								current_file_selection--;
								if(current_file_in_scroll == 0) {
									//clear_screen(COLOR_BG);
									current_file_scroll_value--;
								} else {
									current_file_in_scroll--;
								}
							}
						} else {
							if(current_dir_selection) {
								current_dir_selection--;
								if(current_dir_in_scroll == 0) {
									//clear_screen(COLOR_BG);
									current_dir_scroll_value--;
								} else {
									current_dir_in_scroll--;
								}
							}
						}
					}
					if (keydown & KEY_DOWN) { 
						if(current_column == 0) {
							if(current_file_selection < (num_files - 1)) {
								current_file_selection++;
								if(current_file_in_scroll == (FILE_LIST_ROWS - 1)) {
									//clear_screen(COLOR_BG);
									current_file_scroll_value++;
								} else {
									current_file_in_scroll++;
								}
							}
						} else {
							if(current_dir_selection < (num_dirs - 1)) {
								current_dir_selection++;
								if(current_dir_in_scroll == (FILE_LIST_ROWS - 1)) {
									//clear_screen(COLOR_BG);
									current_dir_scroll_value++;
								} else {
									current_dir_in_scroll++;
								}
							}
						}
					}
					if (keydown & KEY_L) {  
						if(current_column == 0) {
							if(current_file_selection>FILE_LIST_ROWS) {
								current_file_selection-=FILE_LIST_ROWS;
								current_file_scroll_value -= FILE_LIST_ROWS;
								if (current_file_in_scroll>current_file_selection){
									//clear_screen(COLOR_BG);
									current_file_scroll_value=0;
									current_file_in_scroll=current_file_selection;
								}
							} else {
								current_file_selection=0;
								current_file_scroll_value=0;
								current_file_in_scroll=0;
							}
						} else {
							if(current_dir_selection) {
								current_dir_selection--;
								if(current_dir_in_scroll == 0) {
									//clear_screen(COLOR_BG);
									current_dir_scroll_value--;
								} else {
									current_dir_in_scroll--;
								}
							}
						}
					}
					if (keydown & KEY_R) {  
						if(current_column == 0) {
							if(current_file_selection < (num_files - 1 - FILE_LIST_ROWS)) {
								current_file_selection+=FILE_LIST_ROWS;
								current_file_scroll_value+=FILE_LIST_ROWS;
								if (current_file_scroll_value>(num_files - FILE_LIST_ROWS)){
									//clear_screen(COLOR_BG);
									current_file_scroll_value=num_files - FILE_LIST_ROWS;
									current_file_in_scroll=  FILE_LIST_ROWS - (num_files - current_file_selection);
								}
									//clear_screen(COLOR_BG);
							} else {
								current_file_selection = num_files - 1;
								current_file_in_scroll = (num_files<=FILE_LIST_ROWS - 1)?num_files:FILE_LIST_ROWS - 1;
								current_file_scroll_value = (num_files > FILE_LIST_ROWS)?num_files - FILE_LIST_ROWS:0;
							}
						} else {
							if(current_dir_selection < (num_dirs - 1)) {
								current_dir_selection++;
								if(current_dir_in_scroll == (FILE_LIST_ROWS - 1)) {
									//clear_screen(COLOR_BG);
									current_dir_scroll_value++;
								} else {
									current_dir_in_scroll++;
								}
							}
						}
					}
					if (keydown & KEY_LEFT) { 
						if(current_column == 1) {
							if(num_files != 0) current_column = 0;
						}
					}
					if (keydown & KEY_RIGHT) {  
						if(current_column == 0) {
							if(num_dirs != 0) current_column = 1;
						}
					}

            sf2d_end_frame();
            
			gfxFlushBuffers();

			gui_DrawTopScreen();
            
			gfxFlushBuffers();

            sf2d_swapbuffers();


		}

		// free pointers
		for(i = 0; i < num_files; i++) free(file_list[i]);
		free(file_list);

		for(i = 0; i < num_dirs; i++) free(dir_list[i]);
		free(dir_list);
        
        free(file_name);
	}
	
	
	return return_value;
}

/*
	Rom file browser which is called from menu
*/
char *file_ext[] = { ".lnx", ".lyx", ".zip", NULL };

void gui_FileBrowserRun()
{

	static char load_filename[512];

	if(load_file(file_ext, load_filename, false) != -1) { // exit if file is chosen
		handy_3ds_core_reinit(load_filename);
		gui_ClearScreen();
		done = TRUE;
		loadslot = -1;
	}
}

/*
	Rom browser which is called FIRST before all other init
	Return values :		1 - file chosen, name is written at *romname
						0 - no file
*/
int gui_LoadFile(char *romname)
{
	int result = 0;

	if(load_file(file_ext, romname, true) != -1) result = 1;
    
    gui_ClearScreen();

	return result;
}

/*
	Shows previews of load/save and pause
*/
void ShowPreview(MENU *menu)
{
	char prename[256];
	char *prebuffer = (char*)previewtex->data;

	if(menu == &gui_MainMenu && (menu->itemCur == 1 || menu->itemCur == 2)) {   //show savestate preview
		if(loadslot != gui_LoadSlot) {
			// create preview name
			sprintf(prename, "%s/%s.%i.img", config_save_path, rom_name_with_no_ext, gui_LoadSlot);
			// check if file exists
			FILE *fp = fopen(prename, "rb");
			if(fp) {
				fread((char*)prebuffer, 1, 256 * 128 * 4, fp);
				fclose(fp);
			} else 
			if(emptyslot)
				memcpy(prebuffer, (char*)emptyslot->data, 256 * 128 * 4);
			else
				memset(prebuffer, 0xff , 256 * 128 * 4);    // white rectangle if no img loaded
 			loadslot = gui_LoadSlot; // do not load img file each time
		}
	} else {    //show...
		memset(prebuffer, 0x0 , 256 * 128 * 4);    //empty for now, maybe show logo?
        loadslot = -1;  //force reload of preview next time (disable if logo in seperate texture)

	}
}

/*
	Shows menu items and pointing arrow
*/
void ShowMenu(MENU *menu)
{
	int i;
	MENUITEM *mi = menu->m;
	int boxColor;
	
	    switch (Handy_cfg_BGColor) {
		case 0:
		case 1:
		    boxColor = 0x4444aaff;
			break;
		case 2:
		    boxColor = 0x44aa44ff;
			break;
		case 3:
		    boxColor = 0xaa4444ff;
			break;
	}


	// show menu lines
	for(i = 0; i < menu->itemNum; i++, mi++) {
		int fg_color;
		sf2d_draw_rectangle(10, 35 + i*26, 125, 19, (menu->itemCur == i)?boxColor:COLOR_INACTIVE_ITEM_BG); 
		if(menu->itemCur == i) fg_color = COLOR_ACTIVE_ITEM; else fg_color = COLOR_INACTIVE_ITEM;
		ShowMenuItem(7, 24 + i * 13, mi, fg_color,(menu == &gui_MainMenu)?0:1);
	}

	// show preview screen
	ShowPreview(menu);

	// print info string

    sftd_draw_text(font, 6, 11, 0x666666ff, 16, "Handy 3ds 1.0 alpha");
    sftd_draw_text(font, 5, 10, 0xffffffff, 16, "Handy 3ds 1.0 alpha");

	if (menu == &gui_MainMenu)
		sftd_draw_text(font, 40, 115, COLOR_HELP_TEXT, 10, "Press B to return to game");
	else
		sftd_draw_text(font, 25, 115, COLOR_HELP_TEXT, 10, "Press B to return to previous menu");
 }


void gui_DrawTopScreen() {

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

	sf2d_texture *temptext;
		
	(prevscreen)?temptext=prevscreen:temptext=insertgame;

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		
	if(background && Handy_cfg_ShowBackground && ((Handy_cfg_scalemode <3) || (Handy_rotation&1)) )
		sf2d_draw_texture_rotate_cut_scale(background, 0, 0, 0, 0, 0, background->width, background->height, bgstretchx, bgstretchy);

    switch (Handy_rotation) {
		case 0:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_rotate_cut_scale(temptext, 120, 69, 0, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
					sf2d_draw_texture_rotate_cut_scale(temptext, 40, 18, 0, 0, 0, 160, 102, 2, 2); //2x
					break;
				case 2:
					sf2d_draw_texture_rotate_cut_scale(temptext, 12, 0, 0, 0, 0, 160, 102, stretchy, stretchy);    //fit
					break;
				case 3:
				default:
					sf2d_draw_texture_rotate_cut_scale(temptext, 0, 0, 0, 0, 0, 160, 102, stretchx, stretchy);  //stretch
					break;
			}
			break;
		case 1:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_rotate_cut_scale(temptext, 120, 69, 1.57, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
				case 2:
				case 3:
				default:
					sf2d_draw_texture_rotate_cut_scale(temptext, 80,44, 1.57, 0, 0, 160, 102, stretchr, stretchr);    //fit
					break;
			}
			break;
		case 2:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_rotate_cut_scale(temptext, 120, 69, 3.14, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
					sf2d_draw_texture_rotate_cut_scale(temptext, 40, 18, 3.14, 0, 0, 160, 102, 2, 2); //2x
					break;
				case 2:
					sf2d_draw_texture_rotate_cut_scale(temptext, 12, 1, 3.14, 0, 0, 160, 102, stretchy, stretchy);    //fit
					break;
				case 3:
				default:
					sf2d_draw_texture_rotate_cut_scale(temptext, 0, 0, 3.14, 0, 0, 160, 102, stretchx, stretchy);  //stretch
					break;
			}
			break;
		case 3:
		default:
			switch (Handy_cfg_scalemode) {
				case 0:
					sf2d_draw_texture_rotate_cut_scale(temptext, 120, 69, 4.71, 0, 0, 160, 102, 1, 1); //1x
					break;
				case 1:
				case 2:
				case 3:
				default:
					sf2d_draw_texture_rotate_cut_scale(temptext, 80, 44, 4.71, 0, 0, 160, 102, stretchr, stretchr);    //fit
					break;
			}
			break;
	}

	if (prevscreen){
		sf2d_draw_rectangle(128, 98, 144, 44, 0xffffffff); 
		sf2d_draw_rectangle(130, 100, 140, 40, 0x000044ff); 
		sftd_draw_text(font, 73, 63, 0xffffffff, 16, "GAME PAUSED");
	}
       
	sf2d_end_frame();
}

/*
	Main function that runs all the stuff
*/
void gui_MainMenuRun(MENU *menu)
{
    APP_STATUS status;
	MENUITEM *mi;

	done = FALSE;

	while(!done) {
		mi = menu->m + menu->itemCur; // pointer to highlite menu option

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
				return;
			}

		}
        
        hidScanInput();
        u32 keydown = hidKeysDown();
		if (keydown & KEY_A) 
            if(mi->itemOnA != NULL) {
                gui_ClearScreen();
                (*mi->itemOnA)();
            }
		// B - exit or back to previous menu
		if (keydown & KEY_B) 
            return;
		// UP - arrow down
		if (keydown & KEY_UP)  
            if(--menu->itemCur < 0) menu->itemCur = menu->itemNum - 1;
		// DOWN - arrow up
		if (keydown & KEY_DOWN)  
            if(++menu->itemCur == menu->itemNum) menu->itemCur = 0;
		// LEFT - decrease parameter value
        if (keydown & KEY_LEFT) {  
			if(mi->itemPar != NULL && *mi->itemPar > 0) *mi->itemPar -= 1;
		}
		// RIGHT - increase parameter value
        if (keydown & KEY_RIGHT) {  
			if(mi->itemPar != NULL && *mi->itemPar < mi->itemParMaxValue) *mi->itemPar += 1;
		}
 
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        if(!done) {
            ShowMenu(menu); // show menu items
 			if (menu == &gui_MainMenu && (menu->itemCur == 1 || menu->itemCur == 2)) {
				sf2d_draw_rectangle(146, 53, 164, 106, 0xffffffff); 
				sf2d_draw_texture_part(previewtex, 148 , 55, 0, 0, 160, 102);
				int countslot;
				static char slot_str[2] = { 0,0 };
				for(countslot=0; countslot<10;countslot++) {
					slot_str[0] = '0' + countslot;
					sftd_draw_text(font, 81 + 7 * countslot, 87 , (countslot==gui_LoadSlot)?COLOR_ACTIVE_ITEM:COLOR_INACTIVE_ITEM, 10, slot_str);
				}
			} else 
				sf2d_draw_texture_part(previewtex, 148 , 55, 0, 0, 160, 102);
        }
                
        sf2d_end_frame();
                
        gfxFlushBuffers();

		gui_DrawTopScreen();
 
        gfxFlushBuffers();
        sf2d_swapbuffers();
	}
}

void gui_Run()
{
	extern int BT_A, BT_B; // remove later, temporal hack
    

	gui_ClearScreen();
	gui_MainMenuRun(&gui_MainMenu);
	if(Handy_cfg_SwapAB == 0) {
		BT_A = KEY_A;
		BT_B = KEY_B;
	} else {
		BT_A = KEY_B;
		BT_B = KEY_A;
	} //!!might want to add button swapping back in later?
	gui_ClearScreen();
}

void gui_ConfigMenuRun()
{
	gui_MainMenuRun(&gui_ConfigMenu);
}

void gui_ConfigScreenRun()
{
	gui_MainMenuRun(&gui_ConfigScreen);
}

void gui_Reset()
{
	mpLynx->Reset();
	done = TRUE; // mark to exit
}

void strncpy_u2a(char* dst, u16* src, int n)
{
    int i = 0;
    while (i < n && src[i] != '\0')
    {
        if (src[i] & 0xFF00)
            dst[i] = 0x7F;
        else
            dst[i] = (char)src[i];
        
        i++;
    }
    
    dst[i] = '\0';
}








