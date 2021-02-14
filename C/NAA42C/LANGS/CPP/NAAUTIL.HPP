/****************************** NAAUTIL.HPP ***********************************
	    "Numerical Analysis Algorithms in C++" Utilities I v4.0
******************************************************************************/

//
//      Return   Procedure
//      Value    Name            Description
//      -----------------------------------------------------------------
//	void     naaerror      -  Exits a program with an error message
//	void     get_title     -  Prompts for an optional title
//	void     open_outfile  -  Opens the default output file
//	void     close_outfile -  Closes the default output file
//


/*****************
 * INCLUDE FILES *
 *****************/

#include <stdio.h>		// For fprintf().
#include <iostream.h>		// For cin, cout and cerr.
#include <stdlib.h>		// Needed for calloc() by some compilers and
				// for tolower().
#include <math.h>		// For math function prototypes.


/********************
 * GLOBAL VARIABLES *
 ********************/

char title[133];		// Used in get_title() and print_title().
FILE *file_id;			// Stream used to save output into a file.


/***********************
 * FUNCTION PROTOTYPES *
 ***********************/

void naaerror      (char []);
void get_title     (void);
void open_outfile  (char *);
void close_outfile (char *);


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* naaerror() - Numerical Analysis Algorithms standard error handler.        */
/*****************************************************************************/
void naaerror(char error_text[])
{
  cerr << "\nNumerical Analysis Algorithms run-time error...\n";
  cerr << error_text;
  cerr << "\n...now exiting to system...\n";
  exit (1);
}

/*****************************************************************************/
/* get_title() - Prints the NAA banner and prompts for an optional title.    */
/*****************************************************************************/
void get_title(void)
{
  int i;

  // Print the Numerical Analysis Algorithms banner.
  for (i=1;i<80;i++)  cout << "-";
  cout << "\n\t\t\"Numerical Analysis Algorithms in C++\" v4.0\n";
  for (i=1;i<80;i++)  cout << "-";
  cout << "\n";

  cout << "Enter an optional title [ie - Set 2.1,  Problem 2 a) ].\n----> ";
  fgets(title, 133, stdin);
  cout << "\n";
}

/*****************************************************************************/
/* open_outfile() - Opens the default output file and prints the NAA banner  */
/*                  and optional title.                                      */
/*****************************************************************************/
void open_outfile(char *outfile)
{
  char msg[80];			// Holds an error message string.
  int i;

  // Open the default output file.
  if ((file_id = fopen(outfile, "w")) == NULL) {
    sprintf(msg, "Can not open the output file named \"%s\".", outfile);
    naaerror(msg);
  }

  // Print the Numerical Analysis Algorithms banner.
  for (i=1;i<80;i++)  fprintf(file_id, "-");
  fprintf(file_id, "\n\t\t\"Numerical Analysis Algorithms in C++\" v4.0\n");
  for (i=1;i<80;i++)  fprintf(file_id, "-");
  fprintf(file_id, "\n\n");

  // Print the optional title.
  if (title[0] != '\n')
    fprintf(file_id, "%s\n", title);
}

/*****************************************************************************/
/* close_outfile() - Closes the default output file.                         */
/*****************************************************************************/
void close_outfile(char *outfile)
{
  char msg[80];			// Holds an error message string.

  if (fclose(file_id) == EOF) {
    sprintf(msg, "Can not close the output file named \"%s\".", outfile);
    naaerror(msg);
  }
}

/*****************************************************************************/
/*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0     */
/*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.           */
/*****************************************************************************/
