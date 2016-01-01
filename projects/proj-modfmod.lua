-- Crafter 2D PreMake 5 configuration file
-- Copyright 2014, Jeroen Broekhuizen

-- create the project
project "mod_fmod"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/mods/mod_fmod"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/mods/mod_fmod/**.cpp", "../src/mods/mod_fmod/**.h", "../src/mods/mod_fmod/**.inl" }
	includedirs { "../src" }
	links { "Core" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		flags { "Symbols" }
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	--- Windows
	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		includedirs { path.join(libdir, "fmod/include") }
		libdirs { path.join(libdir, "fmod/lib") }
		links { "fmodex_vc" }

	--- Linux
	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }

