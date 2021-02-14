-- NAME: Harold Toomey
-- TASK: Hello World
-- LANG: Ada

with text_io;  use text_io;	-- For file I/O
with ada_io;   use ada_io;	-- For easier text, integer and float I/O
				-- NOTE: ada_io is supplied with the Meridian
				-- Ada Compiler

procedure HelloWorld is
  type file_type is limited private;
begin
  put_line ("Hello, World!");
  new_line;
end HelloWorld;
