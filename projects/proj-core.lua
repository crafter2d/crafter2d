-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/core"
	
	-- set project files
	files { "src/core/**.cpp", "src/core/**.c", "src/core/**.h", "src/core/**.inl" }
	includedirs { "src" }

	configuration "Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
	
	configuration "Windows"
		defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
		excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*" }
		includedirs { 	path.join(libdir, "zlib/include"),
						path.join(libdir, "iconv/include") }
		libdirs { 	path.join(libdir, "zlib/lib"),
					path.join(libdir, "iconv/lib") }
				
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
		
	configuration "Debug"
		links { "zlib1_d" }
				
	configuration "Release"
		links { "zlib1" }

	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-std=c++11", "-W", "-Wall", "-O0", "-Wunused-parameter" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
		excludes { "src/core/vfs/win*.*", "src/core/system/win*.*" }

