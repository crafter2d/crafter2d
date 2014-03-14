-- Crafter 2D PreMake 5 configuration file
-- Copyright 2014, Jeroen Broekhuizen

-- create the project
project "mod_fmod"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/mods/mod_fmod"
	
	-- set project files
	files { "src/mods/mod_fmod/**.cpp", "src/mods/mod_fmod/**.h", "src/mods/mod_fmod/**.inl" }
	includedirs { "src" }
	links { "Core" }
	defines { "MOD_EXPORTS" }
		
	configuration "Debug"
		defines { "_DEBUG" }
		flags { "Symbols" }
		targetsuffix "d"
		
	configuration "Release"
		defines { "NDEBUG" }
		optimize "On"

	--- Windows
	configuration "windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "fmod/include") }
		libdirs { path.join(libdir, "fmod/lib") }
		links { "fmodex_vc" }

	--- Linux
	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
