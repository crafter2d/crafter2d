-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Compiler"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/tools/compiler"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/tools/compiler/**.cpp", "../src/tools/compiler/**.h", "../src/tools/compiler/**.inl" }
	includedirs { "../src" }
	links { "Core", "Xml" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
	filter "system:Windows"
		defines { "WIN32" }
	
	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
