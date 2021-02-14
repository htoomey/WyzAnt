/******************************** NAAUTIL.C ***********************************
	     "Numerical Analysis Algorithms in C" Utilities I v4.2
******************************************************************************/

/*
**  This source file contains many useful procedures, some being used in all
**  of the "Numerical Analysis Algorithms in C" programs.
**
**  At least three of the #defines are needed for all of the supporting ".c"
**  programs.  These #defines are: TRUE, FALSE, and ANSI.  The supporting
**  ".c" files are:
**
**	naautil.c	complex.c	round.c
**	naautil2.c	eqeval.c	trunc.c
**	naautil3.c	gaussj.c
**
**  Many of these functions where derived from the book "Numerical Recipes
**  in C".
**
**  "Naautil.c" contains the most often used routines.  Most are dynamic
**  memory allocation routines used to create very flexible vectors and
**  matrices.
**
**  "Naautil2.c" contains less frequently used dynamic memory allocation
**  routines to create very flexible vectors, matrices, and cubes.
**  Currently used only by: "081.c" and "125.c"
**
**  "Naautil3.c" contains routines for complex vectors, matrices, and cubes.
**  Currently used only by: "027.c", "028a.c" and "081.c"
**
**  This file should be included (only once!) whenever the following
**  procedures or functions are used:
**
**    Return    Procedure
**    Type      Name             Description
**    ----------------------------------------------------------------------
**    void      naaerror      -  Numerical Analysis Algorithms Error Handler
**    int       printf2       -  Like printf() but writes to a file too
**    void      NAA_do_first  -  NAA initialization procedure
**    void      NAA_do_last   -  NAA final procedure
**
**    int*           ivector  -  Allocates a 1-D array of integers
**    float*         vector   -  Allocates a 1-D array of floats
**    double*        dvector  -  Allocates a 1-D array of doubles
**    long double*   ldvector -  Allocates a 1-D array of long doubles
**    fcomplex*      cvector  -  Allocates a 1-D array of fcomplex
**
**    int**          imatrix  -  Allocates a 2-D array of integers
**    float**        matrix   -  Allocates a 2-D array of floats
**    double**       dmatrix  -  Allocates a 2-D array of doubles
**    long double**  ldmatrix -  Allocates a 2-D array of long doubles
**    fcomplex**     cmatrix  -  Allocates a 2-D array of fcomplex
**
**    int***         icube    -  Allocates a 3-D array of integers
**    float***       cube     -  Allocates a 3-D array of floats
**    double***      dcube    -  Allocates a 3-D array of doubles
**    long double*** ldcube   -  Allocates a 3-D array of long doubles
**    fcomplex***    ccube    -  Allocates a 3-D array of fcomplex
**
**    void      free_ivector  -  Frees the allocated 1-D array memory
**    void      free_vector   -  Frees the allocated 1-D array memory
**    void      free_dvector  -  Frees the allocated 1-D array memory
**    void      free_ldvector -  Frees the allocated 1-D array memory
**    void      free_cvector  -  Frees the allocated 1-D array memory
**
**    void      free_imatrix  -  Frees the allocated 2-D array memory
**    void      free_matrix   -  Frees the allocated 2-D array memory
**    void      free_dmatrix  -  Frees the allocated 2-D array memory
**    void      free_ldmatrix -  Frees the allocated 2-D array memory
**    void      free_cmatrix  -  Frees the allocated 2-D array memory
**
**    void      free_icube    -  Frees the allocated 3-D array memory
**    void      free_cube     -  Frees the allocated 3-D array memory
**    void      free_dcube    -  Frees the allocated 3-D array memory
**    void      free_ldcube   -  Frees the allocated 3-D array memory
**    void      free_ccube    -  Frees the allocated 3-D array memory
*/

/*********************/
/* DEFINES AND FLAGS */
/*********************/

/*
**  The FLAGS below allow the users more flexibility when compiling and
**  running these Numerical Analysis Algorithms.  They can also be used to
**  make these programs more portable to different computer systems.
*/

#define	PI	3.14159265358979323846264338327950288419716939937510582097

#ifndef FALSE
#define FALSE		0	/* Define FALSE if not already defined.      */
#endif

#ifndef TRUE
#define TRUE		!FALSE	/* Define TRUE if not already defined.       */
#endif

#define	ANSI		TRUE	/* Set to TRUE if using an ANSI C standard   */
				/* compliant compiler (default).             */
				/* Set to FALSE if using an older C compiler */
				/* (Kernighan & Ritchie Style C).            */

#define	ANSI_FUNCT	FALSE	/* Set to TRUE if using ANSI's style for     */
				/* declaring functions.  Set to FALSE if     */
				/* using K&R style functions (default).      */
				/* (Set to TRUE if using THINK C 4.0 on a    */
				/* Macintosh.)                               */

#define TITLE_PROMPT	TRUE	/* Set to TRUE if you want to be prompted    */
				/* for an optional title at the start of     */
				/* each program (default).                   */
				/* Set to FALSE to disable the title prompt. */

#define	FILE_SAVE	TRUE	/* Set to TRUE to save output to a file      */
				/* (default).                                */
				/* Set to FALSE to create no output file.    */

#define EQ_EVAL		FALSE	/* Set to TRUE to be prompted for the use of */
				/* the Equation Evaluator at run-time.       */
				/* Set to FALSE if entering f(x) inside the  */
				/* C source code and re-compiling it.        */
				/* (default)                                 */

#define	NAAUTIL_OBJ	FALSE	/* Set to TRUE if using "naautil.c" as a     */
				/* pre-compiled object code file to be       */
				/* linked to at algorithm compile time.      */
				/* Set to FALSE if using "naautil.c" as an   */
				/* un-compiled include file (default).       */

/********************** Computer System Specific Flags ***********************/

#define	OLD_UNIX_OS	FALSE	/* TRUE if running on older UNIX systems.    */

#define NO_LONG_DOUBLES	TRUE	/* TRUE if your C compiler does not have     */
				/* the "long double" type.                   */
				/* (Set to TRUE for most VAX C compilers)    */

/*****************************************************************************/

#define	MAX_LINE_SIZE 130	/* Largest input line size accepted.      */
				/* Set to 130 for MS-DOS computers.       */


/*****************
 * INCLUDE FILES *
 *****************/

#include <math.h>		/* For math function prototypes.             */
#include <stdio.h>		/* For scanf(), printf() and fprintf().      */
#include <stdlib.h>		/* Needed for calloc(), free() and rand().   */

#if OLD_UNIX_OS == TRUE		/* For older UNIX C compilers       */
#include <varargs.h>		/* For vfprintf() used in printf2() */
#else
#include <stdarg.h>		/* For vfprintf() used in printf2() */
#endif

/*
**  NOTE: Including <ctype.h> for tolower() may causes errors in Microsoft C
**  5.0 for IBM PCs where tolower() is defined in the <stdlib.h> header file
**  as well.
*/


/********************
 * GLOBAL VARIABLES *
 ********************/

#if NAAUTIL_OBJ == FALSE	/* If "naautil.c" is not pre-compiled.    */
  FILE *file_id;		/*   Identifies the output file.          */
  char  tmpstr[MAX_LINE_SIZE];	/*   A temporary string.                  */
  int   eqeval = FALSE;		/*   Set if using the Equation Evaluator. */
#else				/* If "naautil.c" is pre-compiled.        */
  extern FILE *file_id;
  extern char  tmpstr[MAX_LINE_SIZE];
  extern int   eqeval;
#endif


/***********************
 * FUNCTION PROTOTYPES *
 ***********************/

#if ANSI == TRUE	/* ANSI STANDARD PROTOTYPING (Post-December 14, 1989)*/
  void           naaerror      (char error_text[]);
  int            printf2       (char *format, ...);
  void           NAA_do_first  (char *outfile);
  void           NAA_do_last   (char *outfile);
  double**       dmatrix       (int a, int b, int c, int d);
  float**        matrix        (int a, int b, int c, int d);
  double*        dvector       (int a, int b);
  float*         vector        (int a, int b);
  int*           ivector       (int a, int b);
  void           free_dmatrix  (double **m, int a, int b, int c, int d);
  void           free_matrix   (float **m, int a, int b, int c, int d);
  void           free_dvector  (double *v, int a, int b);
  void           free_vector   (float *v, int a, int b);
  void           free_ivector  (int *v, int a, int b);
  double         eval_eq       (double x, ...);
#else  /* ANSI */	/* OLDER STYLE PROTOTYPING (Pre-December 14, 1989) */
			/* For compatibility with older C compilers.       */
  void           naaerror();
  int            printf2();
  void           NAA_do_first();
  void           NAA_do_last();
  double**       dmatrix();
  float**        matrix();
  double*        dvector();
  float*         vector();
  int*           ivector();
  void           free_dmatrix();
  void           free_matrix();
  void           free_dvector();
  void           free_vector();
  void           free_ivector();
  double         eval_eq();
#endif	/* ANSI */


#if NAAUTIL_OBJ == FALSE	/* Include the routines below only if the */
				/* NAAUTIL_OBJ flag is set to FALSE.      */

/****************************
 * EQUATION EVALUATOR LOGIC *
 ****************************/

#if EQ_EVAL == TRUE		/* Needed for the Equation Evaluator's     */
#include "eqeval.c"		/* defines, global variables and functions */
#else  /* EQ_EVAL */

