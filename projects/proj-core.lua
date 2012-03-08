-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Core"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/core"
	
-- set project files
files { "src/core/**.cpp", "src/core/**.c", "src/core/**.h", "src/core/**.inl" }
includedirs { "src" }

if ( os.is("windows") ) then
	-- this is an export dll
	defines { "WIN32", "CORE_EXPORTS" }
	
	excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*" }
		
	includedirs { 	path.join(libdir, "glee/include"),
                    path.join(libdir, "cg/include"),
					path.join(libdir, "zlib/include"),
					path.join(libdir, "tinyxml/include"),
					path.join(libdir, "icu/include") }
					
	libdirs { 	path.join(libdir, "glee/lib"),
				path.join(libdir, "cg/lib"),
				path.join(libdir, "zlib/lib"),
				path.join(libdir, "tinyxml/lib"),
				path.join(libdir, "icu/lib") }
				
	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "GLee_d", "mingw32", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32",  "cg", "cgGL",
					"minizip_d", "zlib1", "tinyxmld_STL"  } 
		 
		configuration "Release"
			links { "GLee_d", "SOIL", "mingw32", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32", "cg", "cgGL",
					"minizip", "zlib1", "tinyxml_STL"  } 
	else

	links { "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32", "cg", "cgGL", "icuuc" }
		
		configuration "Debug"
			links { "GLee_d", "tinyxmld_STL", "zlib1_d", "minizip_d" }
					
		configuration "Release"
			links { "GLee", "tinyxml_STL", "zlib1", "minizip" }
	end
elseif ( os.is("linux") ) then

	defines { "LINUX" }
	
	buildoptions { "-W", "-Wall", "-O0", "-Wunused-parameter" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end

	excludes { "src/core/vfs/win*.*", "src/core/system/win*.*", "src/core/vfs/zip/*32.*" }
		
end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	targetsuffix "d"
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
