-- Crafter2D PreMake 5 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/core"
	
	-- set project files
	files { "../src/core/**.cpp", "../src/core/**.c", "../src/core/**.h", "../src/core/**.inl" }
	includedirs { "../src" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		links { "zlib1_d" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		links { "zlib1" }
	
	filter "system:windows"
		defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
		excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*" }
		includedirs { 	path.join(libdir, "zlib/include"),
						path.join(libdir, "iconv/include") }
		libdirs { 	path.join(libdir, "zlib/lib"),
					path.join(libdir, "iconv/lib") }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-std=c++11", "-W", "-Wall", "-O0", "-Wunused-parameter" }
		excludes { "../src/core/vfs/win*.*", "../src/core/system/win*.*" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
				
	-- set IDE specific settings
	configuration "cb-gcc"	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
	configuration "cb-gcc and Debug"
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1" }
	 
	configuration "cb-gcc and Release"
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1" }
	
	configuration "vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32", "dbghelp", "libiconv" }
		

