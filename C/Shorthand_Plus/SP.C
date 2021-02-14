/******************************************************************************
			sp.c (ShorthandPlus)
******************************************************************************/

#include "sp.h"

struct SPLIST splist[MAXNUMABBR];	/* Allocates an array for splist. */

/* MODES */
boolean ACTIVE=1;	/* 1 = replace abbrev., 0 = leave all abbrev. alone. */
boolean BEEP=1;		/* 1 = beep when replacing abbrev., 0 = no beep. */
boolean CASESEN=0;	/* 1 = case sensitive, 0 = case insensitive. */
boolean AUTOSAVE=1;	/* 1 = Save changes automat., 0 = ask to save. */

/* FLAGS */
boolean DIRTY=0;	/* 1 = splist was modified but not saved to a file. */
boolean VERBOSE=1;	/* 1 = ask certain questions in the procedures. */
boolean HAVESPFILE=0;	/* 1 = Found spfile name from command-line. */
boolean HAVESPPATH=0;	/* 1 = Found sppath name from command-line. */

/* Global VARIABLES */
long kscount=0L;	/* Keystroke count variable. */
long kssaved=0L;	/* Keystrokes saved variable. */
int nabbrev;		/* Number of abbreviations in memory (splist). */
int abindx;		/* abbrev[] index. */

char sppath[100];	/* ShorthandPlus path for spfiles. */
char spfile[100];	/* ShorthandPlus file in memory. */
char spfile2[100];	/* A second ShorthandPlus file. */
FILE *spfilefp;		/* File pointer to spfile. */
FILE *spfile2fp;	/* File pointer to spfile2. */

char string[MAXREPSTR];	/* A VERY temporary place to hold a string. */
int ch;			/* A VERY temporary place to hold a character. */

/*****************************************************************************/
main(int argc, char *argv[])
{
  unsigned code;
  unsigned char scan, shift;
  boolean DONE=0;
  int ascii;
  char abbrev[MAXABBR+1];
  struct SPLIST *p;


  check_switches(argc, argv);	/* Checks command line switches. */

  /* Set the default path if no path was given at the command line. */
  if (!HAVESPPATH)  strcpy(sppath, SPPATH);

  /* Load abbreviation list.  Should be in main() of calling program, kb.c. */
  /* Assumed to be presorted. */
  if (!HAVESPFILE) {
    printf("Enter file name of abbreviation list.\n");
    printf("Press <RETURN> for \"%s\": ", SPFILE);
    gets(spfile);
    if (strlen(spfile) == 0)	/* Default to "spfile". */
      strcpy(spfile, SPFILE);
  }

  make_full_path(spfile);	/* Add full path to file to be opened. */

  /* Loads abbreviation list (splist) from a file (spfile). */
  nabbrev = load_list(spfile);
  printf("Loaded %d abbreviations from file \"%s\".\n", nabbrev, spfile);


  do {

    code = bioskey(0);		/* Waits for keyboard input. */
    scan = (unsigned char) (code >> 8);
    ascii = (unsigned char) code;
    shift = (unsigned char) bioskey(2);	/* Requests shift status. */

    kscount++;			/* Counts keystrokes. */

				/* Check for Alt-m to activate menu. */
    if (((shift & 0x08) == 0x08) && (scan == LETTER_m))
      menu();

    if ((shift & 0x0C) == 0) {	/* Enter this if CTRL or ALT is not pressed. */

      putch(ascii);		/* Output the character. */

      switch (scan) {
        case BS:		/* Special case if backspace. */
          putch(' ');
          putch('\b');
          if (abindx > 0)  abindx--;
          break;

        case RETURN:		/* Special case if return. */
          putch('\n');
          abindx = 0;
          break;

        case F1:		/* Press F1 to exit. */
          DONE = 1;
          break;

        default:		/* Build abbreviation. */
         if (ACTIVE)
            abbrev[abindx++] = ascii;
          break;
      }

      if (ACTIVE) {
        if (abindx > MAXABBR-1)	/* Accept MAXABBR letter abbrev. maximum. */
          abindx = 0;		/* Initialize abbrev[] index. */
        if (ascii == ' ') {	/* Time to replace abbrev. with a string. */
          abbrev[--abindx] = '\0'; /* Replace 'space' with the NULL char. */
          abindx = 0;		/* Initialize abbrev[] index. */

	  /* Look for a match.  If a match is found return a pointer to its */
	  /* structure, otherwise return NULL. */
          if ((p = binsearch(abbrev)) != NULL)
            replace_abbrev(p);	/* Replace abbrev. with replacement string. */
        }
      }
    }
  }  while (!DONE);

}  /* main */


/******************************************************************************
				check_switches()
Sets parameters and modes if specified as command-line switches.
******************************************************************************/
void check_switches(int argc, char *argv[])
{
  char ch0, ch1, ch2;
  int len;

  /* Check command line switches. */
  /* Example 1:   SP.EXE -AYES -BNO -CNO -FSPFILE3 -P\SP -SYES */	
  /* Example 2:   SP.EXE /Boff /aon -cON -fspfile3 /P\sp\ /sno */	
  /* Example 3:   SP.EXE -A -C -B -S */	/* Toggles defaults. */

  while (--argc > 0) {
    ++argv;
    if ((*argv[0] == '-') || (*argv[0] == '/')) { /* Found a switch. */

      ch0 = tolower(*++argv[0]);	/* For option (ie. B = BEEP) */
      ch1 = tolower(*++argv[0]);	/* For Yes/No and toggling the */
					/* default response. */
      ch2 = tolower(argv[0][1]);	/* For oN/oFf response. */

      switch (ch0) {

        case 'a':		/* ACTIVE YES/NO or ON/OFF switch. */
          if ((ch1 == 'n') || (ch2 == 'f') || (ch1 == '\0'))
            ACTIVE = 0;		/* Default is 1. */
          break;

        case 'b':		/* BEEP YES/NO or ON/OFF switch. */
          if ((ch1 == 'n') || (ch2 == 'f') || (ch1 == '\0'))
            BEEP = 0;		/* Default is 1. */
          break;

        case 'c':		/* CASE SENSITIVE YES/NO or ON/OFF switch. */
          if ((ch1 == 'y') || (ch2 == 'n') || (ch1 == '\0'))
            CASESEN = 1;	/* Default is 0. */
          break;

        case 's':		/* AUTOSAVE YES/NO or ON/OFF switch. */
          if ((ch1 == 'n') || (ch2 == 'f') || (ch1 == '\0'))
            AUTOSAVE = 0;		/* Default is 1. */
          break;

        case 'f':		/* Sets default spfile. */
          strcpy(spfile, argv[0]);
          make_full_path(spfile); /* Add full path to file to be opened. */
          HAVESPFILE = 1;	/* Flag indicating we have a default spfile. */
          break;

        case 'p':		/* Sets default sppath. */
          strcpy(sppath, argv[0]);
          len = strlen(sppath);
          if (sppath[len-1] != '\\')	/* Append a '\' to the path if it */
            sppath[len++] = '\\';	/* ...was not already placed there. */
          sppath[len] = '\0';
          HAVESPPATH = 1;	/* Flag indicating we have a default spfile. */
          break;

        default:
          printf("Unknown switch option encountered (-%c).\n", toupper(ch0));
          break;
      }
    }
  }
}  /* check_switches() */


/******************************************************************************
				binsearch()
Looks for a match in splist[0]...splist[n-1] when given an abbreviation.
Returns a pointer to the structure containing the abbreviation if a match is
found or NULL if a match is not found.  See p. 137 of "The C Programming
Language".  List is assumed to be presorted.
******************************************************************************/
struct SPLIST *binsearch(char *abbrev)
{
  int cond;
  struct SPLIST *low = &splist[0];
  struct SPLIST *high = &splist[nabbrev];
  struct SPLIST *mid;

  while (low < high) {
    mid = low + (high-low) / 2;
    if ((CASESEN) && ((cond = strcmp(abbrev, mid->abbrev)) < 0))
      high = mid;		/* Case sensitive search. */
    else if ((!CASESEN) && ((cond = stricmp(abbrev, mid->abbrev)) < 0))
      high = mid;		/* Case insensitive search. */
    else if (cond > 0)
      low = mid + 1;
    else
      return mid;
  }
  return NULL;

}  /* binsearch() */


/******************************************************************************
			replace_abbrev()
Replaces the abbreviation with a replacement string.
******************************************************************************/
void replace_abbrev(struct SPLIST *p)
{
  int j, len;

  /* Determine the number of characters in the abbreviation including the */
  /* trailing space. */
  len = strlen(p->abbrev) + 1;

  /* Delete abbreviation from screen including the trailing space. */
  for (j=1;j<=len;j++)  putch('\b');

  /* Now put the replacement string in its stead followed by the trailing */
  /* space. */
  fputs(p->repstr,stdout);
  putch(' ');

  if (BEEP)  putch('\a');	/* Beep */

  /* Add-up number of keystrokes saved. */
  kssaved += strlen(p->repstr) - len + 1;

}  /* replace_abbrev() */


/*****************************************************************************/
/******************************************************************************
				menu()
Gives a  maintinance menu (ie-adding SR words/files, editing SR words/files,
deleting SR words/files, sorting and quick access to all SR words/files, print
listings of SR words/files, toggle speaker, show keystroke count, etc.
******************************************************************************/
void menu()
{
  printf("\n");
  printf("----------------------------------- MENU --------------------------------------\n");
  printf("\n");
  printf("  -------------- MODES ----------------   -------------- HELP -----------------\n");
  printf("  T - Toggle ACTIVE/INACTIVE mode         G - Tutorial                         \n");
  printf("  B - Toggle Beep ON/OFF                  N - File ANalyzer and splist Builder \n");
  printf("  U - Toggle Case SENSITIVE/INSENSITIVE   H - Help \n");
  printf("  W - Toggle AUTOSAVE mode                         \n");
  printf("  K - Show keystrokes used/saved                   \n");
  printf("  Z - Show current settings                        \n");
  printf("\n");
  printf("   -------- LIST --------                 ------------- FILE ------------\n");
  printf("   A - Add to splist                      C - Create a new spfile        \n");
  printf("   D - Delete from splist                 L - Load new spfile into memory\n");
  printf("   E - Edit splist                        M - Combine (Merge) two spfiles\n");
  printf("   P - Print splist                       O - Sort an spfile             \n");
  printf("   F - Save splist                        R - Delete (Remove) a file     \n");
  printf("   S - Show splist     \n");
  printf("   X - Remove splist   \n");
  printf("\n");
  printf("-------------------------------------------------------------------------------\n");

  gets(string);			/* Read in response. */
  ch = tolower(string[0]);

  switch (ch) {

    /* ------------------------- MODES ------------------------------------- */
    case 't':		/* Toggle ACTIVE/INACTIVE mode. */
      ACTIVE = (ACTIVE ? 0 : 1);
      printf("\nShorthandPlus is %sACTIVATED.\n", ACTIVE ? "" : "IN");
      if (!ACTIVE)
        abindx = 0;	/* Initialize abbrev[] index. */
      break;

    case 'b':		/* Toggle Beep ON/OFF. */
      BEEP = (BEEP ? 0 : 1);
      printf("\nBeep %s.\n", BEEP ? "ON" : "OFF");
      break;

    case 'u':		/* Toggle case sensitivity ON/OFF. */
      CASESEN = (CASESEN ? 0 : 1);
      printf("\nIs case %sSENSITIVE.\n", CASESEN ? "" : "IN");
      break;

    case 'w':		/* Toggle AUTOSAVE ON/OFF. */
      AUTOSAVE = (AUTOSAVE ? 0 : 1);
      printf("\nAUTOSAVE is %sACTIVEATED.\n", AUTOSAVE ? "" : "IN");
      break;

    case 'k':		/* Show keystrokes used/saved. */
      show_keystrokes();
      break;

    case 'z':		/* Show current settings. */
      show_settings();
      break;

    /* ------------------------- SPLIST ------------------------------------ */
    case 'a':		/* Add to splist. */
      add_to_list();
      save_list(spfile);
      break;

    case 'd':		/* Delete from splist. */
      delete_from_list();
      save_list(spfile);
      break;

/*
    case 'e':		/* Edit splist. */
      edit_list();
      break;
*/

    case 'p':		/* Print splist. */
      print_list();
      break;

    case 'f':		/* Save splist to a file (spfile). */
      DIRTY = 0;		/* Keeps from prompting unneccesarily. */
      check_fname(spfile);	/* Asks for an another file name. */
      save_list(spfile);
      break;

    case 's':		/* Show splist. */
      show_list();
      break;

    case 'x':		/* Remove splist. */
      if (DIRTY)
        save_list(spfile);
      clear_list();
      break;

    /* ------------------------- SPFILE ------------------------------------ */
    case 'c':		/* Create a new spfile. */
      create_file();
      break;

    case 'l':		/* Load new spfile into memory. */
      load_new_list();
      break;

    case 'm':		/* Combine (Merge) two spfiles. */
      if (combine_lists())
        printf("File \"%s\" has been created from two files.\n", spfile2);
      break;

    case 'o':		/* Sort an spfile (proper format assumed). */
      if (sort_file())
        printf("File \"%s\" has been sorted and saved.\n", spfile2);
      break;

    case 'r':		/* Delete (Remove) a file (Delete it). */
      delete_file();
      break;

    /* ------------------------- HELP -------------------------------------- */
/*
    case 'g':		/* A tutorial. */
      tutorial();
      break;

    case 'n':		/* File Analyzer and List Builder. */
      file_analyzer();
      break;

    case 'h':		/* On line halp. */
      help();
      break;
*/

    /* ------------------------- OTHER ------------------------------------- */
    case '\0':		/* A <RETURN> only means exit from menu. */
      return;

    default:
      puts("\a\nInvalid menu option.\n");
      return;
  }

}  /* menu() */


/******************************************************************************
				show_keystrokes()
Gives information on how well ShorthandPlus is saving you keystrokes.  Also is
helpful for people who get paid by the number of keystrokes "they" type.
******************************************************************************/
void show_keystrokes()
{
  printf("\nKeystroke Count (Actual):  %ld\n", --kscount);
  printf("Keystrokes Saved Using SP: %ld\n", kssaved);
  printf("Effective Keystrokes Used: %ld\n", kscount + kssaved);

}  /* show_keystrokes() */


