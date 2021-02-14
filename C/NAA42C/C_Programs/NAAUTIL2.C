/****************************** NAAUTIL2.C ************************************
	     "Numerical Analysis Algorithms in C" Utilities II v4.2
******************************************************************************/

/*
**  This file is very similar to "naautil.c".  It contains less frequently
**  used dynamic memory allocation routines to create very flexible vectors,
**  matrices, and cubes.  For complex vectors, matrices, and cubes, use
**  "naautil3.c".  The file "naautil.h" should be included BEFORE this file
**  to define the ANSI flag and the NO_LONG_DOUBLES flag as TRUE or FALSE.
** 
**  It contains the following procedures and functions:
** 
**    Return         Procedure
**    Type           Name            Description
**    ---------------------------------------------------------------------
**    long double*** ldcube        -  Allocates a 3-D array of long doubles
**    double***      dcube         -  Allocates a 3-D array of doubles
**    float***       cube          -  Allocates a 3-D array of floats
**    int***         icube         -  Allocates a 3-D array of integers
**    long double**  ldmatrix      -  Allocates a 2-D array of long doubles
**    int**          imatrix       -  Allocates a 2-D array of integers
**    long double*   ldvector      -  Allocates a 1-D array of long doubles
**
**    void           free_ldcube   -  Frees the allocated 3-D array memory
**    void           free_dcube    -  Frees the allocated 3-D array memory
**    void           free_cube     -  Frees the allocated 3-D array memory
**    void           free_icube    -  Frees the allocated 3-D array memory
**    void           free_ldmatrix -  Frees the allocated 2-D array memory
**    void           free_imatrix  -  Frees the allocated 2-D array memory
**    void           free_ldvector -  Frees the allocated 1-D array memory
** 
**  Some of these functions can be found in the text "Numerical Recipes in C".
**  They have been modified and improved for this application.
*/


/*********************/
/* DEFINES AND FLAGS */
/*********************/

#ifndef NO_LONG_DOUBLES
#define NO_LONG_DOUBLES	TRUE	/* Set to TRUE if "long double"s are not   */
#endif				/* supported on your compiler.  (ie - VAX) */


/*****************
 * INCLUDE FILES *
 *****************/

/*
** NOTE:  Requires that "naautil.h" be included, but only once!
*/

/************************
 * FUNCTION PROTOTYPING *
 ************************/

#if ANSI == TRUE		/* Set this flag once in "naautil.h"         */
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989)                      */
  double***      dcube         (int a, int b, int c, int d, int e, int f);
  float***       cube          (int a, int b, int c, int d, int e, int f);
  int***         icube         (int a, int b, int c, int d, int e, int f);
  int**          imatrix       (int a, int b, int c, int d);
  void           free_dcube    (double ***m, int a, int b, int c, int d,
                                int e, int f);
  void           free_cube     (float ***m, int a, int b, int c, int d,
                                int e, int f);
  void           free_icube    (int ***m, int a, int b, int c, int d,
                                int e, int f);
  void           free_imatrix  (int **m, int a, int b, int c, int d);
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)                         */
  /* Placed here for compatibility with older C compilers.                   */
  double***      dcube();
  float***       cube();
  int***         icube();
  int**          imatrix();
  void           free_dcube();
  void           free_cube();
  void           free_icube();
  void           free_imatrix();
#endif


#if NO_LONG_DOUBLES == FALSE

#if ANSI == TRUE		/* Set this flag once in "naautil.h"         */
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989)                      */
  long double*** ldcube        (int a, int b, int c, int d, int e, int f);
  long double**  ldmatrix      (int a, int b, int c, int d);
  long double*   ldvector      (int a, int b);
  void           free_ldcube   (long double ***m, int a, int b, int c, int d,
                                int e, int f);
  void           free_ldmatrix (long double **m, int a, int b, int c, int d);
  void           free_ldvector (long double *v, int a, int b);
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)                         */
  /* Placed here for compatibility with older C compilers.                   */
  long double*** ldcube();
  long double**  ldmatrix();
  long double*   ldvector();
  void           free_ldcube();
  void           free_ldmatrix();
  void           free_ldvector();
#endif

