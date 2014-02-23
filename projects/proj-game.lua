-- JEngine SSE PreMake 5 configuration file
-- Copyright 2010-2013, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/game"
	debugdir "bin"
	
	files { "src/game/**.cpp", "src/game/**.h", "src/game/**.inl" }
	links { "Core", "Engine" }
	includedirs { "src" }
	
	configuration "Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		flags { "Optimize" }

	configuration "windows"
		defines { "WIN32" }
		includedirs { 	path.join(libdir, "sdl/include"),
						path.join(libdir, "tinyxml/include")
					}

		libdirs { 	path.join(libdir, "sdl/lib"),
					path.join(libdir, "tinyxml/lib"),
				}
		
		links { "SDLmain", "SDL", "user32", "vfw32", "ws2_32" }
	
	configuration "windows and Debug"
		links { "tinyxmld_STL" }
			
	configuration "windows and Release"
		links { "tinyxml_STL" }

	configuration "linux"
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
	   
		defines { "LINUX" }
		
		links { "SDL", "tinyxml" }