/******************************************************************************
				show_settings()
Shows the current mode settings.  Also idicates which file is in memory and
whether it has been saved or not after splist was modified.
******************************************************************************/
void show_settings()
{
  printf("\nShorthandPlus is %sACTIVATED.\n", ACTIVE ? "" : "IN");  
  printf("Beep is %s.\n", BEEP ? "ON" : "OFF");  
  printf("Text is CASE %sSENSITIVE.\n", CASESEN ? "" : "IN");  
  printf("AUTOSAVE is %sACTIVATED.\n", AUTOSAVE ? "" : "IN");  
  printf("File in memory: \"%s\"\n", spfile);
  printf("Abbreviations in memory: %d\n", nabbrev);
  if (DIRTY)
    printf("List in memory has been modifed but not saved to a file.\n");

}  /* show_settings() */


/******************************************************************************
				add_to_list()
Adds abbreviations and replacement strings to the splist in memory.  Will also
save the modified list to a file, if desired.
******************************************************************************/
add_to_list()
{
  char newabbrev[MAXABBR+1], newrepstr[MAXREPSTR+1];
  struct SPLIST *p;
  int indx, j, yn;
  boolean INSERT, REPLACE, HAVEREPSTR, EXISTS;

  do {

    INSERT = 1;		/* 1 = insert an abbrev. into splist, */
			/* 0 = do not insert an abbrev. */
    REPLACE = 1;	/* 1 = replace the abbrev. in splist, */
			/* 0 = do not replace the abbrev. */

    /* Can enter:  "abbrev" <RETURN> "replacement_string" <RETURN>  */
    /*  or enter:  "abbrev" <SPACE> "replacement_string" <RETURN>   */
    /*  when adding to the abbreviation list.                       */

    printf("Enter an abbreviation (%d char. max):\n\t", MAXABBR);
    scanf("%s", newabbrev);	/* Reads in first string only. */
    if (getchar() == ' ') {	/* Grab LF (From CR/LF) or <space> char. */
      gets(newrepstr);		/* Get entire line up to \n. */
      HAVEREPSTR = 1;		/* 1 = have read in the replacement string. */
    } else
      HAVEREPSTR = 0;

    /* Check if abbreviation already exists. */
    indx = 0;
    EXISTS = 0;
    if (CASESEN) {		/* Case sensitive search. */
      /* Do a sequential search to find adding point. */
      while ((strcmp(newabbrev, splist[indx].abbrev) > 0) && (indx < nabbrev))
        indx++;
      if (strcmp(newabbrev, splist[indx].abbrev) == 0)
        EXISTS = 1;
    } else {			/* Case insensitive search. */
      /* Do a sequential search to find adding point. */
      while ((stricmp(newabbrev, splist[indx].abbrev) > 0) && (indx < nabbrev))
        indx++;
      if (stricmp(newabbrev, splist[indx].abbrev) == 0)
        EXISTS = 1;
    }

    if (EXISTS) {
      INSERT = 0;		/* Do not insert.  May replace though. */
      printf("\aAbbreviation already exists.\n");
      printf("Its replacement string is: \"%s\"\n", splist[indx].repstr);
      printf("Replace it? (y/n): ");
      gets(string);
      yn = tolower(string[0]);
      if (yn == 'n')
        REPLACE = 0;		/* Do not replace the old entry. */
    }

    /* Alter list with new repstr and possibly new abbrev. */
    if (REPLACE) {

      if (nabbrev >= MAXNUMABBR) {	/* Is there room in list? */
        printf("\n\aList size has reached its maximum.");
        printf("  Can not add any more abbreviations.\n");
        return;
      }

      if (!HAVEREPSTR) {	/* Get repstr if we do not have it yet. */
        printf("Enter its replacement string (%d char. max):\n\t", MAXREPSTR);
        gets(newrepstr);	/* Get entire line up to \n. */
      }

      if (INSERT) {		/* Add new abbrev. and repstr. */

        /* Move all strings down a place from indx. */
        for (j=nabbrev;j>indx;j--) {
          splist[j].abbrev = splist[j-1].abbrev;
          splist[j].repstr = splist[j-1].repstr;
        }

        /* Add new abbrev. */
        splist[indx].abbrev = (char *) malloc(strlen(newabbrev)+1);
        strcpy(splist[indx].abbrev, newabbrev);

        /* Add new repstr. */
        splist[indx].repstr = (char *) malloc(strlen(newrepstr)+1);
        strcpy(splist[indx].repstr, newrepstr);

        nabbrev++;		/* Incriment abbrev. count. */

      } else {			/* Replace old repstr ONLY. */
        /* Free memory of old repstr. */
        free((char *) splist[indx].repstr);

        /* Add new repstr. */
        splist[indx].repstr = (char *) malloc(strlen(newrepstr)+1);
        strcpy(splist[indx].repstr, newrepstr);
      }

      DIRTY = 1;	/* 1 = splist has been altered and not saved. */
    }

    printf("Would you like to add any more abbreviations? (y/n): ");
    gets(string);
    ch = tolower(string[0]);

  } while (ch == 'y');

}  /* add_to_list() */


