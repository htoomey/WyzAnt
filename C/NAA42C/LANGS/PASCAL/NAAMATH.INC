(******************************* NAAMATH.INC **********************************
	"Numerical Analysis Algorithms in Pascal" Math Procedures v4.0
******************************************************************************)

(*
This include file contains many functions that are not part of the PASCAL
language but are implemented in other languages.  Only the functions that
involve floating-point numbers (i.e. - REAL, float, double, DOUBLE PRECISION)
are implemented.  These functions may be cut-and-pasted from this file and
placed into your programs as need, or you can include them all inside every
program by leaving the line "{$I NAAMATH.INC}" inside the programs as they now
stand.

These functions are not in perfect alphabetical order, since some of the
functions call upon others (such as POW()), making their implementation easier
and more readable.

WARNING:  If using Turbo Pascal, do not compile with TURBOBCD.COM.  (Sqrt, Sin,
and similar operations are nor implemented in the BCD version.)  Use TURBO.COM
to compile.

	VARIABLES USED:			COMPILERS COMPARED:
	I, R, X, Y : REAL;		Microsoft C 5.0
	N : INTEGER;			Microsoft FORTRAN77 v3.3
	C : COMPLEX_TYPE;		Borland Turbo PASCAL 3.02A
					??? ADA


FUNCTIONS	ANSI	MSC	FORTRAN			Pascal Conflict with
		C	C	77	ADA	PASCAL	another language
-------------------------------------------------------------------------------
ABS(X)						X
ACOS(X)		X	X	X
AINT(X)				X
ANINT(X)			X
ARCCOS(X)					
ARCSIN(X)					
ARCTAN(X)					X
ASIN(X)		X	X	X
ATAN(X)		X	X	X		(X)
ATAN2(X,Y)	X	X	X
CABS(C)			X	X
CEIL(X)		X	X
COS(X)		X	X	X		X
COSH(X)		X	X	X
DIM(X)				X
EXP(X)		X	X	X		X
FABS(X)		X	X
FLOOR(X)	X	X
FMOD(X,Y)	X	X
FREXP(X,N)	X	X				
HYPOT(X,Y)		X
J0(X)			X
J1(X)			X
JN(N,X)			X
LDEXP(X,N)	X	X
LN(X)						X
LOG(X)		X	X	X		(X)	C, FORTRAN
LOG10(X)	X	X	X
MOD(X)				X		X	FORTRAN Function
MODF(X,*N)	X	X				NOT IMPLIMENTED
NINT(X)				X
POW(X,Y)	X	X
PWROFTEN(N)					X
ROUND(X)					X
SIN(X)		X	X	X		X
SINH(X)		X	X	X
SQR(X)						X
SQRT(X)		X	X	X		X
TAN(X)		X	X	X
TANH(X)		X	X	X
TRUNC(X)					X
Y0(X)			X
Y1(X)			X
YN(N,X)			X
*)


(******************************************************************************
  TYPE DEFINITION:  Complex_type.  For complex numbers.
******************************************************************************)
TYPE COMPLEX_TYPE = RECORD
  R, I : REAL
END;


