/******************************* NAAUTIL.H ************************************
	      "Numerical Analysis Algorithms in C" Utilities I v4.0
******************************************************************************/

/*
**      Return   Procedure
**      Value    Name            Description
**      -----------------------------------------------------------------
**	void     naaerror      -  Exits a program with an error message
**	void     get_title     -  Prompts for an optional title
**	void     open_outfile  -  Opens the default output file
**	void     close_outfile -  Closes the default output file
*/

/***********/
/* DEFINES */
/***********/

/*
**  The ANSI FLAG below allows the user more flexibility when compiling and
**  running these Numerical Analysis Algorithms.  Set to TRUE first.  Older
**  C compilers may require it to be set to FALSE.  No adverse effects should
**  occur.
*/

#ifndef TRUE
#define TRUE	1		/* Define TRUE if not already defined.       */
#endif

#ifndef FALSE
#define FALSE	0		/* Define FALSE if not already defined.      */
#endif

#define	ANSI		TRUE	/* Set to TRUE if using an ANSI C standard   */
				/* compliant compiler (default).             */
				/* Set to FALSE if using an older C compiler.*/
				/* ANSI = American National Standards        */
				/*        Institute                          */

/*****************
 * INCLUDE FILES *
 *****************/

#include <math.h>		/* For math function prototypes.             */
#include <stdio.h>		/* For scanf(), printf() and fprintf().      */

#if ANSI == TRUE
#include <stdlib.h>		/* Needed for calloc() by some compilers and */
#endif				/* for tolower().                            */


/********************
 * GLOBAL VARIABLES *
 ********************/

char title[133];		/* Used in get_title() and print_title().    */
FILE *file_id;			/* Stream used to save output into a file.   */


/***********************
 * FUNCTION PROTOTYPES *
 ***********************/

#if ANSI == TRUE
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989)    */
  void     naaerror      (char []);
  void     get_title     (void);
  void     open_outfile  (char *);
  void     close_outfile (char *);
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)       */
  /* Placed here for compatibility with older C compilers. */
  void     naaerror();
  void     get_title();
  void     open_outfile();
  void     close_outfile();
#endif


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* naaerror() - Numerical Analysis Algorithms standard error handler.        */
/*****************************************************************************/
void naaerror(error_text)
char error_text[];
{
  fprintf(stderr,"\nNumerical Analysis Algorithms run-time error...\n");
  fprintf(stderr,"%s", error_text);
  fprintf(stderr,"\n...now exiting to system...\n");
  exit (1);
}

/*****************************************************************************/
/* get_title() - Prints the NAA banner and prompts for an optional title.    */
/*****************************************************************************/
void get_title()
{
  int i;

  /* Print the Numerical Analysis Algorithms banner. */
  for (i=1;i<80;i++)  printf("-");
  printf("\n\t\t\"Numerical Analysis Algorithms in C\" v4.0\n");
  for (i=1;i<80;i++)  printf("-");
  printf("\n");

  printf("Enter a title [ie - Set 2.1,  Problem 2 a) ].\n----> ");
  fgets(title, 133, stdin);
  printf("\n");
}

/*****************************************************************************/
/* open_outfile() - Opens the default output file and prints the NAA banner  */
/*                  and optional title.                                      */
/*****************************************************************************/
void open_outfile(outfile)
char *outfile;
{
  char msg[80];			/* Holds an error message string */
  int i;

  /* Open the default output file. */
  if ((file_id = fopen(outfile, "w")) == NULL) {
    sprintf(msg, "Can not open the output file named \"%s\".", outfile);
    naaerror(msg);
  }

  /* Print the Numerical Analysis Algorithms banner. */
  for (i=1;i<80;i++)  fprintf(file_id, "-");
  fprintf(file_id, "\n\t\t\"Numerical Analysis Algorithms in C\" v4.0\n");
  for (i=1;i<80;i++)  fprintf(file_id, "-");
  fprintf(file_id, "\n\n");

  /* Print the optional title. */
  if (title[0] != '\n')
    fprintf(file_id, "%s\n", title);
}

/*****************************************************************************/
/* close_outfile() - Closes the default output file.                         */
/*****************************************************************************/
void close_outfile(outfile)
char *outfile;
{
  char msg[80];			/* Holds an error message string */

  if (fclose(file_id) == EOF) {
    sprintf(msg, "Can not close the output file named \"%s\".", outfile);
    naaerror(msg);
  }
}

/*****************************************************************************/
/*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0     */
/*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.           */
/*****************************************************************************/