/******************************************************************************
				delete_from_list()
Deletes abbreviations and replacement strings from the splist in memory.  Is
always case sensitive.  Will also save the modified list to a file, if desired.
******************************************************************************/
void delete_from_list()
{
  char *theabbrev = string;	/* Alias to save space on the stack. */
  struct SPLIST *p;
  int indx, j, yn;

  do {

    printf("Enter the abbreviation to delete:\n\t");
    scanf("%s", theabbrev);	/* Reads in first string only. */
    getchar();			/* Clean-up after scanf() */

    /* Do a sequential search to find the abbrev. */
    indx = 0;
    while ((strcmp(theabbrev, splist[indx].abbrev) > 0) && (indx < nabbrev))
      indx++;

    /* Check if the abbreviation already exists. */
    if (strcmp(theabbrev, splist[indx].abbrev) == 0) {	/* Exists. */

      /* Free memory of old abbrev. and old repstr. */
      free((char *) splist[indx].abbrev);
      free((char *) splist[indx].repstr);

      /* Move all strings up a place from indx. */
      for (j=indx;j<nabbrev;j++) {
        splist[j].abbrev = splist[j+1].abbrev;
        splist[j].repstr = splist[j+1].repstr;
      }
      nabbrev--;		/* Update abbrev. count. */
      DIRTY = 1;	/* 1 = splist has been altered and not saved. */

    } else 			/* Does not exist. */
      printf("No match found for %s.\n", theabbrev);

    printf("Would you like to delete any more abbreviations? (y/n): ");
    gets(string);
    ch = tolower(string[0]);

  } while (ch == 'y');

}  /* delete_from_list() */


/******************************************************************************
				edit_list()
Edits abbreviations and/or replacement strings in the splist in memory.  Will
also save the edited list to a file, if desired.
******************************************************************************/
void edit_list()
{


}  /* edit_list() */


/******************************************************************************
				print_list()
Prints the splist in memory after saving it to file spfile.  Works best when
executed from the DOS prompt.
******************************************************************************/
void print_list()
{
  int success;

  DIRTY = 0;		/* Keeps from prompting unneccesarily. */
  success = save_list(spfile);	/* Save splist to spfile. */

  if (success) {
    printf("Splist in memory has been saved to file \"%s\".\n", spfile);
    strcpy(string, "print ");
    strcat(string, spfile);
    system(string);			/* Ask DOS to "PRINT FILE". */
  }

}  /* print_list() */


/******************************************************************************
				save_list()
Saves the splist in memory to a file, usually to "spfile".  Returns a 1 if
successful and 0 if unsuccessful.
******************************************************************************/
int save_list(char *fname)
{
  int i;
  char *ptr;
  FILE *fp;

  /* Ask only if not in AUTOSAVE mode and then only if file is DIRTY. */
  if (!AUTOSAVE && DIRTY) {
    printf("Would you like to save changes to a file? (y/n): ");
    gets(string);
    ch = tolower(string[0]);
    if (ch == 'n')  return 0;	/* Save was unsuccessful. */
  }
				/* Open the file in write mode. */
  if ((fp = fopen(fname, "w+t")) == NULL) {
    printf("Could not open file \"%s\".\n", fname);
    return 0;			/* Save was unsuccessful. */
  }

  for (i=0;i<nabbrev;i++) {
    fputs(splist[i].abbrev, fp);/* Write abbrev. */
    fputs("\n", fp);		/* Write \n. */
    fputs(splist[i].repstr, fp);/* Write repstr. */
    fputs("\n", fp);		/* Write \n. */
  }

  fclose(fp);			/* Close the file */
  DIRTY = 0;			/* Clear DIRTY flag.  File has be saved. */
  return 1;			/* Save was successful. */

}  /* save_list() */


/******************************************************************************
				show_list()
Prints the splist in memory to the screen.
******************************************************************************/
void show_list()
{
  int i;

  printf("\n--------------------- Abbreviation List ----------------------\n");
  for (i=0;i<nabbrev;i++) {
    if (i%24 == 23) {
      printf("-- more --");
      getch();				/* Wait for a keystroke (<space>) */
      printf("\b\b\b\b\b\b\b\b\b\b");	/*   then delete "-- more --" and */
      printf("           ");		/*   continue on. */
      printf("\b\b\b\b\b\b\b\b\b\b\b");
    }
    printf("%d\t%s\t\t%s\n", i+1, splist[i].abbrev, splist[i].repstr);
  }
  printf("--------------------------------------------------------------\n\n");

}  /* show_list() */


/******************************************************************************
				create_file()
Creates a new splist from the keyboard.  Will save the newly created list to
a file.
******************************************************************************/
void create_file()
{
/*
  There are three ways to create an spfile.

  METHOD 1:   (file)
    1.  Create a file containing abbreviations and replacement
        strings using any editor or word processor.
    2.  Use the format explained in the comments of sort_file().
    3.  Save the file as a DOS text file.
    4.  Load ShorthandPlus if not already loaded.
    5.  Sort the file (menu option O).
    6.  Load it into memory if desired (Menu option L).

  METHOD 2:   (keyboard)
    1.  Load shorthandPlus if not already loaded.
    2.  Load an empty spfile into memory. (pick any ficticous file name.)
    3.  Build a new list using menu option A for Add.
    4.  Save splist using menu option F.

  METHOD 3:   (this routine)
    1.  Prompt for a new file name.
    2.  Make sure it does not already exist.
    3.  Open this empty file.
    4.  Build a list using add_to_list().
    5.  Save splist using save_list().
*/

  int fh, i;

  printf("Enter name of spfile to create: ");
  gets(spfile2);
  if (strlen(spfile2) == 0)	/* Abort if no name was entered. */
    return;

  /* See if file already exists.  If so ask if it should be overwritten, */
  /* ...otherwise create it. */
  make_full_path(spfile2);	/* Add full path to file to be opened. */

  if ((fh = creatnew(spfile2, 0)) < 0) {
    printf("File \"%s\" already exists.\n", spfile2);
    printf("Should it be replaced?\n");
    gets(string);			/* Read in response. */
    ch = tolower(string[0]);
    if (ch == 'n')		/* Abort this routine if file is not to be */
      return;			/* ...overwritten or replaced. */
  }

  close(fh);			/* close file.  It was open in binary mode. */

  DIRTY = 0;			/* Clear DIRTY flag.  Suppresses prompt. */
  save_list(spfile);		/* Save list in memory to spfile. */
  clear_list();			/* Remove splist from memory. */

  add_to_list();		/* Add to splist.  Sorts as it adds. */
  DIRTY = 0;			/* Clear DIRTY flag.  Suppresses prompt. */
  save_list(spfile2);		/* Save new list to a file. */
  printf("File \"%s\" has been created with %d entries.\n", spfile2, nabbrev);

  clear_list();			/* Remove new list from memory. */
  load_list(spfile);		/* Load origional splist back into memory. */

}  /* create_file() */


/******************************************************************************
				load_list()
Reads from a file (spfile) to load the ShorthandPlus list (splist).  Returns
the number of abbreviation loaded into memory.
******************************************************************************/
int load_list(char *fname)
{
  struct SPLIST *p;
  int len;
  boolean whichone, FULL;

  /* Check for valid file name.  If valid, open to read in text mode, */
  /* otherwise print an error message. */
  if ((spfilefp = fopen(fname, "r+t")) == NULL) {
    printf("Could not open file \"%s\".\n", fname);
    return 0;
  }

  p = splist;			/* Point to beginning of splist array. */
  whichone = 0;			/* 0 = build abbrev, 1 = build repstr. */
  nabbrev = 0;			/* Initialize list count. */
  FULL = 0;			/* 1 = splist is full, 0 = not full. */

				/* Get a line from the file. */
  while ((!FULL) && (fgets(string, MAXREPSTR, spfilefp) != NULL)) {

    len = strlen(string);
    if (len == 1)		/* Just an extra <RETURN> out of place. */
      continue;			/* ...so ignore it and go on. */
    string[len-1] = '\0';	/* Replace newline with NULL character. */

    if (!whichone) {		/* Build abbreviation. */
      p->abbrev = (char *) malloc(len+1);
      strcpy(p->abbrev, string); /* Copy abbreviation into structure. */
      whichone = 1;
    } else {			/* Build replacement string. */
      p->repstr = (char *) malloc(len+1);
      strcpy(p->repstr, string); /* Copy replacement string into structure. */
      whichone = 0;
      nabbrev++;		/* Incriment abbrev. count. */
      p++;			/* Get pointer to next splist structure. */
    }
				/* Checks on upper limit for the number of */
    if (nabbrev == MAXNUMABBR) {	/* ... abbrev. to load into memory. */
      printf("List is full.  No more new abbreviations may be added.\n");
      FULL = 1;			/* Splist is full. */
    }

  }

  fclose(spfilefp);		/* Close file after reading it. */
  return (nabbrev);		/* Return number of abbrev. loaded into mem. */

}  /* load_list() */


/******************************************************************************
				load_new_list()
Loads a new file into memory (splist), removing the previous one.  (May allow
combine (merge) option later).
******************************************************************************/
void load_new_list()
{
  int i;

  printf("Enter new file name to load into memory: ");
  gets(spfile2);

  /* Abort if no new file name is given. */
  if (strlen(spfile2) == 0) {
    printf("Abbreviation list was NOT replaced.\n");
    return;
  }

  /* Check for valid file name. */
  make_full_path(spfile2);	/* Add full path to file to be opened. */
  if ((spfilefp = fopen(spfile2, "r+t")) == NULL) {
    printf("Could not open file \"%s\".\n", spfile2);
    printf("List in memory was not replaced.\n");
    return;
  }
  fclose(spfilefp);		/* Close valid file so load_list() can */
				/* ...reopen it. */

  clear_list();			/* Remove splist from memory. */
  nabbrev = load_list(spfile2);	/* Load new spfile. */
  DIRTY = 0;			/* Newly loaded splist is clean. */

  printf("File \"%s\" has been replaced with \"%s\".\n", spfile, spfile2);
  strcpy(spfile, spfile2);	/* Update global variable for spfile. */
  printf("Loaded %d abbreviations from file \"%s\".\n", nabbrev, spfile);

}  /* load_new_list() */


/******************************************************************************
				combine_lists()
Combines (merges) two spfiles into a single spfile.  The two lists need not be
presorted and they may contain duplicate entries.  (Any duplicate entries in
file1 are overwritten by the ones in file2).
******************************************************************************/
int combine_lists()
{
  FILE *fp1, *fp2;
  char file1[80], file2[80];
  int success;

  printf("Enter first file to combine: ");	/* Prompt for first file. */
  gets(file1);
  if (file1[0] == '\0') {
    printf("File not found.\n");
    return 0;
  }
  make_full_path(file1);

  if ((fp1 = fopen(file1, "rt")) == NULL) {	/* Open file1 */
    printf("Could not open file \"%s\".\n", file1);
    return 0;			/* Combine was unsuccessful. */
  }


  printf("Enter second file to combine: ");	/* Prompt for second file. */
  gets(file2);
  if (file2[0] == '\0') {
    printf("File not found.\n");
    return 0;
  }
  make_full_path(file2);

  if ((fp2 = fopen(file2, "rt")) == NULL) {	/* Open file2. */
    printf("Could not open file \"%s\".\n", file2);
    return 0;			/* Combine was unsuccessful. */
  }


  printf("Enter the combined file name: ");	/* Prompt for combined file. */
  gets(spfile2);
  if (spfile2[0] == '\0') {
    printf("File not found.\n");
    return 0;
  }
  make_full_path(spfile2);

  if ((spfile2fp = fopen(spfile2, "w+t")) == NULL) {	/* Open spfile2. */
    printf("Could not open file \"%s\".\n", spfile2);
    return 0;			/* Combine was unsuccessful. */
  }

  while ((ch = fgetc(fp1)) != EOF) /* Save file1 and file2 into spfile2. */
    fputc(ch, spfile2fp);
  while ((ch = fgetc(fp2)) != EOF)
    fputc(ch, spfile2fp);

  fclose(fp1);			/* Close all files. */
  fclose(fp2);
  fclose(spfile2fp);

  VERBOSE = 0;			/* So sort_file() will not ask questions. */
  success = sort_file();	/* Sort spfile2 to finish. */
  VERBOSE = 1;			/* So sort_file() can ask questions again. */

  return (success);		/* 1 = successful, 0 = failed. */

}  /* combine_lists() */


