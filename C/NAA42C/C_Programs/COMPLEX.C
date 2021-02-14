/******************************** COMPLEX.C ***********************************
	      "Numerical Recipes in C" Complex Arithmetic Routines
******************************************************************************/

/*
**  Requires #include "naautil.h" which contains the #define for ANSI as well
**  as "#include <math.h>" for necessary mathematical prototypes.  This header
**  file is currently included in "naautil3.c" only.
** 
**  Note: This header file will cause a program to CRASH if the program is
**        compiled with the Microsoft C 5.0 using the "/qc" compiler switch
**        for invoking the QuickC compiler.
** 
** 
**  This include file contains the following complex functions:
** 
**    Return   Procedure
**    Value    Name       Description
**    -----------------------------------------------------------------
**    fcomplex Cadd    -  Complex addition
**    fcomplex Csub    -  Complex subtraction
**    fcomplex Cmul    -  Complex multiplication
**    fcomplex Cdiv    -  Complex division
**    fcomplex Complex -  Converts two floats into a complex number
**    float    Cabs    -  Complex absolute value (magnitude)
**    fcomplex Conjg   -  Complex conjugate
**    fcomplex Csqrt   -  Complex square root
**    fcomplex RCmul   -  Complex multiply with a scalar
** 
**  These functions can be found in the text "Numerical Recipes in C".
** 
**  A complex number is defined to be a structure containing two FLOAT values,
**  the real (.r) and imaginary (.i) parts.
*/

/********************/
/* TYPE DEFINITIONS */
/********************/

typedef struct FCOMPLEX {float r,i;} fcomplex;

/*
**  NOTE: Many compilers define the type "complex" in "math.h" as a structure
**        containing doubles instead of floats.  See Microsoft C 5.0's "math.h"
**        file for an example.
*/


/***********************/
/* FUNCTION PROTOTYPES */
/***********************/

#if ANSI == TRUE
  /* ANSI STANDARD PROTOTYPING (Post-December 14, 1989) */
  fcomplex Cadd    (fcomplex a, fcomplex b);
  fcomplex Csub    (fcomplex a, fcomplex b);
  fcomplex Cmul    (fcomplex a, fcomplex b);
  fcomplex Cdiv    (fcomplex a, fcomplex b);
  fcomplex Complex (double re, double im);
  /* NOTE: The doubles may need changing to float */
  float    Cabs    (fcomplex z);
  fcomplex Conjg   (fcomplex z);
  fcomplex Csqrt   (fcomplex z);
  fcomplex RCmul   (double x, fcomplex a);
  /* NOTE: The double above may need changing to float */
#else
  /* OLDER STYLE PROTOTYPING (Pre-December 14, 1989)       */
  /* Placed here for compatibility with older C compilers. */
  fcomplex Cadd();
  fcomplex Csub();
  fcomplex Cmul();
  fcomplex Cdiv();
  fcomplex Complex();
  float    Cabs();
  fcomplex Conjg();
  fcomplex Csqrt();
  fcomplex RCmul();
#endif


/***************
 * SUBROUTINES *
 ***************/

/*****************************************************************************/
/* Cadd() - Returns the complex sum of two complex numbers.                  */
/*****************************************************************************/
fcomplex Cadd(a,b)
fcomplex a,b;
{
  fcomplex c;
  c.r = a.r + b.r;
  c.i = a.i + b.i;
  return (c);
}

/*****************************************************************************/
/* Csub() - Returns the complex difference of two complex numbers.           */
/*****************************************************************************/
fcomplex Csub(a,b)
fcomplex a,b;
{
  fcomplex c;
  c.r = a.r - b.r;
  c.i = a.i - b.i;
  return (c);
}

/*****************************************************************************/
/* Cmul() - Returns the complex product of two complex numbers.              */
/*****************************************************************************/
fcomplex Cmul(a,b)
fcomplex a,b;
{
  fcomplex c;
  c.r = a.r*b.r - a.i*b.i;
  c.i = a.i*b.r + a.r*b.i;
  return (c);
}

/*****************************************************************************/
/* Cdiv() - Returns the complex quotient of two complex numbers.             */
/*****************************************************************************/
fcomplex Cdiv(a,b)
fcomplex a,b;
{
  fcomplex c;
  float r, den;
  if (fabs(b.r) >= fabs(b.i)) {
    r = b.i/b.r;
    den = b.r + r*b.i;
    c.r = (a.r + r*a.i)/den;
    c.i = (a.i - r*a.r)/den;
  } else {
    r = b.r/b.i;
    den = b.i + r*b.r;
    c.r = (a.r*r + a.i)/den;
    c.i = (a.i*r - a.r)/den;
  }
  return (c);
}

/*****************************************************************************/
/* Complex() - Returns a complex number with specified real and imaginary    */
/*             parts.                                                        */
/*****************************************************************************/
fcomplex Complex(re,im)
double re,im;
{
  fcomplex c;
  c.r = re;
  c.i = im;
  return (c);
}

/*****************************************************************************/
/* Cabs() - Returns the absolute value (modulus) of a complex number.        */
/*          NOTE: You may wish to use Microsoft C 5.0's cabs() function.     */
/*****************************************************************************/
float Cabs(z)
fcomplex z;
{
  float x,y,ans,temp;
  x = fabs(z.r);
  y = fabs(z.i);
  if (x == 0.0)
    ans = y;
  else if (y == 0.0)
    ans = x;
  else if (x > y) {
    temp = y/x;
    ans = x*sqrt(1.0+temp*temp);
  } else {
    temp = x/y;
    ans = y*sqrt(1.0+temp*temp);
  }
  return (ans);
}

/*****************************************************************************/
/* Conjg() - Returns the complex conjugate of a complex number.              */
/*****************************************************************************/
fcomplex Conjg(z)
fcomplex z;
{
  fcomplex c;
  c.r = z.r;
  c.i = -z.i;
  return (c);
}

/*****************************************************************************/
/* Csqrt() - Returns the complex square root of two complex numbers.         */
/*****************************************************************************/
fcomplex Csqrt(z)
fcomplex z;
{
  fcomplex c;
  float x,y,w,r;
  if ((z.r == 0.0) && (z.i == 0.0)) {
    c.r = c.i = 0.0;
    return (c);
  } else {
    x = fabs(z.r);
    y = fabs(z.i);
    if (x >= y) {
      r = y/x;
      w = sqrt(x) * sqrt(0.5*(1.0+sqrt(1.0+r*r)));
    } else {
      r = x/y;
      w = sqrt(y) * sqrt(0.5*(r+sqrt(1.0+r*r)));
    }
    if (z.r >= 0.0) {
      c.r = w;
      c.i = z.i/(2.0*w);
    } else {
      c.i = (z.i >= 0.0) ? w : -w;
      c.r = z.i/(2.0*c.i);
    }
    return (c);
  }
}

/*****************************************************************************/
/* RCmul() - Returns the complex product of a real number and a complex      */
/*           number.                                                         */
/*****************************************************************************/
fcomplex RCmul(x,a)
double x;
fcomplex a;
{
  fcomplex c;
  c.r = x*a.r;
  c.i = x*a.i;
  return (c);
}

/******************************************************************************
*	Used by:  Harold A. Toomey, CARE-FREE SOFTWARE, 3Q 1991, v4.2         *
******************************************************************************/
