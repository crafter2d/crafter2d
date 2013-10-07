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
	defines { "WIN32", "CORE_EXPORTS", "UNICODE" }
	
	excludes { "src/core/vfs/linux*.*", "src/core/system/linux*.*", "src/core/string/linux*.*" }
		
	includedirs { 	path.join(libdir, "zlib/include"),
					path.join(libdir, "tinyxml/include") }
					
	libdirs { 	path.join(libdir, "zlib/lib"),
				path.join(libdir, "tinyxml/lib") }
				
	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1", "tinyxmld_STL" }
		 
		configuration "Release"
			links { "mingw32", "gdi32", "user32", "vfw32", "ws2_32", "zlib1", "tinyxml_STL" }
	else

		links { "gdi32", "user32", "vfw32", "ws2_32" }
		
		configuration "Debug"
			links { "tinyxmld_STL", "zlib1_d" }
					
		configuration "Release"
			links { "tinyxml_STL", "zlib1" }
	end
elseif ( os.is("linux") ) then

	defines { "LINUX" }
	
	buildoptions { "-W", "-Wall", "-O0", "-Wunused-parameter" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end

	excludes { "src/core/vfs/win*.*", "src/core/system/win*.*", "src/core/vfs/zip/*32.*", "src/core/string/win*.*" }		
end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	targetsuffix "d"
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
