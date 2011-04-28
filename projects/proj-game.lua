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
	includedirs { 	path.join(libdir, "sdl/include"),
					path.join(libdir, "glee/include"),
					path.join(libdir, "lua/include"), 
					path.join(libdir, "tolua++/include"),
					path.join(libdir, "tinyxml/include"),
					path.join(libdir, "openal/include"),
					path.join(libdir, "zlib/include"),
					"src" }

	libdirs { 	path.join(libdir, "sdl/lib"),
				path.join(libdir, "glee/lib"),
				path.join(libdir, "zlib/lib"),
				path.join(libdir, "tinyxml/lib"),
				path.join(libdir, "lua/lib"), 
				path.join(libdir, "tolua++/lib") }
	
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
