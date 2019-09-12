-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_c2d"
	kind "SharedLib"
	location "../build/mods/mod_c2d"
	
	setDefaultProjectSettings()
		
	-- set project files
	files { "../src/mods/mod_c2d/**.cpp", "../src/mods/mod_c2d/**.h", "../src/mods/mod_c2d/**.inl" }
	includedirs { "../src" }

    filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Systems
	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		includedirs { path.join(libdir, "zlib/include"), path.join(libdir, "libsquish/include") }
		libdirs { path.join(libdir, "zlib/lib"), path.join(libdir, "libsquish/lib") }
		links { "Core", "Engine", "Xml", "d3dcompiler.lib" }
	
	filter { "system:Windows", "Debug" }
		links { "zlibd.lib", "squishd.lib" }
			
	filter { "system:Windows", "Release" }
		links { "zlib.lib", "squish.lib" }

	filter "system:Linux"
		defines { "LINUX" }
        removefiles { "../src/mods/mod_c2d/effect/builders/dx*" }
		buildoptions { "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
		
    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-lXmld" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-lXml" }

