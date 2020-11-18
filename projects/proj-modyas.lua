-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

project "mod_yas"
	kind "SharedLib"
	location "../build/mods/mod_yas"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/mods/mod_yas/**.cpp", "../src/mods/mod_yas/**.c", "../src/mods/mod_yas/**.h", "../src/mods/mod_yas/**.inl" }
	includedirs { "../src", "../src/mods", "../src/libraries" }
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
		defines { "WIN32", "SCRIPT_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
		includedirs { path.join(libdir, "zlib/include") }
		libdirs { path.join(libdir, "zlib/lib") }
    	links { "Core", "yas-cil" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
        libdirs { "../bin" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-llibyas_cild" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-llibyas_cil" }
