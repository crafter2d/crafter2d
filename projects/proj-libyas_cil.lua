-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

project "libyas_cil"
	kind "StaticLib"
	location "../build/libraries/libyas_cil"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/libraries/libcil/**.cpp", "../src/libraries/libcil/**.h", "../src/libraries/libcil/**.inl" }
	includedirs { "../src" }
	libdirs { "../bin" }
	
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
	 
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Systems
	filter "system:Windows"
		defines { "WIN32", "_ALLOW_KEYWORD_MACROS" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0", "-fpic" }
        libdirs { "../bin" }