#endif  /* NO_LONG_DOUBLES */


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* dcube() - Allocates a double cube with range [a..b][c..d][e..f].          */
/*           Like dmatrix() but for three dimensional arrays.                */
/*****************************************************************************/
double ***dcube(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
  int i,j;
  double ***m;

  m=(double ***) calloc((unsigned) (b-a+1), sizeof(double **));
  if (!m) naaerror("allocation failure 1 in dcube()");
  m -= a;

  for (i=a;i<=b;i++) {
    m[i]=(double **) calloc((unsigned) (d-c+1), sizeof(double *));
    if (!m[i]) naaerror("allocation failure 2 in dcube()");
    m[i] -= c;
  }

  for (i=a;i<=b;i++)
    for (j=c;j<=d;j++) {
      m[i][j]=(double *) calloc((unsigned) (f-e+1), sizeof(double));
      if (!m[i][j]) naaerror("allocation failure 3 in dcube()");
      m[i][j] -= e;
    }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* cube() - Allocates a float cube with range [a..b][c..d][e..f].            */
/*          Like matrix() but for three dimensional arrays.                  */
/*****************************************************************************/
float ***cube(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
  int i,j;
  float ***m;

  m=(float ***) calloc((unsigned) (b-a+1), sizeof(float **));
  if (!m) naaerror("allocation failure 1 in cube()");
  m -= a;

  for (i=a;i<=b;i++) {
    m[i]=(float **) calloc((unsigned) (d-c+1), sizeof(float *));
    if (!m[i]) naaerror("allocation failure 2 in cube()");
    m[i] -= c;
  }

  for (i=a;i<=b;i++)
    for (j=c;j<=d;j++) {
      m[i][j]=(float *) calloc((unsigned) (f-e+1), sizeof(float));
      if (!m[i][j]) naaerror("allocation failure 3 in cube()");
      m[i][j] -= e;
    }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* icube() - Allocates an integer cube with range [a..b][c..d][e..f].        */
/*           Like dmatrix() but for three dimensional arrays.                */
/*****************************************************************************/
int ***icube(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
  int i,j;
  int ***m;

  m=(int ***) calloc((unsigned) (b-a+1), sizeof(int **));
  if (!m) naaerror("allocation failure 1 in icube()");
  m -= a;

  for (i=a;i<=b;i++) {
    m[i]=(int **) calloc((unsigned) (d-c+1), sizeof(int *));
    if (!m[i]) naaerror("allocation failure 2 in icube()");
    m[i] -= c;
  }

  for (i=a;i<=b;i++)
    for (j=c;j<=d;j++) {
      m[i][j]=(int *) calloc((unsigned) (f-e+1), sizeof(int));
      if (!m[i][j]) naaerror("allocation failure 3 in icube()");
      m[i][j] -= e;
    }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* imatrix() - Allocates an integer matrix with range [a..b][c..d].          */
/*****************************************************************************/
int **imatrix(a,b,c,d)
int a,b,c,d;
{
  int i;
  int **m;

  /* allocate pointers to rows */
  m=(int **) calloc((unsigned) (b-a+1), sizeof(int*));
  if (!m) naaerror("allocation failure 1 in imatrix()");
  m -= a;

  for (i=a;i<=b;i++) {	/* allocate rows and set pointers to them.           */
    m[i]=(int *) calloc((unsigned) (d-c+1), sizeof(int));
    if (!m[i]) naaerror("allocation failure 2 in imatrix()");
    m[i] -= c;
  }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* free_dcube() - Frees a double cube allocated with dcube().                */
/*****************************************************************************/
void free_dcube(m,a,b,c,d,e,f)
double ***m;
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
/* free_cube() - Frees a float cube allocated with dcube().                  */
/*****************************************************************************/
void free_cube(m,a,b,c,d,e,f)
float ***m;
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
/* free_icube() - Frees a integer cube allocated with icube().               */
/*****************************************************************************/
void free_icube(m,a,b,c,d,e,f)
int ***m;
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
/* free_imatrix() - Frees an integer matrix allocated with imatrix().        */
/*****************************************************************************/
void free_imatrix(m,a,b,c,d)
int **m;
int a,b,c,d;			/* (variable d is never used.) */
{
  int i;

  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}


#if NO_LONG_DOUBLES == FALSE

/*****************************************************************************/
/* ldcube() - Allocates a long double cube with range [a..b][c..d][e..f].    */
/*            Like ldmatrix() but for three dimensional arrays.              */
/*****************************************************************************/
long double ***ldcube(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
  int i,j;
  long double ***m;

  m=(long double ***) calloc((unsigned) (b-a+1), sizeof(long double **));
  if (!m) naaerror("allocation failure 1 in ldcube()");
  m -= a;

  for (i=a;i<=b;i++) {
    m[i]=(long double **) calloc((unsigned) (d-c+1), sizeof(long double *));
    if (!m[i]) naaerror("allocation failure 2 in ldcube()");
    m[i] -= c;
  }

  for (i=a;i<=b;i++)
    for (j=c;j<=d;j++) {
      m[i][j]=(long double *) calloc((unsigned) (f-e+1), sizeof(long double));
      if (!m[i][j]) naaerror("allocation failure 3 in ldcube()");
      m[i][j] -= e;
    }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* ldmatrix() - Allocates a long double matrix with range [a..b][c..d].      */
/*****************************************************************************/
long double **ldmatrix(a,b,c,d)
int a,b,c,d;
{
  int i;
  long double **m;

  /* allocate pointers to rows */
  m=(long double **) calloc((unsigned) (b-a+1), sizeof(long double*));
  if (!m) naaerror("allocation failure 1 in ldmatrix()");
  m -= a;

  for (i=a;i<=b;i++) {	/* allocate rows and set pointers to them */
    m[i]=(long double *) calloc((unsigned) (d-c+1), sizeof(long double));
    if (!m[i]) naaerror("allocation failure 2 in ldmatrix()");
    m[i] -= c;
  }

  return (m);		/* return pointer to array of pointers to rows.      */
}

/*****************************************************************************/
/* ldvector() - Allocates a long double vector with range [a..b].            */
/*****************************************************************************/
long double *ldvector(a,b)
int a,b;
{
  long double *v;

  v=(long double *)calloc((unsigned) (b-a+1), sizeof(long double));
  if (!v) naaerror("allocation failure in ldvector()");
  return (v-a);
}

/*****************************************************************************/
/* free_ldcube() - Frees a long double cube allocated with ldcube().         */
/*****************************************************************************/
void free_ldcube(m,a,b,c,d,e,f)
long double ***m;
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
/* free_ldmatrix() - Frees a long double matrix allocated with ldmatrix().   */
/*****************************************************************************/
void free_ldmatrix(m,a,b,c,d)
long double **m;
int a,b,c,d;			/* (variable d is never used.) */
{
  int i;

  for(i=b;i>=a;i--)
    free((char *) (m[i]+c));
  free((char *) (m+a));
}

/*****************************************************************************/
/* free_ldvector() - Frees a long double vector allocated by ldvector().     */
/*****************************************************************************/
void free_ldvector(v,a,b)
long double *v;
int a,b;			/* (variable b is never used.) */
{
  free((char *) (v+a));
}

#endif  /* NO_LONG_DOUBLES */

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
