/******************************************************************************
				MKMENU.C
		A color menu making program for ShorthandPlus.
******************************************************************************/

#include <conio.h>		/* For Text attributes and colors. */
#include <stdio.h>		/* For fopen(). */

/* Prototypes. */
void load_array(int nrows, int ncols, char _char[], char _attr[], char buf[]);


/*
*** COLOR CONSTANTS FROM <conio.h> ***

BLACK		WHITE
BLUE		LIGHTBLUE
GREEN		LIGHTGREEN
CYAN		LIGHTCYAN
RED		LIGHTRED
MAGENTA		LIGHTMAGENTA
BROWN		YELLOW
LIGHTGREY	DARKGRAEY
		BLINK
*/

#define	RB	((RED<<4) + BLUE)
#define	RR	((RED<<4) + LIGHTRED)
#define	RW	((RED<<4) + WHITE)
#define	WB	((LIGHTGRAY<<4) + BLUE)
#define	WR	((LIGHTGRAY<<4) + RED)
#define	WG	((LIGHTGRAY<<4) + GREEN)
#define	BW	((BLUE<<4) + WHITE)
#define	BY	((BLUE<<4) + YELLOW)
#define	BG	((BLUE<<4) + GREEN)

/* Screen parameters. */
#define SCRR		25	/* Number of screen rows. */
#define SCRC		80	/* Number of screen columns. */
#define	SCREENSIZE	SCRR*SCRC*2+1

/* Window parameters. */
#define WINR		12	/* Number of window rows. */
#define WINC		60	/* Number of window columns. */
#define	WINDOWSIZE	WINR*WINC*2+1


/*********************************** MENU ************************************/
char menu_char[] = "\
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\
º                      ³         ShorthandPlus 1.0        ³                    º\
ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\
º                      ³                                  ³                    º\
ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\
º                                                                              º\
ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\
º                                                                              º\
ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ MODES ÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ HELP ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\
º T - Toggle ACTIVE/INACTIVE mode   ³     G - Tutorial                         º\
º B - Toggle Beep ON/OFF            ³     N - File Analyzer and splist builder º\
º U - Toggle Case SENSITIVE/INSENS  ³     H - Help                             º\
º W - Toggle AUTOSAVE mode          ³                                          º\
º K - Show keystrokes used/saved    ³                                          º\
º Z - Show current settings         ³                                          º\
º                                   ³                                          º\
ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ LIST ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ FILE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\
º       A - Add to splist           ³     C - Create a new spfile              º\
º       D - Delete from splist      ³     L - Load new spfile into memory      º\
º       E - Edit splist             ³     M - Combine (Merge) two spfiles      º\
º       P - Print splist            ³     O - Sort an spfile                   º\
º       F - Save splist             ³     R - Delete (Remove) a file           º\
º       S - Show splist             ³                                          º\
º       X - Remove splist           ³                                          º\
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼";

char menu_attr[] = {
/*         1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20  21 22 23 24 25 26 27 28 29 30  31 32 33 34 35 36 37 38 39 40  41 42 43 44 45 46 47 48 49 50  51 52 53 54 55 56 57 58 59 60  61 62 63 64 65 66 67 68 69 70  71 72 73 74 75 76 77 78 79 80 */
/*  1 */  RB,RR,RW,WB,WR,WG,BW,BY,BG,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  2 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  3 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  4 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  5 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  6 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  7 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  8 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/*  9 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 10 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 11 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 12 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 13 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 14 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 15 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 16 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 17 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 18 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 19 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 20 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 21 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 22 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 23 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 24 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
/* 25 */  BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW, BW,BW,BW,BW,BW,BW,BW,BW,BW,BW,
};

/********************************** WINDOW ***********************************/
char win_char[] = "\
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\
º                     DELETE WINDOW                        º\
ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶\
º                                                          º\
º                                                          º\
º                                                          º\
º                                                          º\
º                                                          º\
º                                                          º\
º                                                          º\
º                                                          º\
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼";

char win_attr[] = {
/*         1  2  3  4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20  21 22 23 24 25 26 27 28 29 30  31 32 33 34 35 36 37 38 39 40  41 42 43 44 45 46 47 48 49 50  51 52 53 54 55 56 57 58 59 60  */
/*  1 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  2 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  3 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  4 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  5 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  6 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  7 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  8 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/*  9 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/* 10 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/* 11 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
/* 12 */  WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR, WR,WR,WR,WR,WR,WR,WR,WR,WR,WR,
};

/*****************************************************************************/


main()
{
  char menubuf[SCREENSIZE];	/* CHAR, ATTR, CHAR, ATTR, ... */
  char winbuf[WINDOWSIZE];	/* CHAR, ATTR, CHAR, ATTR, ... */
  int left, top, right, bottom;
  FILE *fp;


  /* Build menubuf from menu_char[] and menu_attr[]. */
  load_array(SCRR, SCRC, menu_char, menu_attr, menubuf);
  menubuf[SCREENSIZE-1] = '\0'; /* Makes a NULL terminated string for fputs(). */

  /* Dispalys the menu on screen. */
  left = 1;
  top = 1;
  right = SCRC+left-1;
  bottom = SCRR+top-1;
  puttext(left, top, right, bottom, menubuf);
  while( !kbhit() );


  /* Build winbuf from win_char[] and win_attr[]. */
  load_array(WINR, WINC, win_char, win_attr, winbuf);
  winbuf[WINDOWSIZE-1] = '\0'; /* Makes a NULL terminated string for fputs(). */

  /* Dispalys the window on screen. */
  left = 10;
  top = 6;
  right = WINC+left-1;
  bottom = WINR+top-1;
  puttext(left, top, right, bottom, winbuf);
  getchar();


  fp = fopen("\\SP\\SP.MNU", "w+r");	/* Saves the menus to a file. */
  fputs(menubuf, fp);
  fputs(winbuf, fp);
  fclose(fp);

}


/******************************************************************************
				load_array()
Builds an array with given characters and attributes of size nrow x ncol.
******************************************************************************/
void load_array(int nrows, int ncols, char _char[], char _attr[], char buf[])
{
  int i, j, k;

  for (i=0;i<nrows;i++)
    for (j=0;j<ncols;j++)
      for (k=0;k<2;k++)
        if (k == 0)  buf[2*(i + nrows*j) + k] = _char[i + nrows*j];
        else         buf[2*(i + nrows*j) + k] = _attr[i + nrows*j];

}

/*****************************************************************************/
