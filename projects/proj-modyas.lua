-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

project "mod_yas"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/mods/mod_yas"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/mods/mod_yas/**.cpp", "../src/mods/mod_yas/**.c", "../src/mods/mod_yas/**.h", "../src/mods/mod_yas/**.inl" }
	includedirs { "../src", "../src/mods" }
    libdirs { "../bin" }
	
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
	 
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	filter "system:Windows"
		defines { "WIN32", "SCRIPT_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
		includedirs { path.join(libdir, "zlib/include") }
		libdirs { path.join(libdir, "zlib/lib") }
    	links { "Core" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
        linkoptions { "-lCored" }
