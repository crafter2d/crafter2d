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
					libdir .. "tinyxml/include",
					"src" }

	libdirs { libdir .. "sdl/lib",
				libdir .. "glee/lib",
				libdir .. "zlib/lib",
				libdir .. "tinyxml/lib",
				libdir .. "lua/lib", 
				libdir .. "tolua++/lib" }
	
	links { "SDLmain", "SDL", "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32", "Core", "Engine" }
	
	configuration "Debug"
		links { "GLee_d", "tolua++_d", " lua5.1_d", "tinyxmld_STL" }
		ignoredefaultlibs { "LIBC.lib", "msvcrt.lib" }
			
	configuration "Release"
		links { "GLee", "tolua++", "lua5.1", "tinyxml_STL" }
end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
