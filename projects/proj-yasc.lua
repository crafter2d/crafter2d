-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Yasc"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/tools/yasc"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/tools/yasc/**.cpp", "../src/tools/yasc/**.c", "../src/tools/yasc/**.h", "../src/tools/yasc/**.inl" }
	includedirs { "../src", "../src/libraries", "../src/tools" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Systems
	filter "system:Windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "antlr/include") }
		libdirs { path.join(libdir, "antlr/lib") }
    	links { "Core", "libcil" }
	
	filter { "system:Windows", "Debug" }
		links { "antlr3cd.lib" }
			
	filter { "system:Windows", "Release" }
		links { "antlr3c.lib" }

	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
        links { "antlr3c" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-llibyas_cild" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-llibyas_cil" }

