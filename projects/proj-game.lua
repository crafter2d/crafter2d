-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/game"
	debugdir "bin"
	
-- set project files
files { "src/game/**.cpp", "src/game/**.h", "src/game/**.inl" }
includedirs { "src" }

if ( os.is("windows") ) then
	includedirs { 	path.join(libdir, "sdl/include"),
					path.join(libdir, "glee/include"),
					path.join(libdir, "tinyxml/include"),
					path.join(libdir, "openal/include"),
					path.join(libdir, "zlib/include") }

	libdirs { 	path.join(libdir, "sdl/lib"),
				path.join(libdir, "glee/lib"),
				path.join(libdir, "zlib/lib"),
				path.join(libdir, "tinyxml/lib") }
	
	links { "SDLmain", "SDL", "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32" }
	
	configuration "Debug"
		links { "GLee_d", "tinyxmld_STL" }
			
	configuration "Release"
		links { "GLee", "tinyxml_STL" }
end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	targetsuffix "d"
	flags { "Symbols" }
	links { "Core", "Engine", "Script" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
	links { "Core", "Engine", "Script" }
