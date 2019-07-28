-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Gen"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/tools/gen"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/tools/gen/**.cpp", "../src/tools/gen/**.h", "../src/tools/gen/**.inl" }
	includedirs { "../src" }
	
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
    -- Systems
	filter "system:Windows"
		defines { "WIN32", "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS" }
        links { "Core" }
		includedirs { path.join(libdir, "ctemplate/include"),
					  path.join(libdir, "icu/include")}

		libdirs { path.join(libdir, "ctemplate/lib") }
	
	filter { "system:Windows", "Debug" }
		links { "libctemplate-debug" }
			
	filter { "system:Windows", "Release" }
		links { "libctemplate" }

	filter "system:Linux"
		defines { "LINUX" }
		links { "ctemplate", "tinyxml" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        libdirs { "..\bin" }

	filter { "system:linux", "Debug" }
        linkoptions { "-lCored", "-lEngined" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore", "-lEngine" }
