-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Yasc"
	kind "ConsoleApp"
	language "C++"
	debugdir "../bin"
	targetdir "../bin"
	location "../build/yasc"
	flags { "NoPCH" }
	
	-- set project files
	files { "../tools/yasc/**.cpp", "../tools/yasc/**.c", "../tools/yasc/**.h", "../tools/yasc/**.inl" }
	includedirs { "../src", "../tools" }
	links { "Core" }
	removeprebuildcommands { cxxcommand }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "system:Windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "antlr/include") }
		libdirs { path.join(libdir, "antlr/lib") }
	
	filter { "system:Windows", "Debug" }
		links { "antlr3cd.lib" }
			
	filter { "system:Windows", "Release" }
		links { "antlr3c.lib" }

	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }
