-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	location "../build/engine"
	
	setDefaultProjectSettings()
	
    -- set project files
	files { "../src/engine/**.cpp", "../src/engine/**.h", "../src/engine/**.inl" }
	includedirs { "../src" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Platforms
	filter "system:Windows"
		defines { "WIN32", "ENGINE_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
    	links { "Core" }
		includedirs { pkgconf.cflags('box2d') }
		libdirs { pkgconf.libdir('box2d') }

	filter "system:Linux"
		buildoptions { "-W", "-Wall", "-O0" }
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
		links { pkgconf.libs('box2d') .. 'd' }
				
	configuration { "vs*", "Release" }
		links { pkgconf.libs('box2d') }

