-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_c2d"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/mods/mod_c2d"
	flags { "NoPCH" }
		
	-- set project files
	files { "../src/mods/mod_c2d/**.cpp", "../src/mods/mod_c2d/**.h", "../src/mods/mod_c2d/**.inl" }
	includedirs { "../src" }
	links { "Core", "Engine" }
	defines { "TIXML_USE_STL" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		flags { "Symbols" }
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		includedirs { path.join(libdir, "tinyxml/include"), path.join(libdir, "zlib/include") }
		libdirs { path.join(libdir, "tinyxml/lib"), path.join(libdir, "zlib/lib") }
		links { "d3dcompiler.lib" }
	
	filter { "system:Windows", "Debug" }
		links { "tinyxmld_STL.lib", "zlib1_d.lib" }
			
	filter { "system:Windows", "Release" }
		links { "tinyxml_STL.lib", "zlib1.lib" }

	filter "system:Linux"
		defines { "LINUX" }
        removefiles { "../src/mods/mod_c2d/effect/builders/dx*" }
		buildoptions { "-std=c++11", "-W", "-Wall", "-O0" }
		
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }
		
