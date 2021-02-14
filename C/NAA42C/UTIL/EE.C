/********************************* EE.C ***************************************
			Equation Evaluator v1.0
*******************************************************************************

Program: EE.C

Purpose:
  Equation Evaluator familiarization tool.  Use to familiarize those new to
  the C programming language with writing mathematical equations.  All math
  functions defined in the standard C library header <math.h> are valid.
  Includes unary "+" if ANSI is set to 1 (TRUE).  Uses infix notation.
  Spaces are allowed when entering the equations.

Usage:  ee [optional equation]

Example:  ee cos(x + 3.14) - 2

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
******************************************************************************/

char tmpstr[130];		/* A temporary string.          */
int eqeval;			/* Needed to compile "eqeval.c" */

#include <stdio.h>		/* For printf() and scanf()     */
#include <stdlib.h>		/* For strtod()                 */
#include <stdarg.h>		/* For va_arg() in eval_eq()    */
#include "eqeval.c"		/* For parse_eq() and eval_eq() */


main (argc, argv)
int argc;
char *argv[];
{
  int i, j, has_vars;
  double x, y, t;

  /* Identify program and copyright information */
  printf("Equation Evaluator Version 1.0\n");
  printf("Copyright (C) 1991, Harold A. Toomey.  All rights reserved.\n\n");

  /* Get the equation */
  if (argc == 1) {		/* For no command line arguments. */
    printf("Enter an equation using x, y and/or t as variables:\n----> ");
    fgets(tmpstr, MAX_LINE_SIZE, stdin);
  } else {
    strcpy(tmpstr, argv[1]);	/* Get entire equation from command line */
    for (i=2;i<argc;i++)	/* even if entered with spaces. */
      strcat(tmpstr, argv[i]);
  }

  /* Parse the equation into a binary tree */
  parse_eq();

  /* Set has_vars to TRUE if the equation contains any variables */
  has_vars = FALSE;
  for (i=0;i<MAX_NUM_OPS;i++) {
    if ((eq_node[i]->id == VARX_ID) ||
        (eq_node[i]->id == VARY_ID) ||
        (eq_node[i]->id == VART_ID)) {
      has_vars = TRUE;
      break;
    }
  }

  /* Evaluate the equation.  If it has any variables, get their values. */
  if (has_vars == TRUE) {
    do {

      /* Get values of x, y and t separated by spaces */
      printf("Enter values for x, y and t separated by spaces (x y t): ");
      fgets(tmpstr, MAX_LINE_SIZE, stdin);

      /* Count how many numbers were entered */
      i = 0;
      if (tmpstr[0] != '\n') {
        for (j=0;isspace(tmpstr[j]);j++)	/* Skip leading spaces */
          ;
        for (;j<strlen(tmpstr);j++)
          if (isspace(tmpstr[j]))
            if (!isspace(tmpstr[j+1]))		/* Skip extra spaces */
              i++;				/* Found a word */
      }

      switch (i) {		/* Zero out unused variables */
        case 0:  x = 0.0;
        case 1:  y = 0.0;
        case 2:  t = 0.0;
        default: break;
      }  

      switch (i) {		/* Print f(), f(x), f(x,y), or f(x,y,t) */
        case 0: printf("f() = %.11lg\n", eval_eq(0));
                break;
        case 1: sscanf(tmpstr, "%lf", &x);
                printf("f(%lg) = %.11lg\n", x, eval_eq(x,y,t));
                break;
        case 2: sscanf(tmpstr, "%lf%lf", &x, &y);
                printf("f(%lg,%lg) = %.11lg\n", x, y, eval_eq(x,y,t));
                break;
        default: sscanf(tmpstr, "%lf%lf%lf", &x, &y, &t);
                printf("f(%lg,%lg,%lg) = %.11lg\n", x, y, t, eval_eq(x,y,t));
                break;
      }

      printf("Evaluate the equation again? (Y/N) N: ");
      fgets(tmpstr, MAX_LINE_SIZE, stdin);

    } while (tolower(tmpstr[0]) == 'y');

  } else
    printf("f() = %.11lg\n", eval_eq(0));

}

/*****************************************************************************/
/*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.           */
/*****************************************************************************/
