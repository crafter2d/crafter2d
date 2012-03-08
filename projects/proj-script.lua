-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

project "Script"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/script"
	
-- set project files
files { "src/script/**.cpp", "src/script/**.c", "src/script/**.h", "src/script/**.inl" }
includedirs { "src" }

if ( os.is("windows") ) then
	defines { "WIN32", "SCRIPT_EXPORTS" }
	
	includedirs { 	path.join(libdir, "antlr/include"),
					path.join(libdir, "zlib/include") }
					
	libdirs { 	path.join(libdir, "antlr/lib"),
				path.join(libdir, "zlib/lib") }
else
	-- linux
end

configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "d"
    flags { "Symbols" }
	links { "antlr3c", "Core" }
 
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	links { "antlr3c", "Core" }