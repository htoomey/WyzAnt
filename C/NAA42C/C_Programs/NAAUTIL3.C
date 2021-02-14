/****************************** NAAUTIL3.C ************************************
	    "Numerical Analysis Algorithms in C" Utilities III v4.2
******************************************************************************/

/*
**  This file is very similar to "naautil.c".  It contains less frequently
**  used dynamic memory allocation routines to create very flexible complex
**  vectors, matrices, and cubes.
** 
**  It contains the following procedures and functions:
** 
**    Return         Procedure
**    Type           Name            Description
**    ---------------------------------------------------------------------
**    fcomplex***    ccube         -  Allocates a 3-D array of fcomplex
**    fcomplex**     cmatrix       -  Allocates a 2-D array of fcomplex
**    fcomplex*      cvector       -  Allocates a 1-D array of fcomplex
**
**    void           free_ccube    -  Frees the allocated 3-D array memory
**    void           free_cmatrix  -  Frees the allocated 2-D array memory
**    void           free_cvector  -  Frees the allocated 1-D array memory
** 
**  These functions were derived from the text "Numerical Recipes in C".
*/


/*****************
 * INCLUDE FILES *
 *****************/

/* NOTE:  Requires that "naautil.h" be included only once! */
#include "complex.c"		/* Needed for complex arithmetic. */


/************************
 * FUNCTION PROTOTYPING *
 ************************/

#if ANSI == TRUE		/* Set this flag once in "naautil.h"         */
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989)                      */
  fcomplex***    ccube         (int a, int b, int c, int d, int e, int f);
  fcomplex**     cmatrix       (int a, int b, int c, int d);
  fcomplex*      cvector       (int a, int b);
  void           free_ccube    (fcomplex ***m, int a, int b, int c, int d,
                                int e, int f);
  void           free_cmatrix  (fcomplex **m, int a, int b, int c, int d);
  void           free_cvector  (fcomplex *v, int a, int b);
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)                         */
  /* Placed here for compatibility with older C compilers.                   */
  fcomplex***    ccube();
  fcomplex**     cmatrix();
  fcomplex*      cvector();
  void           free_ccube();
  void           free_cmatrix();
  void           free_cvector();
#endif


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* ccube() - Allocates a complex cube with range [a..b][c..d][e..f].         */
/*           Like cmatrix() but for three dimensional arrays.                */
/*****************************************************************************/
fcomplex ***ccube(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
  int i,j;
  fcomplex ***m;

  m=(fcomplex ***) calloc((unsigned) (b-a+1), sizeof(fcomplex **));
  if (!m) naaerror("allocation failure 1 in ccube()");
  m -= a;

  for (i=a;i<=b;i++) {
    m[i]=(fcomplex **) calloc((unsigned) (d-c+1), sizeof(fcomplex *));
    if (!m[i]) naaerror("allocation failure 2 in ccube()");
    m[i] -= c;
  }

  for (i=a;i<=b;i++)
    for (j=c;j<=d;j++) {
      m[i][j]=(fcomplex *) calloc((unsigned) (f-e+1), sizeof(fcomplex));
      if (!m[i][j]) naaerror("allocation failure 3 in ccube()");
      m[i][j] -= e;
    }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* cmatrix() - Allocates a complex matrix with range [a..b][c..d].           */
/*****************************************************************************/
fcomplex **cmatrix(a,b,c,d)
int a,b,c,d;
{
  int i;
  fcomplex **m;

  /* allocate pointers to rows. */
  m=(fcomplex **) calloc((unsigned) (b-a+1), sizeof(fcomplex*));
  if (!m) naaerror("allocation failure 1 in cmatrix()");
  m -= a;

  /* allocate rows and set pointers to them. */
  for (i=a;i<=b;i++) {
    m[i]=(fcomplex *) calloc((unsigned) (d-c+1), sizeof(fcomplex));
    if (!m[i]) naaerror("allocation failure 2 in cmatrix()");
    m[i] -= c;
  }

  return (m);		/* return pointer to array of pointers to rows. */
}

/*****************************************************************************/
/* cvector() - Allocates a complex vector with range [a..b].                 */
/*****************************************************************************/
fcomplex *cvector(a,b)
int a,b;
{
  fcomplex *v;

  v = (fcomplex *) malloc((unsigned) (b-a+1) * sizeof(fcomplex));
  if (!v) naaerror("allocation failure in cvector()");
  return (v-a);
}

/*****************************************************************************/
/* free_ccube() - Frees a complex cube allocated with ccube().               */
/*****************************************************************************/
void free_ccube(m,a,b,c,d,e,f)
fcomplex ***m;
int a,b,c,d,e,f;		/* (variable f is never used.) */
{
  int i, j;

  for(i=b;i>=a;i--)
    for(j=d;j>=c;j--)
      free((char *) (m[i][j]+e));
  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}

/*****************************************************************************/
/* free_cmatrix() - Frees a complex matrix allocated with cmatrix().         */
/*****************************************************************************/
void free_cmatrix(m,a,b,c,d)
fcomplex **m;
int a,b,c,d;			/* (variable d is never used.) */
{
  int i;

  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}

/*****************************************************************************/
/* free_cvector() - Frees a complex vector allocated by cvector().           */
/*****************************************************************************/
void free_cvector(v,a,b)
fcomplex *v;
int a,b;			/* (variable b is never used.) */
{
  free((char *) (v+a));
}

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
