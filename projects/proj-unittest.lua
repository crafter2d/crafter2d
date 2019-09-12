-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

-- create the project
project "UnitTest"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/unittest"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/unittest/**.cpp", "../src/unittest/**.h", "../src/unittest/**.inl" }
	includedirs { "../src" }
	
	-- set up unit test framework
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	-- Platforms
	filter "system:windows"
		includedirs { pkgconf.cflags("googletest") }	
		libdirs { pkgconf.libdir("googletest") }
	    links { "Core", "Engine" }
		
	filter { "system:windows", "Debug" }
		links { pkgconf.libs('googletest') .. 'd' }
	
	filter { "system:windows", "Release" }
		links { pkgconf.libs('googletest') }
				
	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0", pkgconf.cflags('gtest') }
		links { 'gtest', 'pthread' }
		libdirs { "../bin" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-lEngined" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-lEngine" }
