{
FILE NAME: SIMPSON.PAS
LANGUAGE:  PASCAL
COMPILER:  Turbo PASCAL Version 3.02A
STATUS:    Compiles and runs correctly
           Note: NAAMATH.INC has not been fully tested yet
}

{******************************************************************************
		Composite Simpson's Rule - Algorithm 4.1
*******************************************************************************

                                ôb
To approximate the integral I = ³  f(x) dx:
                                õa

INPUT endpoints a, b; positive integer n; the function f().

OUTPUT approximation XI to I.

*******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0      *
******************************************************************************}

PROGRAM simpson;

{ USES TRANSCEND }		{ Un-comment if using UCSD Pascal. }

  TYPE
    STRING_TYPE = STRING[80];

  CONST
    PI = 3.1415926535897932384626433832795028841972;
    outfile = 'simpson.out';	{ Default name of the output file. }
    eq_text_f = 'f(x) = sin(x)';{ Needs Updating $                 }

  VAR
    a, b, h, X, XI, XI0, XI1, XI2 : REAL;
    i, n    : INTEGER;
    title   : STRING_TYPE;
    file_id : TEXT;

  {****************************
   * PROCEDURES AND FUNCTIONS *
   ****************************}

  {$I NAAUTIL.INC}		{ Numerical Analysis Algorithms Utilities. }
  {$I NAAMATH.INC}		{ Complete set of math functions.          }


{*****************************************************************************
 * f(x) - Function to evaluate, f(x).  Needs updating $.		     *
 *****************************************************************************}
  FUNCTION f(x : REAL) : REAL;
  BEGIN
    f := SIN(x)
  END;


{*****************************************************************************
 *               THIS IS WHERE THE PROGRAM STARTS EXECUTING                  *
 *****************************************************************************}

BEGIN

  {**********
   * INPUTS *
   **********}

  title := get_title;		{ Prompts for optional comments. }

  WRITELN ('Composite Simpson`s Rule - Algorithm 4.1');
  WRITELN;
  WRITELN (eq_text_f);
  WRITELN;
  WRITE ('Enter endpoint a: ');
  READLN (a);
  WRITE ('Enter endpoint b: ');
  READLN (b);

  REPEAT
    WRITE ('Enter number of intervals on [a,b], n: ');
    READLN (n);
    IF n <= 0 THEN
      WRITELN ('ERROR - n must be greater than zero.');
  UNTIL (n > 0);

  {*************
   * ALGORITHM *
   *************}

  { STEP #1 }
  h := (b - a)/n;

  { STEP #2 }
  XI0 := f(a) + f(b);
  XI1 := 0.0;			{ Summation of f(x(2i-1)). }
  XI2 := 0.0;			{ Summation of f(x(2i)).   }

  { STEP #3 }
  FOR i := 1 TO n - 1 DO
    BEGIN

      { STEP #4 }
      X := a + i*h;

      { STEP #5 }
      IF NOT ODD(i) THEN
        XI2 := XI2 + f(X)	{ For even i. }
      ELSE
        XI1 := XI1 + f(X)	{ For odd i.  }
    END;

  { STEP #6 }
  XI := h*(XI0 + 2.0*XI2 + 4.0*XI1) / 3.0;

  {***********
   * OUTPUTS *
   ***********}

  { STEP #7 }
  { Write ouput to the screen. }
  print_title (title);

  WRITELN ('Composite Simpson`s Rule - Algorithm 4.1');
  WRITELN;
  WRITELN (eq_text_f, ' from ', a :8:8, ' to ', b :8:8, '.');
  WRITELN;
  WRITELN ('n = ', n, ' intervals on [a,b].');
  WRITELN ('Interval number h = ', h :8:8);
  WRITELN;
  WRITELN ('    ô', b :8:8);
  WRITELN ('I = ³  f(x) dx = ', XI :8:8);
  WRITELN ('    õ', a :8:8);
  WRITELN;
  WRITELN ('Required ', n + 1, ' functional evaluations.');

  { Write output to a file. }
  ASSIGN (file_id, outfile);	{ Opens outfile for output. }
  REWRITE (file_id);

  print_title_to_outfile (title);

  WRITELN (file_id);
  WRITELN (file_id, 'Composite Simpson`s Rule - Algorithm 4.1');
  WRITELN (file_id);
  WRITELN (file_id, eq_text_f, ' from ', a :8:8, ' to ', b :8:8, '.');
  WRITELN (file_id);
  WRITELN (file_id, 'n = ', n, ' intervals on [a,b].');
  WRITELN (file_id, 'Interval number h = ', h :8:8);
  WRITELN (file_id);
  WRITELN (file_id, '    ô', b :8:8);
  WRITELN (file_id, 'I = ³  f(x) dx = ', XI :8:8);
  WRITELN (file_id, '    õ', a :8:8);
  WRITELN (file_id);
  WRITELN (file_id, 'Required ', n + 1, ' functional evaluations.');
  CLOSE (file_id);		{ Closes output file. }

  WRITELN;
  WRITELN ('Output saved into file "', outfile, '".');

END.  				{ STOP }

{******************************************************************************
*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.	      *
******************************************************************************}
