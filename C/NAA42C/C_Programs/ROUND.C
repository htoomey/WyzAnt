/********************************** ROUND.C ***********************************
		Round() - Rounds X to SIG significant digits
******************************************************************************/

/*
**  This include file contains the routine round().  This routine will round
**  a floating point number to 1 to 8 significant digits.  It is useful in
**  showing the effects of computer round-off error.  It is identical to
**  trunc() except for two lines which are added to perform the rounding.
**
**  Requires "#include <math.h>" which is usually included when using
**  "naautil.h".
**
**  The real number of significant digits available is machine dependant.
**  To find the number of significant digits, see #include <float.h>.
**  Some common numbers are:
**
**                 Defined    IBM PC   Macintosh         MIPS Computer Systems
**    Type         Name       80X86    680X0      VAX    R2010A/R3010/R4010
**    ----------------------------------------------------------------------
**    float        FLT_DIG       6        6        6         6
**    double       DBL_DIG      15       18       15        15
**    long double  LDBL_DIG     15       18       15        15
*/

/*****************/
/* INCLUDE FILES */
/*****************/

#include <float.h>		/* For DBL_DIG definition in the algorithms. */


/***********************/
/* FUNCTION PROTOTYPES */
/***********************/

#if ANSI == TRUE
  double round(double X, int SIG);
#else
  double round();
#endif


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* round() - Rounds X to SIG significant digits.  Range checking for SIG is  */
/*           performed inside the algorithms.                                */
/*****************************************************************************/
double round(X, SIG)
double X;
int SIG;			/* Number of significant digits      */
{
  double y, rem;
  int i;

  if (X == 0.0)
    return (0.0);		/* Special case.                     */

  y = X;			/* Make a copy of X.                 */

  for (i=0; fabs(y) > 1.0; i--)	/* Shift right to the decimal point. */
    y /= 10.0;			/* y = y / 10. */
				/* Shift left SIG places.            */
  while (fabs(y) <= fabs(pow(10.0, (double)(SIG - 1)))) {
    y *= 10.0;			/* y = y * 10. */
    i++;			/* i = i + 1.  */
  }

  y = modf(y, &rem);		/* Returns fractional portion of y.  */
  if (y >=  0.5)  y -= 1.0;	/* Rounds up if y >= 0.5.            */
  if (y <= -0.5)  y += 1.0;	/* Rounds up if y <= -0.5.           */
  rem = y / pow(10.0, (double) i);

  return(X - rem);
}

/******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2      *
*	Copyright (C) 1988-1991, Harold A. Toomey, All Rights Reserved.       *
******************************************************************************/
