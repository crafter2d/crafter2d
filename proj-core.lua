-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/core"
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/core/**.cpp", "src/core/**.h", "src/core/**.inl" }

if ( os.is("windows") ) then
	-- this is an export dll
	defines { "CORE_EXPORTS" }
	
	libdir = "../externallibs/" .. _ACTION .. "/"
	
	includedirs { libdir .. "sdl/include",
                    libdir .. "glee/include",
                    libdir .. "cg/include",
					libdir .. "zlib/include",
					libdir .. "tinyxml/include",
					libdir .. "lua/include", 
					libdir .. "tolua++/include",
					"src" }
					
	libdirs { libdir .. "sdl/lib",
				libdir .. "glee/lib",
				libdir .. "cg/lib",
				libdir .. "zlib/lib",
				libdir .. "tinyxml/lib",
				libdir .. "lua/lib", 
				libdir .. "tolua++/lib" }
end
