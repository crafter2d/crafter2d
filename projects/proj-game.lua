-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	language "C++"
	targetdir "../bin"
	debugdir "../bin"
	location "../build/game"
	flags { "NoPCH" }
	
	files { "../src/game/**.cpp", "../src/game/**.h", "../src/game/**.inl" }
	includedirs { "../src" }
	links { "Core", "Engine" }
	removeprebuildcommands { cxxcommand }
	
	filter "configurations:Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		flags { "Optimize" }

	-- System
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { 	path.join(libdir, "sdl/include"),
						path.join(libdir, "tinyxml/include")
					}

		libdirs { 	path.join(libdir, "sdl/lib"),
					path.join(libdir, "tinyxml/lib"),
				}
		
		links { "SDLmain", "SDL", "user32", "vfw32", "ws2_32" }

	filter "system:Linux"
		defines { "LINUX" }		
		links { "SDL", "tinyxml" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
	
	-- Toolsets
	filter { "action:vs*", "Debug" }
		links { "tinyxmld_STL" }
			
	filter { "action:vs*", "Release" }
		links { "tinyxml_STL" }
	
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }

