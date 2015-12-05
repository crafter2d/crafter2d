-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Compiler"
	kind "ConsoleApp"
	language "C++"
	targetdir "../bin"
	debugdir "../bin"
	location "../build/compiler"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/compiler/**.cpp", "../src/compiler/**.h", "../src/compiler/**.inl" }
	includedirs { "../src" }
	links { "Core" }
	removeprebuildcommands { cxxcommand }
		
	filter "configurations:Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		optimize "On"
		
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "tinyxml/include") }
		libdirs { 	path.join(libdir, "tinyxml/lib") }
	
	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        links { "tinyxml" }
		
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }

    filter { "action:vs*", "Debug" }
        links { "tinyxmld_STL" }

    filter { "action:vs*", "Debug" }
        links { "tinyxml_STL" }

