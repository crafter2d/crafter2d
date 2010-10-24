-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/game/**.cpp", "src/game/**.h", "src/game/**.inl" }