/******************************************************************************
				sort_file()
Sorts a file to make an spfile.  Sorts in alphabetical order by abbreviations.
File format must be:
	abbrev<RETURN>		(MAXABBR (20?) characters maximum.)
	repstr<RETURN>		(MAXREPSTR (200?) characters maximum.)
		:
		:
	(MAXNUMABBR (2000?) entry pairs maximum.)

Note:  Excessive <RETURN>s are ignored.  Also a <RETURN> must follow the last
       repstr.
******************************************************************************/
int sort_file()
{
  int len, indx, j;
  boolean whichone, FULL;
  struct SPLIST *p;

  if (VERBOSE) {
    printf("Enter file name to sort: ");	/* Prompt for file name. */
    gets(spfile2);
    if (spfile2[0] == '\0') {
      printf("File not found.\n");
      return 0;
    }
    make_full_path(spfile2);
  }

  if ((spfile2fp = fopen(spfile2, "r+t")) == NULL) {
    printf("Could not open file \"%s\".\n", spfile2);
    return 0;			/* Sort was unsuccessful. */
  }

  DIRTY = 0;			/* Clear DIRTY flag.  Suppresses prompt. */
  save_list(spfile);		/* Save list in memory to spfile. */
  clear_list();			/* Remove splist from memory. */

  whichone = 0;			/* 0 = build abbrev, 1 = build repstr. */
  FULL = 0;			/* 1 = splist is full, 0 = not full. */

  while ((!FULL) && (fgets(string, MAXREPSTR, spfile2fp) != NULL)) {

    len = strlen(string);
    if (len == 1)		/* Just an extra <RETURN> out of place. */
      continue;			/* ...so ignore it and go on. */
    string[len-1] = '\0';	/* Replace newline with NULL character. */

    if (!whichone) {		/* Build abbreviation. */

      /* Do a sequential search to find adding point.  If duplicate abbrev. */
      /* ...are found they are replaced by the latest one in the file. */
      /* (This type of search also finds the indx so entries may be shifted */
      /* ...to make room for new abbreviations.) */
      indx = 0;
      while (strcmp(string, splist[indx].abbrev) > 0)
        indx++;
      p = &splist[indx];

      /* Move all strings down a place from indx, if any. */
      for (j=nabbrev;j>indx;j--) {
        splist[j].abbrev = splist[j-1].abbrev;
        splist[j].repstr = splist[j-1].repstr;
      }

      p->abbrev = (char *) malloc(len+1);
      strcpy(p->abbrev, string); /* Copy abbreviation into structure. */
      whichone = 1;

    } else {			/* Build replacement string. */
      p->repstr = (char *) malloc(len+1);
      strcpy(p->repstr, string); /* Copy replacement string into structure. */
      whichone = 0;
      nabbrev++;		/* Incriment abbrev. count. */
    }
				/* Checks on upper limit for the number of */
    if (nabbrev == MAXNUMABBR) {	/* ... abbrev. to load into memory. */
      printf("List is full.  No more new abbreviations may be added.\n");
      FULL = 1;			/* Splist is full. */
    }

  }

  fclose(spfile2fp);		/* Close file after reading it. */

  DIRTY = 0;			/* Clear DIRTY flag.  Suppresses prompt. */
  save_list(spfile2);		/* Save sorted list to file. */

  clear_list();			/* Remove new list from memory. */
  load_list(spfile);		/* Load origional splist back into memory. */

  return 1;			/* Sort was successful. */

}  /* sort_file() */


/******************************************************************************
				delete_file()
Deletes an spfile (any file) from disk.
******************************************************************************/
void delete_file()
{
  printf("Enter file to delete: ");
  gets(spfile2);
  if (strlen(spfile2) == 0)	/* Abort if no file name is entered. */
    return;

  make_full_path(spfile2);	/* Add full path to file to be opened. */
  if (remove(spfile2) == 0)	/* Delete file. */
    printf("\"%s\" has been deleted.\n", spfile2);
  else				/* Could not delete the file. */
    printf("Can not delete file \"%s\".\n", spfile2);

}  /* delete_file() */


/******************************************************************************
				tutorial()
Walks user through a tutorial on how to use ShorthandPlus.
******************************************************************************/
void tutorial()
{


}  /* tutorial() */


/******************************************************************************
				file_analyzer()
Analyzes files and builds possible abbreviation lists.
******************************************************************************/
void file_analyzer()
{


}  /* file_analyzer() */


/******************************************************************************
				help()
On line help on using ShorthandPlus.
******************************************************************************/
void help()
{


}  /* help() */


/******************************************************************************
				check_fname()
Checks to see if you have another file in mind.  If so it saves it into fname.
Used in save_file().
******************************************************************************/
void check_fname(char *fname)
{
  printf("Save to \"%s\"? (y/n): ", fname);
  gets(string);		/* A <RETURN> response defaults to fname. */
  ch = tolower(string[0]);

  if (ch == 'n') {
    printf("Enter file name to save to: ");
    gets(string);
    if (string[0] == '\0') {
      printf("No file to open.\n");
      return;
    } else
      strcpy(fname, string);
    make_full_path(fname);
  }

}  /* check_fname() */


/******************************************************************************
				make_full_path()
Builds the full path for ShorthandPlus files.  Default path is declared in the
global variable section at the top of this program (\SP\).
******************************************************************************/
void make_full_path(char *fname)
{
  strcpy(string, sppath);		/* Start with the path */
  strcat(string, fname);		/* ...then append to it the file name */
  strcpy(fname, string);		/* ...and give it back to fname. */

}  /* make_full_path() */


/******************************************************************************
				clear_list()
Cleans a list by freeing the memory used for each abbrev. and repstr.
******************************************************************************/
void clear_list()
{
  int i;

  for (i=0;i<nabbrev;i++) {
    free((char *) splist[i].abbrev);	/* Free memory from abbrev. */
    free((char *) splist[i].repstr);	/* Free memory from repstr. */
  }

  nabbrev = 0;			/* Initialize abbrev. count. */

}  /* clear_list() */


/*****************************************************************************/
