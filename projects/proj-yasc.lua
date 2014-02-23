-- JEngine SSE PreMake 5 configuration file
-- Copyright 2013, Jeroen Broekhuizen

-- create the project
project "Yasc"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/yasc"
	debugdir "bin"
	
	-- set project files
	files { "tools/yasc/**.cpp", "tools/yasc/**.c", "tools/yasc/**.h", "tools/yasc/**.inl" }
	includedirs { "src", "tools" }
	links { "Core" }
		
	configuration "Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "antlr/include") }
		libdirs { path.join(libdir, "antlr/lib") }
	
	configuration { "Debug", "windows" }
		links { "antlr3cd.lib" }
			
	configuration { "Release", "windows" }
		links { "antlr3c.lib" }

	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
