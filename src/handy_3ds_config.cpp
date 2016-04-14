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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "handy_3ds_config.h"

int Handy_cfg_scalemode = 0;
int Handy_cfg_Show_FPS = 0;
int Handy_cfg_Throttle = 1;
int Handy_cfg_SwapAB = 0;
int Handy_cfg_ShowBackground = 1;
int Handy_cfg_Sound = 1;
int Handy_cfg_BottomScr = 1;
int Handy_cfg_BGColor = 1;
int Handy_cfg_Language = 0;

/*
char const *Handy_3DS_String_Keys[] = 
    { 
		"[Language_Name]",
		"[wait]",
		"[pause]",
		"[Exit_Menu]",
		"[Show_Menu]",
		"[Back_to_Game]",
		"[Back_to_Main]",
		"[Back_to_Previous]",
		"[Load_Rom]",
		"[Load_State]",
		"[Save_State]",
		"[Reset_rom]",
		"[Config]",
		"[Exit]",
		"[Screen_Size]",
		"[Language]",
		"[Show_fps]",
		"[Limit_fps]",
		"[Swap:A/B]",
		"[Show_Background]",
		"[Save_config]",
		"[1x]",
		"[2x]",
		"[Fit]",
		"[Stretch]",
		"[Yes]",
		"[No]",
		"[Black]",
		"[Blue]",
		"[Green]",
		"[Red]"
	};
*/
	
char const *Handy_3DS_String_EN[] = 
    { 
		"English",
		"Wait please ...",
		"GAME PAUSED",
		"Press B to exit",
		"Touch the screen to show the menu",
		"Press B to return to game",
		"Press B to return to the main menu",
		"Press B to return to previous menu",
		"Load Rom",
		"Load State",
		"Save State",
		"Reset ROM",
		"Config",
		"Exit",
		"Screen config",
		"Language",
		"Sound",
		"Limit FPS",
		"Swap A/B",
		"Save config",
		"Screen Size",
		"Bottom Screen",
		"Show FPS",
		"Show background",
		"Background color",
		"1x",
		"2x",
		"Fit",
		"Stretch",
		"Yes",
		"No",
		"Black",
		"Blue",
		"Green",
		"Red"
	};    

char const *Handy_3DS_String_IT[] = 
    { 
		"Italiano",
		"Attendere ...",
		"GIOCO IN PAUSA",
		"Premi B per uscire",
		"Tocca lo schermo per il menu",
		"Premi B per tornare al gioco",
		"Premi B per tornare al menu principale",
		"Premi B per tornare al menu precedente",
		"Carica Rom",
		"Carica stato",
		"Salva stato",
		"Riavvia ROM",
		"Configura",
		"Esci",
		"Configura schermo",
		"Lingua",
		"Suono",
		"Limita FPS",
		"Scambia A/B",
		"Salva configurazione",
		"Dimensione schermo",
		"Schermo inferiore",
		"Mostra FPS",
		"Mostra sfondo",
		"Colore sfondo",
		"1x",
		"2x",
		"Adattato",
		"Esteso",
		"Sì",
		"No",
		"Nero",
		"Blu",
		"Verde",
		"Rosso"
	};    

char const *Handy_3DS_String_FR[] = 
    { 
"Français",
"Attendez s'il vous plaît ...",
"JEU EN PAUSE",
"Appuyez sur B pour sortir",
"Touchez l'écran pour afficher le menu",
"Appuyez B pour revenir au jeu",
"Appuyez B pour revenir au menu principal",
"Appuyez B pour revenir au menu précédent",
"Charger Rom",
"Charger l'état",
"Enregistrer l'état",
"Réinitialiser ROM",
"Configurer",
"Sortie",
"Configurer l'écran",
"Langue",
"Son",
"Limites FPS",
"Échange A / B",
"Enregistrer configuration",
"Taille de l'écran",
"Écran du bas",
"Montre FPS",
"Montre le fond",
"Couleur de fond",
"1X",
"2X",
"Adapté",
"Etendu",
"Oui",
"Non",
"Noir",
"Bleu",
"Vert",
"Rouge"
};

