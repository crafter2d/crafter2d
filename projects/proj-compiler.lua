-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Compiler"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/compiler"
	debugdir "bin"
	
	-- set project files
	files { "src/compiler/**.cpp", "src/compiler/**.h", "src/compiler/**.inl" }
	includedirs { "src" }
	links { "Core" }
		
	configuration "Debug"
		defines { "_DEBUG", "TIXML_USE_STL" }
		targetsuffix "d"
		flags { "Symbols" }
		links { "tinyxmld_STL" }
		
	configuration "Release"
		defines { "NDEBUG", "TIXML_USE_STL" }
		optimize "On"
		links { "tinyxml_STL" }

	configuration "windows"
		defines { "WIN32" }
		includedirs { path.join(libdir, "tinyxml/include") }
		libdirs { 	path.join(libdir, "tinyxml/lib") }
	
	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
