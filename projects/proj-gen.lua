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
	removeprebuildcommands { cxxcommand }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		links { "Core" }
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		links { "Core" }

	filter "system:Windows"
		includedirs { path.join(libdir, "ctemplate/include"),
					  path.join(libdir, "icu/include")}

		libdirs { path.join(libdir, "ctemplate/lib") }
	
	filter { "system:Windows", "Debug" }
		links { "libctemplate-debug" }
			
	filter { "system:Windows", "Release" }
		links { "libctemplate" }

	filter "system:Linux"
		defines { "LINUX" }
		--links { "SDL", "tinyxml" }
		buildoptions { "-W", "-Wall", "-O0" }
		
	filter "action:cb-gcc"
		linkoptions { "-Xlinker", "-zmuldefs" }
