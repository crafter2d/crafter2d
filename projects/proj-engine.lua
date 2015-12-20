-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "../bin"
	location "../build/engine"
	flags { "NoPCH" }
	
	files { "../src/engine/**.cpp", "../src/engine/**.h", "../src/engine/**.inl" }
	includedirs { "../src" }
	links { "Core" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

    -- Platforms
	filter "system:Windows"
		defines { "WIN32", "ENGINE_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
		
		includedirs { 	path.join(libdir, "zlib/include"),
				path.join(libdir, "box2d/include") }

		libdirs { 	path.join(libdir, "zlib/lib"),
				path.join(libdir, "box2d/lib") }

	filter "system:Linux"
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		defines { "LINUX" }
		includedirs { "/usr/include", "/usr/local/include" }
        links { "Box2D" }

--        if _OPTIONS["with-box2d"] then
--            includedirs { path.join(_OPTIONS["with-box2d"], "Box2D"), _OPTIONS["with-box2d"] .. "/Box2D/Box2D" }
--            links { _OPTIONS["with-box2d"] .. "/Box2D/Build/Box2D/Box2D" }
--        else
--		    links { "Box2D" }
--        end

	-- Visual Studio
	filter "action:vs*"
		links { "gdi32", "user32", "vfw32", "ws2_32" }
		
	configuration { "vs*", "Debug" }
		links { "box2d_d", "zlib1_d" }
				
	configuration { "vs*", "Release" }
		links { "box2d", "zlib1" }
		
	-- CB support
	configuration "cb-gcc"
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition", "-Xlinker", "-zmuldefs" }
	  
	filter { "action:cb-gcc", "Debug" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32",
			"zlib1", "box2d_d" }
	 
	filter { "action:cb-gcc", "Release" }
		links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32",
			"zlib1", "box2d" }

