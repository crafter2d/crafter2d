-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Compiler"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/compiler"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/compiler/**.cpp", "../src/compiler/**.h", "../src/compiler/**.inl" }
	includedirs { "../src" }
	links { "Core" }
		
	filter "configurations:Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		optimize "On"
		
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { pkgconf.cflags('tinyxml') }
		libdirs { pkgconf.libdir('tinyxml') }
	
	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
        links { "tinyxml" }

    filter { "action:vs*", "Debug" }
        links { pkgconf.libs('tinyxml') .. 'd' }

    filter { "action:vs*", "Debug" }
        links { pkgconf.libs('tinyxml') }

