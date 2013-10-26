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
	dependson { "D3D" }
	
	files { "src/game/**.cpp", "src/game/**.h", "src/game/**.inl" }
	links { "Core", "Engine", "Script" }
	includedirs { "src" }
	
	configuration "Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		flags { "Optimize" }

	configuration "windows"
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

