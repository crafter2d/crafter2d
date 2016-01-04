-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Gen"
	kind "ConsoleApp"
	language "C++"
	debugdir "../bin"
	targetdir "../bin"
	location "../build/gen"
	flags { "NoPCH" }
	
	-- set project files
	files { "../src/gen/**.cpp", "../src/gen/**.h", "../src/gen/**.inl" }
	includedirs { "../src" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		
    -- Systems
	filter "system:Windows"
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
        libdir { "..\bin" }
		
    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }

