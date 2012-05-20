-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Gen"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/gen"
	debugdir "bin"
	
-- set project files
files { "src/gen/**.cpp", "src/gen/**.h", "src/gen/**.inl" }
includedirs { "src" }

if ( os.is("windows") ) then
	includedirs { path.join(libdir, "ctemplate/include") }

	libdirs { path.join(libdir, "ctemplate/lib") }
	
	configuration "Debug"
		links { "libctemplate-debug" }
			
	configuration "Release"
		links { "libctemplate" }
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
	
	--links { "SDL", "tinyxml" }

end

configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "d"
	flags { "Symbols" }
	links { "Core" }
	
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	links { "Core" }
