-- ****************************************************************************
-- * PACKAGE:  NAA_UTIL.ADA                                                   *
-- * VERSION:  4.0                                                            *
-- *                                                                          *
-- * WRITTEN BY:  Harold A. Toomey, CARE-FREE SOFTWARE               	      *
-- * DATE:        22 May 1991  (2Q 1991)                                      *
-- *                                                                          *
-- * DESCRIPTION:                                                             *
-- *   Contains useful utilities (functions and procedures) to be used with   *
-- *   "Numerical Analysis Algorithms in Ada" v4.0.                           *
-- *                                                                          *
-- * VARIABLES:                                                               *
-- *   numchars : integer;                                                    *
-- *                                                                          *
-- * FUNCTIONS:                                                               *
-- *   get_title()                                                            *
-- *                                                                          *
-- * PROCEDURES:                                                              *
-- *   print_title()                                                          *
-- *   print_title_to_file()                                                  *
-- *                                                                          *
-- ****************************************************************************

with ada_io;  use ada_io;

-- ****************************************************************************
--  Package specification
-- ****************************************************************************
package NAA_UTIL is

  numchars : integer;		-- Stores title length in characters.

  function get_title return string;

  procedure print_title(
      file_id : in file_type;
      title   : in string
  );

  procedure print_title_to_outfile(
      file_id : in file_type;
      title   : in string
  );

end NAA_UTIL;


-- ****************************************************************************
--  Package body
-- ****************************************************************************

package body NAA_UTIL is

-------------------------------------------------------------------------------
-- get_title() - Prompts for an optional title.
-------------------------------------------------------------------------------

function get_title return string is
  title : string(1..80);
begin  -- get_title
  for i in 1..79 loop		-- Print the NAA banner
    put ("-");
  end loop;
  new_line;
  put_line(ht, ht, "'Numerical Analysis Algorithms in Ada' v4.0");
  for i in 1..79 loop
    put ("-");
  end loop;
  new_line;			-- Get the title
  put_line ("Enter a title [ie - Set 2.1,  Problem 2 a) ].");
  put ("----> ");
  get_line (title);
  numchars := title'length;	-- Numchars stores the length of the title.
  return (title);
end get_title;


-------------------------------------------------------------------------------
-- print_title() - Prints NAA banner and title to the screen.
-------------------------------------------------------------------------------

procedure print_title (file_id : in file_type, title : in string) is
begin  -- print_title
  for i in 1..79 loop		-- Print the NAA banner
    put ("-");
  end loop;
  new_line;
  put_line(ht, ht, "'Numerical Analysis Algorithms in Ada' v4.0");
  for i in 1..79 loop
    put ("-");
  end loop;
  new_line;
  if numchars > 0 then
    put_line (title);		-- Print the title
    new_line;
  end if;
end print_title;


-------------------------------------------------------------------------------
-- print_title_to_outfile() - Prints NAA banner and title to the output file.
-------------------------------------------------------------------------------

procedure print_title_to_outfile (file_id : in file_type, title : in string) is
begin  -- print_title_to_outfile
  for i in 1..79 loop		-- Print the NAA banner
    put (file_id, "-");
  end loop;
  new_line (file_id);
  put_line(file_id, ht, ht, "'Numerical Analysis Algorithms in Ada' v4.0");
  for i in 1..79 loop
    put (file_id, "-");
  end loop;
  new_line (file_id);
  if numchars > 0 then
    put_line (file_id, title);	-- Print the title
    new_line (file_id);
  end if;
end print_title_to_outfile;

-------------------------------------------------------------------------------


end NAA_UTIL;	-- end package body NAA_UTIL

-- ****************************************************************************
-- *	   Copyright (C) 1991, Harold A. Toomey, All Rights Reserved.	      *
-- ****************************************************************************