(******************************************************************************
  FUNCTION ATAN2(y, x):  Arctangent [-PI, PI].  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION ATAN2(y, x : REAL) : REAL;
CONST
  PI = 3.1415926535897932384626433832795;
BEGIN
  IF x = 0.0 THEN		(* For the asymptotes at PI/2 and -PI/2. *)
    BEGIN
      IF y >= 0.0 THEN
        ATAN2 := PI / 2.0
      ELSE
        ATAN2 := -PI / 2.0
    END
  ELSE IF x > 0.0 THEN		(* For quadrants I and IV. *)
    ATAN2 := ARCTAN(Y/X)
  ELSE IF y >= 0.0 THEN		(* For quadrant II. *)
    ATAN2 := ARCTAN(Y/X) + PI
  ELSE				(* For quadrant III. *)
    ATAN2 := ARCTAN(Y/X) - PI
END;


(******************************************************************************
  FUNCTION ACOS(x):  Arccosine [0, PI].  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION ACOS(x : REAL) : REAL;
VAR
  theta, y : REAL;
BEGIN
  IF ((x < -1.0) OR (x > 1.0)) THEN	(* x must be between -1 and 1. *)
    naaerror('RANGE ERROR - x must be between -1 and 1 with ACOS().');
  IF x = 0.0 THEN		(* Special case: For theta = 90 degrees. *)
    ACOS := 3.1415926535897932384626433832795 / 2.0;
  y := SQRT(1.0 - SQR(x));
  theta := ARCTAN(y / x);
  IF theta > 0.0 THEN		(* For quadrant I. *)
    ACOS := theta
  ELSE				(* For quadrant II (from quadrant IV). *)
    ACOS := theta + 3.1415926535897932384626433832795
END;


(******************************************************************************
  FUNCTION AINT(x):  Truncation.  Used in FORTRAN 77.
******************************************************************************)
FUNCTION AINT(x : REAL) : REAL;
BEGIN
  AINT := TRUNC(x)
END;


(******************************************************************************
  FUNCTION ANINT(x):  Nearest whole number.  Used in FORTRAN 77.
******************************************************************************)
FUNCTION ANINT(x : REAL) : REAL;
BEGIN
  ANINT := ROUND(x)
END;


(******************************************************************************
  FUNCTION ARCCOS(x):  Arccosine [0, PI].
******************************************************************************)
FUNCTION ARCCOS(x : REAL) : REAL;
BEGIN
  ARCCOS := ACOS(x)
END;


(******************************************************************************
  FUNCTION ASIN(x):  Arcsine [-PI/2, PI/2].  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION ASIN(x : REAL) : REAL;
VAR
  y : REAL;			(* Think of x as y and y as x. *)
BEGIN
  IF ((x < -1.0) OR (x > 1.0)) THEN	(* x must be between -1 and 1. *)
    naaerror('RANGE ERROR - x must be between -1 and 1 with ASIN().');
  y := SQRT(1.0 - SQR(x));
  ASIN := ARCTAN(x / y)		(* For quadrants I and IV. *)
END;


(******************************************************************************
  FUNCTION ARCSIN(x):  Arcsine [-PI/2, PI/2].
******************************************************************************)
FUNCTION ARCSIN(x : REAL) : REAL;
BEGIN
  ARCSIN := ASIN(x)
END;


(******************************************************************************
  FUNCTION ATAN(x):  Arctangent [-PI/2, PI/2].  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION ATAN(x : REAL) : REAL;
BEGIN
  ATAN := ARCTAN(x)
END;


(******************************************************************************
  FUNCTION CABS(r,i):  Complex absolute value.  Used in MSC and FORTRAN 77.
******************************************************************************)
FUNCTION CABS(c : COMPLEX_TYPE) : REAL;
BEGIN
  CABS := SQRT(SQR(c.r) + SQR(c.i))
END;


(******************************************************************************
  FUNCTION CEIL(x):  Smallest integer not less than x.  Used in C.
******************************************************************************)
FUNCTION CEIL(x : REAL) : REAL;
VAR
  tmp : REAL;
BEGIN
  tmp := x - TRUNC(x);
  IF tmp = 0.0 THEN		(* x is already an integer. *)
    CEIL := x
  ELSE IF x > 0.0 THEN		(* For positive x. *)
    CEIL := TRUNC(x) + 1.0
  ELSE				(* For negative x. *)
    CEIL := TRUNC(x)
END;


(******************************************************************************
  FUNCTION COSH(x):  Hyperbolic Cosine.  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION COSH(x : REAL) : REAL;
BEGIN
  COSH := (EXP(x) + EXP(-x)) / 2.0
END;


(******************************************************************************
  FUNCTION DIM(x):  Positive difference.  Used in FORTRAN 77.
******************************************************************************)
FUNCTION DIM(x, y : REAL) : REAL;
VAR
  tmp : REAL;
BEGIN
  tmp := x - y;
  IF tmp >= 0.0 THEN
    DIM := tmp
  ELSE
    DIM := 0.0
END;


(******************************************************************************
  FUNCTION FABS(x):  Absolute value of a float.  Used in C.
******************************************************************************)
FUNCTION FABS(x : REAL) : REAL;
BEGIN
  FABS := ABS(x)
END;


(******************************************************************************
  FUNCTION FLOOR(x):  Largest integer not greater than x.  Used in C.
******************************************************************************)
FUNCTION FLOOR(x : REAL) : REAL;
VAR
  tmp : REAL;
BEGIN
  tmp := x - TRUNC(x);
  IF tmp = 0.0 THEN		(* x is already an integer. *)
    FLOOR := x
  ELSE IF x > 0.0 THEN		(* For positive x. *)
    FLOOR := TRUNC(x)
  ELSE				(* For negative x. *)
    FLOOR := TRUNC(x) - 1.0
END;


(******************************************************************************
  FUNCTION FMOD(x, y):  Floating-point remainder w/sign of x.  Used in C.
******************************************************************************)
FUNCTION FMOD(x, y : REAL) : REAL;
VAR
  remainder : REAL;
BEGIN
  IF y = 0.0 THEN		(* An ambiguous case. *)
    FMOD := 0.0
  ELSE
    BEGIN
      remainder := x / y;
      remainder := remainder - TRUNC(remainder);
      IF x >= 0.0 THEN		(* For positive x. *)
        FMOD := ABS(remainder)
      ELSE			(* For negative x. *)
        FMOD := -ABS(remainder)
    END
END;


(******************************************************************************
  FUNCTION HYPOT(x,y):  Hypotenuse or SQRT(SQR(x) + SQR(y)).  Used in C.
******************************************************************************)
FUNCTION HYPOT(x, y : REAL) : REAL;
BEGIN
  HYPOT := SQRT(SQR(x) + SQR(y))
END;


(******************************************************************************
  FUNCTION J0(x):  Bessel function of first kind (order 0).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)


(******************************************************************************
  FUNCTION J1(x):  Bessel function of first kind (order 1).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)


(******************************************************************************
  FUNCTION JN(x):  Bessel function of first kind (order n).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)


(******************************************************************************
  FUNCTION LDEXP(x, n):  Computes x * 2^n.  Used in C.
******************************************************************************)
FUNCTION LDEXP(x : REAL; n : INTEGER) : REAL;
VAR
  tmp, i : INTEGER;
BEGIN
  IF n < 0 THEN			(* For Negative n. *)
    LDEXP := 0.0
  ELSE IF n = 0 THEN		(* For zero n. *)
    LDEXP := x
  ELSE				(* For Positive n. *)
    BEGIN
      tmp := 1;
      FOR i := 1 to n DO
        tmp := tmp * 2;
      LDEXP := x * tmp
    END
END;


(******************************************************************************
  FUNCTION LOG10(x):  Logorithm to base 10.  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION LOG10(x : REAL) : REAL;
BEGIN
  LOG10 := LN(x) / LN(10.0)
END;


(******************************************************************************
  FUNCTION MODF(x, y):  Like FMOD but saves quotient too.  Used in C.
******************************************************************************)
(* Not implemented.  Use FMOD(). *)


(******************************************************************************
  FUNCTION NINT(x):  Nearest integer.  Used in FORTRAN 77.
******************************************************************************)
FUNCTION NINT(x : REAL) : REAL;
BEGIN
  NINT := ROUND(x)
END;


(******************************************************************************
  FUNCTION POW(x,y):  Returns X^Y  (X raised to the Y power).
******************************************************************************)
FUNCTION POW(x, y : REAL) : REAL;
BEGIN
  IF y = 0.0 THEN		(* Anything raised to the 0 power equals 1. *)
    POW := 1.0
  ELSE IF x = 0.0 THEN
    BEGIN
      IF (y > 0.0) THEN		(* 0 raised to any positive power equals 0. *)
        POW := 0.0
      ELSE			(* Domain error since x=0 and y <= 0. *)
        naaerror('DOMAIN ERROR - Can not solve 0^(-y) in POW().')
    END
  ELSE IF x > 0.0 THEN		(* For positive x. *)
    POW := EXP(y * LN(x))
  ELSE				(* For negative x. *)
    BEGIN
      IF (y - TRUNC(y)) <> 0.0 THEN	(* ERROR - y is not an integer. *)
        BEGIN
          naaerror('ERROR - Can`t solve x^(-y) in POW().  y isn`t an integer.');
          halt
        END
      ELSE IF ODD(TRUNC(y)) THEN
        POW := -EXP(y * LN(-x))	(* For y odd integers. *)
      ELSE
        POW := EXP(y * LN(-x))	(* For y even integers. *)
    END
END;

(******************************************************************************
  FUNCTION FREXP(x, n):  Creates m*2^n where m = [1/2..1].  Used in C.
******************************************************************************)
(* n is never returned, but it is calculated. *)
FUNCTION FREXP(x : REAL; n : INTEGER) : REAL;
BEGIN
  IF x = 0.0 THEN
    BEGIN
      n := 0;
      FREXP := 0.0
    END
  ELSE IF x < 0.0 THEN
    n := TRUNC(LN(-x) / LN(2.0))
  ELSE
    n := TRUNC(LN(x) / LN(2.0));

  IF x > 0.0 THEN
    BEGIN
      IF POW(2.0, (1.0 * n)) = x THEN
        FREXP := 1.0
      ELSE
        BEGIN
          n := n + 1;
          FREXP := x / POW(2.0, (1.0 * n))
        END
    END
  ELSE
    BEGIN
      IF (-POW(2.0, (1.0 * n)) = x) THEN
        FREXP := -1.0
      ELSE
        BEGIN
          n := n + 1;
          FREXP := x / POW(2.0, (1.0 * n))
        END
    END
END;


(******************************************************************************
  FUNCTION SINH(x):  Hyperbolic Sine.  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION SINH(x : REAL) : REAL;
BEGIN
  SINH := (EXP(x) - EXP(-x)) / 2.0
END;


(******************************************************************************
  FUNCTION TAN(x):  Tangent.  Used in C, ADA, and FORTRAN 77.
******************************************************************************)
FUNCTION TAN(x : REAL) : REAL;
BEGIN
  TAN := SIN(x) / COS(x)
END;


(******************************************************************************
  FUNCTION TANH(x):  Hyperbolic Tangent.  Used in C and FORTRAN 77.
******************************************************************************)
FUNCTION TANH(x : REAL) : REAL;
VAR
  tmp1, tmp2 : REAL;
BEGIN
  tmp1 := EXP(x);
  tmp2 := EXP(-x);
  TANH := (tmp1 - tmp2) / (tmp1 + tmp2) 
END;


(******************************************************************************
  FUNCTION Y0(x):  Bessel function of second kind (order 0).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)
(* x must be positive. *)


(******************************************************************************
  FUNCTION Y1(x):  Bessel function of second kind (order 1).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)
(* x must be positive. *)


(******************************************************************************
  FUNCTION YN(x):  Bessel function of second kind (order n).  Used in MSC.
******************************************************************************)
(* Not implemented yet. *)
(* x must be positive. *)


(******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0      *
*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.	      *
******************************************************************************)