#if ANSI == TRUE
double eval_eq (double x, ...)	/* May need this "dummy" procedure */
{
  return 0.0;
}
#else
double eval_eq (x, ...)		/* May need this "dummy" procedure */
double x;
{
  return 0.0;
}
#endif  /* ANSI */

#endif  /* EQ_EVAL */


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* naaerror() - Numerical Analysis Algorithms standard error handler.        */
/*****************************************************************************/
void naaerror(error_text)
char error_text[];
{
  /* Print error message to the screen.  (Standard error device) */
  fprintf(stderr, "\n\"Numerical Analysis Algorithms in C\" run-time");
  fprintf(stderr, " error...\n%s\n", error_text);
  fprintf(stderr, "...now exiting to system...\n");
  exit(-1);			/* Exit the program. */
}


#if FILE_SAVE == FALSE

/*****************************************************************************/
/* Since variable arguments are so terribly non-portable, the below two      */
/* defines will allow the programs to work properly, but without the use of  */
/* the file_save option (or variable argument lists).  They turn printf2()   */
/* into tried-and-true printf() and they hide the usage of the file_id       */
/* variable by turning "fprintf(file_id, ...)" into "sprintf(tmpstr, ...)".  */
/* This causes a write to an ignored string instead of to a file.            */
/*****************************************************************************/

#define printf2		printf		/* For printf2() */
#define fprintf		sprintf		/* For fprintf(file_id, ...) */
#define file_id		tmpstr

#else	/* FILE_SAVE == TRUE */

#if OLD_UNIX_OS == TRUE			/* For older UNIX C compilers */

/*****************************************************************************/
/* printf2() - Like printf() but prints to the outfile (file_id) also, if    */
/*             the FILE_SAVE flag is set to TRUE.                            */
/*             Use this printf2() instead of the below printf2() if your C   */
/*             compiler does not have <stdarg.h>, but does have <varargs.h>. */
/*                                                                           */
/*             NOTE:  This code segment has not been tested.                 */
/*****************************************************************************/

/*
**  printf2() - OPTION #1 -- Uses #include <varargs.h>
*/

int printf2(va_alist)
va_list va_alist;
/* or   (w/out a ';')
va_decl
*/
{
  va_list args;
  char *format;
  int length;

  /* Note the one less parameter in va_start() than in the below printf2() */
  va_start(args);
  format = va_arg(args, char*);
  length = vprintf(format, args);
  vfprintf(file_id, format, args);
  va_end(args);
  return length;
}

#else  /* OLD_UNIX_OS */		/* For ANSI C compilers */

/*****************************************************************************/
/* printf2() - Like printf() but prints to the outfile (file_id) also, if    */
/*             the FILE_SAVE flag is set to TRUE.                            */
/*****************************************************************************/

/*
**  printf2() - OPTION #2 -- Uses #include <stdarg.h>
*/

#if ANSI_FUNCT == TRUE
int printf2(char *format, ...)
#else
int printf2(format, ...)
char *format;
#endif
{
  va_list args;
  int length;

  va_start(args, format);
  length = vprintf(format, args);	/* Prints to the screen (stdout).  */
  vfprintf(file_id, format, args);	/* Prints to a file (file_id).     */
  va_end(args);
  return length;
}

#endif	/* OLD_UNIX_OS */

#endif	/* FILE_SAVE */


/*****************************************************************************/
/* NAA_do_first() - Opens the output file if the FILE_SAVE flag is set to    */
/*                  TRUE, prints the NAA banner to the screen and output     */
/*                  file, then prompts for an optional title and the         */
/*                  optional Equation Evaluator.                             */
/*****************************************************************************/
void NAA_do_first(outfile)
char *outfile;
{
  int i;

#if FILE_SAVE == TRUE		/* Set to TRUE if saving output to a file. */
  /* Open the default output file initialized in the main program. */
  if ((file_id = fopen(outfile, "w")) == NULL) {
    sprintf(tmpstr, "Can not open the output file named \"%s\".", outfile);
    naaerror(tmpstr);
  }
#endif	/* FILE_SAVE */

  /* Print the Numerical Analysis Algorithms banner. */
  for (i=1;i<80;i++)  printf2("-");
  printf2("\n\t\t   \"Numerical Analysis Algorithms in C\" v4.2\n");
  for (i=1;i<80;i++)  printf2("-");
  printf2("\n\n");

#if TITLE_PROMPT == TRUE
  /* Prompt for an optional title to be placed into the output file. */
  printf("Enter an optional title [ie - Set 2.1,  Problem 2 a) ].\n----> ");
  fgets(tmpstr, MAX_LINE_SIZE, stdin);

  /* Print the optional title to the output file only if one was entered. */
  if (tmpstr[0] != '\n')
    fprintf(file_id, "%s\n", tmpstr);
  printf("\n");
#endif	/* TITLE_PROMPT */

#if EQ_EVAL == TRUE
  get_eq();		/* Get the equation and store it into "tmpstr." */
  if (eqeval == TRUE)	/* eqeval is set by get_eq().  If set, then     */
    parse_eq();		/* parse the equation into a usable structure.  */
#endif	/* EQ_EVAL */

}

