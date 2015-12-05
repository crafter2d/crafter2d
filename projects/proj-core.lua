-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	flags { "NoPCH" }
	location "../build/core"
	
	-- set project files
	files { "../src/core/**.cpp", "../src/core/**.c", "../src/core/**.h", "../src/core/**.inl" }
	includedirs { "../src" }
	removeprebuildcommands { cxxcommand }

	-- set up the build settings 
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
	
	filter "system:Windows"
		defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
		excludes { "../src/core/vfs/linux*.*", "../src/core/system/linux*.*" }
		includedirs { 	path.join(libdir, "zlib/include"),
						path.join(libdir, "iconv/include") }
		libdirs { 	path.join(libdir, "zlib/lib"),
					path.join(libdir, "iconv/lib") }
					
	filter "system:Linux"
		defines { "LINUX" }
		excludes { "../src/core/vfs/win*.*", "../src/core/system/win*.*" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0", "-Wunused-parameter" }
        links { "z", "minizip", "SDL2" }
				
	-- set IDE specific settings
	filter "action:vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32", "dbghelp", "libiconv" }

    filter { "action:vs*", "Debug" }
		links { "zlib1_d" }

    filter { "action:vs*", "Release" }
        links { "zlib1" }
		
	filter "action:cb-gcc"
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition", "-Xlinker", "-zmuldefs" }
	  
	filter { "action:cb-gcc", "Debug" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32" }
	 
	filter { "action:cb-gcc", "Release" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32" }
