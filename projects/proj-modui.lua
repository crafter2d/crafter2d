-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_ui"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/mods/mod_ui"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/mods/mod_ui/**.cpp", "../src/mods/mod_ui/**.h", "../src/mods/mod_ui/**.inl" }
	includedirs { "../src" }
	links { "Core" }
	defines { "TIXML_USE_STL" }
	removeprebuildcommands { cxxcommand }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		flags { "Symbols" }
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	-- Set system specific settings
	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		includedirs { path.join(libdir, "tinyxml/include") }
		libdirs { path.join(libdir, "tinyxml/lib") }
		links { "d3dcompiler.lib" }
	
	filter { "system:Windows", "Debug" }
		links { "tinyxmld_STL.lib" }
			
	filter { "system:Windows", "Release" }
		links { "tinyxml_STL.lib" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		
	-- Set IDE specific settings
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }
		