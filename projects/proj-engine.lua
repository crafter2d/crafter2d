-- JEngine SSE PreMake 5 configuration file
-- Copyright 2010-2013, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/engine"
	
	files { "src/engine/**.cpp", "src/engine/**.h", "src/engine/**.inl" }
	includedirs { "src" }
	links { "Core" }

	configuration "Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	-- Visual Studio
	configuration "vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32" }
		
	configuration { "vs*", "Debug" }
		links { "box2d_d", "zlib1_d" }
				
	configuration { "vs*", "Release" }
		links { "box2d", "zlib1" }
		
	-- CB support
	configuration "cb-gcc"
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
	configuration { "cb-gcc", "Debug" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32",
			"zlib1", "box2d_d" }
	 
	configuration { "cb-gcc", "Release" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32",
			"zlib1", "box2d" }

	-- Windows
	configuration "Windows"
		defines { "WIN32", "ENGINE_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
		
		includedirs { 	path.join(libdir, "zlib/include"),
				path.join(libdir, "box2d/include") }

		libdirs { 	path.join(libdir, "zlib/lib"),
				path.join(libdir, "box2d/lib") }

	-- Linux
	configuration "linux"
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
	   
		defines { "LINUX" }
	
		includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
		links { "Box2D" }