/*****************************************************************************/
/* NAA_do_last() - Closes the default output file and informs the user of    */
/*                 its creation or update.                                   */
/*****************************************************************************/
void NAA_do_last(outfile)
char *outfile;
{

#if FILE_SAVE == TRUE
  if (fclose(file_id) == EOF) {
    sprintf(tmpstr, "Can not close the output file named \"%s\".", outfile);
    naaerror(tmpstr);
  }
  printf("\nOutput saved into file \"%s\".\n", outfile);
#endif	/* FILE_SAVE */

}

/*****************************************************************************/
/* dmatrix() - Allocates a double matrix with range [a..b][c..d].            */
/*****************************************************************************/
double **dmatrix(a,b,c,d)
int a,b,c,d;
{
  int i;
  double **m;

  /* allocate pointers to rows. */
  m = (double **) calloc((unsigned) (b-a+1), sizeof(double*));
  if (!m)
    naaerror("allocation failure 1 in dmatrix()");
  m -= a;

  /* allocate rows and set pointers to them. */
  for (i=a;i<=b;i++) {
    m[i] = (double *) calloc((unsigned) (d-c+1), sizeof(double));
    if (!m[i])
      naaerror("allocation failure 2 in dmatrix()");
    m[i] -= c;
  }

  return (m);		/* return pointer to array of pointers to rows. */
}

/*****************************************************************************/
/* matrix() - Allocates a float matrix with range [a..b][c..d].              */
/*****************************************************************************/
float **matrix(a,b,c,d)
int a,b,c,d;
{
  int i;
  float **m;

  /* allocate pointers to rows. */
  m = (float **) calloc((unsigned) (b-a+1), sizeof(float*));
  if (!m)
    naaerror("allocation failure 1 in matrix()");
  m -= a;

  /* allocate rows and set pointers to them. */
  for (i=a;i<=b;i++) {
    m[i] = (float *) calloc((unsigned) (d-c+1), sizeof(float));
    if (!m[i])
      naaerror("allocation failure 2 in matrix()");
    m[i] -= c;
  }

  return (m);		/* return pointer to array of pointers to rows. */
}

/*****************************************************************************/
/* dvector() - Allocates a double vector with range [a..b].                  */
/*****************************************************************************/
double *dvector(a,b)
int a,b;
{
  double *v;

  v = (double *) calloc((unsigned) (b-a+1), sizeof(double));
  if (!v)
    naaerror("allocation failure in dvector()");
  return (v-a);
}

/*****************************************************************************/
/* vector() - Allocates a float vector with range [a..b].                    */
/*****************************************************************************/
float *vector(a,b)
int a,b;
{
  float *v;

  v = (float *) calloc((unsigned) (b-a+1), sizeof(float));
  if (!v)
    naaerror("allocation failure in vector()");
  return (v-a);
}

/*****************************************************************************/
/* ivector() - Allocates an integer vector with range [a..b].                */
/*****************************************************************************/
int *ivector(a,b)
int a,b;
{
  int *v;

  v = (int *) calloc((unsigned) (b-a+1), sizeof(int));
  if (!v)
    naaerror("allocation failure in ivector()");
  return (v-a);
}

/*****************************************************************************/
/* free_dmatrix() - Frees a matrix allocated with dmatrix().                 */
/*****************************************************************************/
void free_dmatrix(m,a,b,c,d)
double **m;
int a,b,c,d;			/* (variable d is never used.) */
{
  int i;

  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}

/*****************************************************************************/
/* free_matrix() - Frees a matrix allocated with matrix().                   */
/*****************************************************************************/
void free_matrix(m,a,b,c,d)
float **m;
int a,b,c,d;			/* (variable d is never used.) */
{
  int i;

  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}

/*****************************************************************************/
/* free_dvector() - Frees a double vector allocated by dvector().            */
/*****************************************************************************/
void free_dvector(v,a,b)
double *v;
int a,b;			/* (variable b is never used.) */
{
  free((char *) (v+a));
}

/*****************************************************************************/
/* free_vector() - Frees a float vector allocated by vector().               */
/*****************************************************************************/
void free_vector(v,a,b)
float *v;
int a,b;			/* (variable b is never used.) */
{
  free((char *) (v+a));
}

/*****************************************************************************/
/* free_ivector() - Frees an integer vector allocated by ivector().          */
/*****************************************************************************/
void free_ivector(v,a,b)
int *v,a,b;			/* (variable b is never used.) */
{
  free((char *) (v+a));
}


/*****************************************************************************/

#endif	/* NAAUTIL_OBJ */

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
