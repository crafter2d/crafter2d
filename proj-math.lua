-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Math"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/math"
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/math/**.cpp", "src/math/**.h", "src/math/**.inl" }
