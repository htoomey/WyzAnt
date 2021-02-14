(****************************** NAAUTIL.INC ***********************************
	"Numerical Analysis Algorithms in Pascal" Utilities I v4.0
******************************************************************************)
(*
This include file contains many useful procedures used by most all of the
Numerical Analysis Algorithms programs.  It should be included in all of the
programs using the line {$I NAAUTIL.INC}.  It contains the following
procedures and functions:

	PROCEDURE naaerror               (str     : STRING_TYPE);
	FUNCTION  get_title                       : STRING_TYPE;
	PROCEDURE print_title            (str     : STRING_TYPE);
	PROCEDURE print_title_to_outfile (str     : STRING_TYPE);

*)

(*****************************************************************************
 * naaerror() - Numerical Analysis Algorithms standard error handler.        *
 *****************************************************************************)
PROCEDURE naaerror(str : STRING_TYPE);
BEGIN
  WRITELN;
  WRITELN('Numerical Analysis Algorithms run-time error...');
  WRITELN(str);
  WRITELN('...now exiting to system...');
  HALT
END;

(*****************************************************************************
 * get_title() - Prints the NAA banner and prompts for an optional title.    *
 *****************************************************************************)
FUNCTION get_title : STRING_TYPE;
VAR
  str : STRING_TYPE;
BEGIN
  FOR i := 1 TO 79 DO
    WRITE('-');
  WRITELN;
  WRITE('                 ');
  WRITELN('"Numerical Analysis Algorithms in Pascal" v4.0');
  FOR i := 1 TO 79 DO
    WRITE('-');
  WRITELN;
  WRITELN;
  WRITELN('Enter a title [ie - Set 2.1,  Problem 2 a) ].');
  WRITE('----> ');
  READLN(str);
  WRITELN;
  get_title := str
END;

(*****************************************************************************
 * print_title() - Prints the optional title to the screen.                  *
 *****************************************************************************)
PROCEDURE print_title(str : STRING_TYPE);
VAR
  i : INTEGER;
BEGIN
  FOR i := 1 TO 79 DO
    WRITE('-');
  WRITELN;
  WRITE('                 ');
  WRITELN('"Numerical Analysis Algorithms in Pascal" v4.0');
  FOR i := 1 TO 79 DO
    WRITE('-');
  WRITELN;
  WRITELN;

  IF LENGTH(str) > 1 THEN
    WRITELN(str);
  WRITELN
END;

(*****************************************************************************
 * print_title_to_outfile() - Prints the optional title to a file.           *
 *****************************************************************************)
PROCEDURE print_title_to_outfile(str : STRING_TYPE);
VAR
  i : INTEGER;
BEGIN
  FOR i := 1 TO 79 DO
    WRITE (file_id, '-');
  WRITELN (file_id);
  WRITE   (file_id, '                 ');
  WRITELN (file_id, '"Numerical Analysis Algorithms in Pascal" v4.0');
  FOR i := 1 TO 79 DO
    WRITE (file_id, '-');
  WRITELN (file_id);
  WRITELN (file_id);

  IF LENGTH(str) > 1 THEN
    WRITELN (file_id, str);
END;

(******************************************************************************
*	Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0      *
*	Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.	      *
******************************************************************************)
