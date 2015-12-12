-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "UnitTest"
	kind "ConsoleApp"
	language "C++"
	debugdir "../bin"
	targetdir "../bin"
	location "../build/unittest"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/unittest/**.cpp", "../src/unittest/**.h", "../src/unittest/**.inl" }
	includedirs { "../src" }
	links { "Core", "Engine" }
	
	-- set up unit test framework
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		
	-- Platforms
	filter "system:windows"
		includedirs { 	path.join(libdir, "unittest/include"),
						path.join(libdir, "zlib/include") }
						
		libdirs { 	path.join(libdir, "zlib/lib"), path.join(libdir, "unittest/lib") }
		links { "unittest++" }
		
	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		links { "unittest++" }
		
