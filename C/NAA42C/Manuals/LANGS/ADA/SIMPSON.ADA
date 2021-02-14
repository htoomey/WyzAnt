-- FILE NAME: SIMPSON.ADA
-- LANGUAGE:  ADA
-- COMPILER:  Meridian Ada 4.1
-- STATUS:    Not tested yet

-- ****************************************************************************
-- 		Composite Simpson's Rule - Algorithm 4.1
-- ****************************************************************************
--
--                                 ôb
-- To approximate the integral I = ³  f(x) dx:
--                                 õa
--
-- INPUT endpoints a, b; positive integer n; the function f():
--
-- OUTPUT approximation XI to I.
--
-- ****************************************************************************
-- *	 Written by:  Harold A. Toomey, CARE-FREE SOFTWARE, 2Q 1991, v4.0     *
-- ****************************************************************************

with naa_util; use naa_util;	-- Numerical Analysis Algorithms utilities
with math_lib; use math_lib;	-- Contains several useful math functions
with text_io;  use text_io;	-- For file I/O
with ada_io;   use ada_io;	-- For easier text, integer and float I/O
				-- NOTE: ada_io is supplied with the Meridian
				-- Ada Compiler

procedure simpson is

  -- *************
  -- * constants *
  -- *************

  -- NOTE: PI is defined in the package math_lib to 40 decimal places.
  outfile   : constant := "simpson.out";    -- Default name of the output file
  eq_text_f : constant := "f(x) = sin(x)";  -- Needs updating  $

  -- *********
  -- * types *
  -- *********
  type file_type is limited private;

  -- *************
  -- * variables *
  -- *************

  a, b, h, x, xi, xi0, xi1, xi2 : float;
  n       : positive;
  title   : string(1..80);
  file_id : file_type;

  -- **********************************
  -- * LOCAL PROCEDURES AND FUNCTIONS *
  -- **********************************

  -- **************************************************************************
  -- * f(x) - Function to evaluate, f(x).  Needs updating $.		      *
  -- **************************************************************************
  function f(x : in float) return float is
  begin  -- f
    return (sin(x));
  end f;

  -- **************************************************************************


-- ********
-- * MAIN *
-- ********

begin

  -- **********
  -- * INPUTS *
  -- **********

  title := get_title;		-- Prompts for optional comments

  put_line ("Composite Simpson's Rule - Algorithm 4.1");
  new_line;
  put ("Enter endpoint a: ");
  get_line (a);
  put ("Enter endpoint b: ");
  get_line (b);

  loop
    put ("Enter number of intervals on [a,b], n: ");
    get_line (n);

--  Place an exception handler here for better use of ADA.
    if n <= 0 then		-- ERROR - n can not be negative.
      put_line ("ERROR - n must be greater than zero.");
    end if;

    exit when n > 0;
  end loop;

  -- *************
  -- * ALGORITHM *
  -- *************

  -- STEP #1
  h := (b - a)/n;

  -- STEP #2
  xi0 := f(a) + f(b);
  xi1 := 0.0;			-- Summation of f(x(2i-1))
  xi2 := 0.0;			-- Summation of f(x(2i))

  -- STEP #3
  for i in 1..n-1 loop

      -- STEP #4
      x := a + i*h;

      -- STEP #5
      if i mod 2 = 0 then
        xi2 := xi2 + f(x);	-- For even i
      else
        xi1 := xi1 + f(x);	-- For odd i
      end if;

  end loop;

  -- STEP #6
  xi := h*(xi0 + 2.0*xi2 + 4.0*xi1) / 3.0;

  -- ***********
  -- * OUTPUTS *
  -- ***********

  -- STEP #7
  -- Write output to the screen.
  print_title (title);

  put_line ("Composite Simpson`s Rule - Algorithm 4.1");
  new_line;
  put_line (eq_text_f, " from ", a, " to ", b, ".");
  new_line;
  put_line ("n = ", n, " intervals on [a,b].");
  put_line ("Interval number h = ", h);
  new_line;
  put_line ("     ô", b);
  put_line ("XI = ³  f(x) dx = ", xi);
  put_line ("     õ", a);
  new_line;
  put_line ("Required ", n+1, " functional evaluations.");

  -- Write output to a file.
  -- Open outfile for output.
  create (file => file_id, mode => out_file, name => outfile);	-- See 14.3.10

  print_title_to_outfile (file_id, title);

  new_line (file_id);
  put_line (file_id, "Composite Simpson`s Rule - Algorithm 4.1");
  new_line (file_id);
  put_line (file_id, eq_text_f, " from ", a, " to ", b, ".");
  new_line (file_id);
  put_line (file_id, "n = ", n, " intervals on [a,b].");
  put_line (file_id, "Interval number h = ", h);
  new_line (file_id);
  put_line (file_id, "     ô", b);
  put_line (file_id, "XI = ³  f(x) dx = ", xi);
  put_line (file_id, "     õ", a);
  new_line (file_id);
  put_line (file_id, "Required ", n+1, " functional evaluations.");
  close (file => file_id);		-- Closes output file.

  new_line;
  put_line ("Output saved into file '", outfile, "'.");

end simpson;  				-- STOP

-- ****************************************************************************
-- *	    Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.	      *
-- ****************************************************************************
