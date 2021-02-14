/******************************************************************************
			sp.h (ShorthandPlus)
******************************************************************************/

#include <bios.h>		/* For bioskey().  May not need later on. */
#include <conio.h>		/* For kbhit(), getch(), etc. */
#include <ctype.h>		/* For tolower(). */
#include <io.h>			/* For creatnew() and close(). */
#include <stdio.h>		/* For fputs() and file manipulation. */
#include <string.h>		/* For strcmp(), stricmp(), strcat(), etc. */

#define	MAXABBR	20		/* Maximum abbreviation size. */
#define	MAXREPSTR 200		/* Maximum replacement string size. */
#define	MAXNUMABBR 2000		/* Max. number of abbrev. to load into mem. */

#define	SPPATH	"\\SP\\"	/* Default path for ShorthandPlus files. */
#define	SPFILE	"spfile"	/* Default file for ShorthandPlus abbrevs. */

#define	BS	14		/* Backspace scan code. */
#define	RETURN	28		/* Return or Enter scan code. */
#define ALT	56		/* raw scan code for alt key. */
#define F1      59		/* Used to exit program. */
#define	LETTER_m 50		/* m's scan code. */

struct SPLIST {			/* Search and Replace structure. */
  char *abbrev;				/* Abbreviation. */
  char *repstr;				/* Replacement string. */
};

/*
EXAMPLE: struct SPLIST splist[MAXNUMABBR];
splist[1].abbrev = "byu";
splist[1].repstr = "Brigham Young University";
*/

typedef unsigned char boolean;


/* Function Prototypes. */
/* MAIN */
void check_switches(int argc, char *argv[]);
struct SPLIST *binsearch(char *abbrev);
void replace_abbrev(struct SPLIST *p);

/* MODES */
void menu(void);
void show_keystrokes(void);
void show_settings(void);

/* SPLIST */
add_to_list(void);
void delete_from_list(void);
void edit_list(void);
void print_list(void);
int save_list(char *fname);
void show_list(void);

/* SPFILE */
void create_file(void);
int load_list(char *fname);
void load_new_list(void);
int combine_lists(void);
int sort_file(void);
void delete_file(void);

/* HELP */
void tutorial(void);
void file_analyzer(void);
void help(void);

/* OTHER */
void check_fname(char *fname);
void make_full_path(char *fname);
void clear_list(void);

/*****************************************************************************/
