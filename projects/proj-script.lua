-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

project "Script"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/script"
	debugworkingdir "$(TargetDir)"

-- set project files
files { "src/script/**.cpp", "src/script/**.c", "src/script/**.h", "src/script/**.inl" }
includedirs { "src" }

if ( os.is("windows") ) then
	defines { "WIN32", "SCRIPT_EXPORTS" }
	
	includedirs { path.join(libdir, "antlr/include") }
	libdirs { path.join(libdir, "antlr/lib") }
else
	-- linux
end

configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "d"
    flags { "Symbols" }
	links { "antlr3cd", "Core" }
 
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	links { "antlr3c", "Core" }