-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/engine"
	flags { "NoPCH" }
	
    -- set project files
	files { "../src/engine/**.cpp", "../src/engine/**.h", "../src/engine/**.inl" }
	includedirs { "../src" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

    -- Platforms
	filter "system:Windows"
		defines { "WIN32", "ENGINE_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
    	links { "Core" }
		includedirs { path.join(libdir, "box2d/include") }
		libdirs { path.join(libdir, "box2d/lib") }

	filter "system:Linux"
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		defines { "LINUX" }
		includedirs { "/usr/include", "/usr/local/include" }
        libdirs { "../bin" }
        links { "Box2D" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }

	-- Visual Studio
	filter "action:vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32" }
		
	configuration { "vs*", "Debug" }
		links { "box2d_d" }
				
	configuration { "vs*", "Release" }
		links { "box2d" }

