-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

project "yas-cil"
	kind "StaticLib"
	location "../build/libraries/yas-cil"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/libraries/yas-cil/**.cpp", "../src/libraries/yas-cil/**.h", "../src/libraries/yas-cil/**.inl" }
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
