-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

project "libyasc"
	kind "StaticLib"
	location "../build/libraries/libyasc"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/libraries/libyasc/**.cpp", "../src/libraries/libyasc/**.h", "../src/libraries/libyasc/**.inl" }
	includedirs { "../src", "../src/libraries" }
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
		includedirs { path.join(libdir, "antlr/include") }
		links { "Core", "libyas_cil" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0", "-fpic" }
        libdirs { "../bin" }
