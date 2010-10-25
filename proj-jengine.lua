-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/game"
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/game/**.cpp", "src/game/**.h", "src/game/**.inl" }

if ( os.is("windows") ) then
	libdir = "../externallibs/" .. _ACTION .. "/"

	includedirs { libdir .. "sdl/include",
					libdir .. "glee/include",
					libdir .. "lua/include", 
					libdir .. "tolua++/include",
					"src" }
	
	links { "SDLmain", "SDL", "Math", "Engine" }
end
