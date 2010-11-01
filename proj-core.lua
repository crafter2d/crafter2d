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
	defines { "WIN32", "CORE_EXPORTS" }
	
	excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*" }
	
	libdir = "../externallibs/" .. _ACTION .. "/"
	
	includedirs { libdir .. "glee/include",
                    libdir .. "cg/include",
					libdir .. "zlib/include",
					libdir .. "tinyxml/include",
					libdir .. "lua/include", 
					libdir .. "tolua++/include",
					"src" }
					
	libdirs { libdir .. "glee/lib",
				libdir .. "cg/lib",
				libdir .. "zlib/lib",
				libdir .. "tinyxml/lib",
				libdir .. "lua/lib", 
				libdir .. "tolua++/lib" }
				
	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "GLee_d", "mingw32", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32",  "cg", "cgGL",
					"minizip_d", "zlib1", "lua", "tolua++_d", "tinyxmld_STL"  } 
		 
		configuration "Release"
			links { "GLee_d", "SOIL", "mingw32", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32", "cg", "cgGL",
					"minizip", "zlib1", "lua", "tolua++", "tinyxml_STL"  } 
	else
		if ( _ACTION > "vs2005" ) then
			ignoredefaultlibs { "libcmt.lib" }
		end
		
		links { "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32", "cg", "cgGL"  }
		
		configuration "Debug"
			links { "GLee_d", "tolua++_d", " lua5.1_d", "tinyxmld_STL", "zlib1_d", "minizip_d" }
			ignoredefaultlibs { "LIBC.lib", "msvcrt.lib" }
					
		configuration "Release"
			links { "GLee", "tolua++", "lua5.1", "tinyxml_STL", "zlib1", "minizip" }
	end
elseif ( os.is("linux") ) then

	defines { "LINUX" }
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end

	excludes { "src/core/vfs/win*.*" }
		
end
