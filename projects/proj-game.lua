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
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	-- System
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "sdl/include") }

		libdirs { path.join(libdir, "sdl/lib") }
		
		links { "Core", "Engine", "SDLmain", "SDL", "user32", "vfw32", "ws2_32" }

	filter "system:Linux"
		defines { "LINUX" }		
        excludes { "../src/game/win32*.*" }
        libdirs { "../bin" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        links { "SDL" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-lEngined" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-lEngine" }
