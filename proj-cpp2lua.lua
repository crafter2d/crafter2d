-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Cpp2Lua"
	kind "ConsoleApp"
	language "C++"
	targetdir "tolua"
	flags { "NoPCH" }
	location "build/cpp2lua"
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/cpp2lua/**.cpp", "src/cpp2lua/**.h", "src/cpp2lua/**.inl" }

if ( os.is("windows") ) then
	
	links { "Core" }

end

configuration "Debug"
	defines { "_DEBUG" }
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
