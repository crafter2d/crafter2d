-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

-- create the project
project "Xml"
	kind "SharedLib"
	location "../build/xml"
	
	setDefaultProjectSettings()
		
	-- set project files
	files { "../src/xml/**.cpp", "../src/xml/**.h", "../src/xml/**.inl" }
	includedirs { "../src" }
	defines { "TIXML_USE_STL" }

    filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Systems
	filter "system:Windows"
		defines { "WIN32", "XML_EXPORTS" }
		includedirs { pkgconf.cflags("tinyxml") }
		libdirs { pkgconf.libdir("tinyxml") }
		links { "Core" }
	
	filter { "system:Windows", "Debug" }
		links { pkgconf.libs("tinyxml") .. 'd' }
			
	filter { "system:Windows", "Release" }
		links { pkgconf.libs("tinyxml") }

	filter "system:Linux"
		defines { "LINUX" }
        removefiles { "../src/mods/mod_c2d/effect/builders/dx*" }
		buildoptions { "-std=c++11", "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
		
    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }

