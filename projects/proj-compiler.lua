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
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG" }
		optimize "On"

	configuration "windows"
		defines { "WIN32" }
	
	configuration "linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		if ( _ACTION == "cb-gcc" ) then
			linkoptions { "-Xlinker", "-zmuldefs" }
		end
