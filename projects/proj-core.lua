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
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		flags { "Optimize" }
	
	configuration "Windows"
		defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
		excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*" }
		includedirs { 	path.join(libdir, "zlib/include"),
						path.join(libdir, "tinyxml/include"),
						path.join(libdir, "iconv/include") }
		libdirs { 	path.join(libdir, "zlib/lib"),
					path.join(libdir, "tinyxml/lib"),
					path.join(libdir, "iconv/lib") }
				
	-- set IDE specific settings
	configuration "cb-gcc"
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
	configuration "cb-gcc and Debug"
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1", "tinyxmld_STL" }
	 
	configuration "cb-gcc and Release"
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1", "tinyxml_STL" }
	
	configuration "vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32", "dbghelp", "libiconv" }
		
	configuration "Debug"
		links { "tinyxmld_STL", "zlib1_d" }
				
	configuration "Release"
		links { "tinyxml_STL", "zlib1" }

	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-std=c++11", "-W", "-Wall", "-O0", "-Wunused-parameter" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
		excludes { "src/core/vfs/win*.*", "src/core/system/win*.*" }

