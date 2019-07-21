-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Game"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/game"
	
	setDefaultProjectSettings()
	
    -- set project files
	files { "../src/game/**.cpp", "../src/game/**.h", "../src/game/**.inl" }
	includedirs { "../src" }
	
	filter "configurations:Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		optimize "On"

	-- System
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { 	path.join(libdir, "sdl/include"),
						pkgconf.cflags('tinyxml')
					}

		libdirs { 	path.join(libdir, "sdl/lib"),
					pkgconf.libdir('tinyxml'),
				}
		
		links { "Core", "Engine", "SDLmain", "SDL", "user32", "vfw32", "ws2_32" }

	filter "system:Linux"
		defines { "LINUX" }		
        excludes { "../src/game/win32*.*" }
        libdirs { "../bin" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        links { "SDL", "tinyxml" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-lEngined" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-lEngine" }

	-- Toolsets
	filter { "action:vs*", "Debug" }
		links { pkgconf.libs('tinyxml') .. 'd' }
			
	filter { "action:vs*", "Release" }
		links { pkgconf.libs('tinyxml') }

