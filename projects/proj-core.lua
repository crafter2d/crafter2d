-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- local pkgconf = require "pkg-config"

-- create the project
project "Core"
	kind "SharedLib"
	location "../build/core"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/core/**.cpp", "../src/core/**.c", "../src/core/**.h", "../src/core/**.inl" }
	includedirs { "../src" }

	-- set up the build settings 
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	
	filter "system:Windows"
		defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
		excludes { "../src/core/vfs/linux*.*", "../src/core/system/linux*.*" }
		includedirs { pkgconf.cflags('libzip'), pkgconf.cflags('iconv') }
		libdirs { pkgconf.libdir('libzip'), pkgconf.libdir('iconv') }
							
	filter "system:Linux"
		defines { "LINUX" }
		excludes { "../src/core/vfs/win*.*", "../src/core/system/win*.*" }
		buildoptions { "-W", "-Wall", "-O0", "-Wunused-parameter", pkgconf.cflags('libzip') }
        	linkoptions { pkgconf.libs('libzip') }
		links { "SDL2" }

	-- set IDE specific settings
	filter "action:vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32", "dbghelp", pkgconf.libs('iconv') }
		
	filter { "action:vs*", "configurations:Debug" }
		links { pkgconf.libs('libzip') .. 'd' }
		
	filter { "action:vs*", "configurations:Release" }
		links { pkgconf.libs('libzip') }