char const *Handy_3DS_String_DE[] =
  {
     "Deutsch",
     "Bitte warten...",
     "PAUSE",
     "Drücke B zum verlassen",
     "Bildschirm berühren um Menü anzuzeigen",
     "Drücke B um zum Spiel zurückzukehren",
     "Drücke B um zum Hauptmenü zurückzukehren",
     "Drücke B um zum vorherigen Menü zurückzukehren",
     "Spiel laden",
     "Spielstand laden",
     "Spielstand speichern",
     "Spiel zurücksetzen",
     "Konfigurieren",
     "Beenden",
     "Bildschirmkonfiguration",
     "Sprache",
     "Ton",
     "FPS limitieren",
     "A/B Tasten tauschen",
     "Konfiguration speichern",
     "Bildschirmgröße",
     "Unteren Bildschirm zeigen",
     "FPS anzeigen",
     "Hintergrund anzeigen",
     "Hintergrundfarbe",
     "1x",
     "2x",
     "Angepasst",
     "Gestreckt",
     "Ja",
     "Nein",
     "Schwarz",
     "Blau",
     "Grün",
     "Rot"
   };  
   
   char const *Handy_3DS_String_NL[] =
  {
     "Nederlands",
     "Wacht even ...", 
     "SPEL GEPAUZEERD",
     "Druk B om te stoppen", 
     "Raak het scherm aan voor het menu",
     "Druk B om terug naar het spel te gaan",
     "Druk B om terug naar het hoofdscherm te gaan",
     "Druk B voor het vorige menu",
     "Laad Rom",
     "Laad opgeslagen spel",
     "Bewaar spel",
     "Herstart ROM",
     "Configuratie",
     "Stoppen",
     "Scherm configuratie",
     "Taal",
     "Geluid",
     "Limiteer FPS",
     "Verwissel A/B",
     "Bewaar configuratie",
     "Scherm grootte",
     "Onderste Scherm",
     "Toon FPS",
     "Toon achtergrond",
     "Achtergrond kleur",
     "1x",
     "2x",
     "Aangepast",
     "Uitrekken",
     "Ja",
     "Nee",
     "Zwart",
     "Blauw",
     "Groen",
     "Rood"
   };


char const *gui_Languages[5] = 
	{
		Handy_3DS_String_EN[HANDY_STR_Language_ID],
		Handy_3DS_String_IT[HANDY_STR_Language_ID],
		Handy_3DS_String_FR[HANDY_STR_Language_ID],
		Handy_3DS_String_DE[HANDY_STR_Language_ID],
		Handy_3DS_String_NL[HANDY_STR_Language_ID]
	}; 

char Handy_3DS_String_list [35][HANDY_3DS_LANGUAGE_STRING_MAX_SIZE];

void set_language(void) {
	int i;
	if (Handy_cfg_Language == 4) 
		for(i=0; i<35; i++)
			strncpy(Handy_3DS_String_list[i], Handy_3DS_String_NL[i], HANDY_3DS_LANGUAGE_STRING_MAX_SIZE-1); 
	else if (Handy_cfg_Language == 3) 
		for(i=0; i<35; i++)
			strncpy(Handy_3DS_String_list[i], Handy_3DS_String_DE[i], HANDY_3DS_LANGUAGE_STRING_MAX_SIZE-1); 
	else if (Handy_cfg_Language == 2) 
		for(i=0; i<35; i++)
			strncpy(Handy_3DS_String_list[i], Handy_3DS_String_FR[i], HANDY_3DS_LANGUAGE_STRING_MAX_SIZE-1); 
	else if (Handy_cfg_Language == 1) 
		for(i=0; i<35; i++)
			strncpy(Handy_3DS_String_list[i], Handy_3DS_String_IT[i], HANDY_3DS_LANGUAGE_STRING_MAX_SIZE-1); 
	else
		for(i=0; i<35; i++)
			strncpy(Handy_3DS_String_list[i], Handy_3DS_String_EN[i], HANDY_3DS_LANGUAGE_STRING_MAX_SIZE-1); 
};

void do_config(char *file)
{

    /* Our token list */
    int i, argc;
    char *argv[0x100];


	set_option_defaults();
	
    for(i = 0; i < 0x100; i++) argv[i] = NULL;

    /* Check configuration file */
    if(file) 
	  if(parse_file(file, &argc, argv)) {

		/* Check extracted tokens */
		parse_args(argc, argv);

		/* Free token list */
		for(i = 0; i < argc; i++)
		{
			if(argv[argc]) free (argv[argc]);
		}
	}

    set_language(); // initialize strings

}


/* Parse configuration file */
int parse_file(const char *filename, int *argc, char **argv)
{
    char token[0x100];
    FILE *handle = NULL;

    *argc = 0;

    handle = fopen(filename, "r");
    if(!handle) return (0);

    while(!(feof(handle)))
    {
		fscanf(handle, "%s", &token[0]);
        int size = strlen(token) + 1;
        argv[*argc] = (char*) malloc(size);
        if(!argv[*argc]) return (0);
		sprintf(argv[*argc], "%s", token);
        *argc += 1;
    }

    if(handle) fclose(handle);
    return (1);
}


void set_option_defaults(void)
{
	Handy_cfg_scalemode = 0;
	Handy_cfg_Show_FPS = 0;
	Handy_cfg_Throttle = 1;
	Handy_cfg_SwapAB = 0;
	Handy_cfg_ShowBackground = 1;
	Handy_cfg_Sound = 1;
	Handy_cfg_BottomScr = 1;
	Handy_cfg_BGColor = 1;
}

/* Parse argument list */
void parse_args(int argc, char **argv)
{
    int i;

    for(i = 0; i < argc; i++)
    {
        int left = argc - i - 1;

        if(strcmp(argv[i], "-nosound") == 0)
        {
            Handy_cfg_Sound = 0;
        }

        if(strcmp(argv[i], "-scale") == 0 && left) 
        {
            Handy_cfg_scalemode = atoi(argv[i+1]);
		    if (Handy_cfg_scalemode > 4) Handy_cfg_scalemode = 0;

        }

        if(strcmp(argv[i], "-language") == 0 && left) 
        {
            Handy_cfg_Language = atoi(argv[i+1]);
		    if (Handy_cfg_Language > 3) Handy_cfg_Language = 0;

        }

        if(strcmp(argv[i], "-bgcolor") == 0 && left) 
        {
            Handy_cfg_BGColor = atoi(argv[i+1]);
		    if (Handy_cfg_BGColor > 4) Handy_cfg_BGColor = 0;

        }

        if(strcmp(argv[i], "-throttle") == 0)
        {
            Handy_cfg_Throttle = 1;
        }

        if(strcmp(argv[i], "-nothrottle") == 0)
        {
            Handy_cfg_Throttle = 0;
        }

        if(strcmp(argv[i], "-fps") == 0)
        {
            Handy_cfg_Show_FPS = 1;
        }

        if(strcmp(argv[i], "-swapAB") == 0)
        {
            Handy_cfg_SwapAB = 1;
        }

        if(strcmp(argv[i], "-background") == 0)
        {
            Handy_cfg_ShowBackground = 1;
        }

        if(strcmp(argv[i], "-nobackground") == 0)
        {
            Handy_cfg_ShowBackground = 0;
        }

        if(strcmp(argv[i], "-nobottomscr") == 0)
        {
            Handy_cfg_BottomScr = 0;
        }

    }
}


void save_config(char *file)
{
    FILE *handle = NULL;
    handle = fopen(file, "w+");

        if(Handy_cfg_Sound == 0)
        {
            fprintf(handle, "%s ", "-nosound");
        }

        if(Handy_cfg_scalemode) 
        {
            fprintf(handle, "%s %i ", "-scale", Handy_cfg_scalemode);
        }

        if(Handy_cfg_Language) 
        {
            fprintf(handle, "%s %i ", "-language", Handy_cfg_Language);
        }

        if(Handy_cfg_Throttle)
        {
            fprintf(handle, "%s ", "-throttle");
        } else {
            fprintf(handle, "%s ", "-nothrottle");
        }

        if(Handy_cfg_Show_FPS)
        {
            fprintf(handle, "%s ", "-fps");
        }

        if(Handy_cfg_SwapAB)
        {
            fprintf(handle, "%s ", "-swapAB");
        }

        if(Handy_cfg_ShowBackground)
        {
            fprintf(handle, "%s ", "-background");
        } else {
            fprintf(handle, "%s ", "-nobackground");
        }

        if(Handy_cfg_BottomScr == 0)
        {
            fprintf(handle, "%s ", "-nobottomscr");
        }

        fprintf(handle, "%s %i ", "-bgcolor", Handy_cfg_BGColor);

 
 		fclose(handle);
}
	
 

