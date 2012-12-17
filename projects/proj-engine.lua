-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/engine"
	
-- set project files
files { "src/engine/**.cpp", "src/engine/**.h", "src/engine/**.inl" }
includedirs { "src" }

-- set the include and library
if ( os.is("windows") ) then
	defines { "WIN32", "ENGINE_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
	
	includedirs { 	path.join(libdir, "sdl/include"),
					path.join(libdir, "openal/include"),
					path.join(libdir, "zlib/include"),
					path.join(libdir, "ogg/include"),
					path.join(libdir, "vorbis/include"),
					path.join(libdir, "tinyxml/include"),
					path.join(libdir, "box2d/include"),
					path.join(libdir, "icu/include") }

    libdirs { 	path.join(libdir, "sdl/lib"),
				path.join(libdir, "openal/lib"),
				path.join(libdir, "zlib/lib"),
				path.join(libdir, "ogg/lib"),
				path.join(libdir, "vorbis/lib"),
				path.join(libdir, "tinyxml/lib"),
				path.join(libdir, "box2d/lib") }

	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
			
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "mingw32", "SDL", "gdi32", "user32", "vfw32", "ws2_32", "OpenAL32", "ALut", "ilu", "vorbisfile",
					"zlib1", "box2d_d", "freetype242MT_D", "tinyxmld_STL" } 
		 
		configuration "Release"
			links { "mingw32", "SDL", "gdi32", "user32", "vfw32", "ws2_32",  "OpenAL32", "ALut", "vorbisfile",
					"zlib1", "box2d", "freetype242MT", "tinyxml_STL" }

	else
		
		links { "SDL", "gdi32", "user32", "vfw32", "ws2_32", 
				"OpenAL32", "ALut"  }
		
		configuration "Debug"
			links { "box2d_d", "vorbisfile_d", "tinyxmld_STL", "zlib1_d" }
					
		configuration "Release"
			links { "box2d", "vorbisfile", "tinyxml_STL", "zlib1" }
   end -- win32
   
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
	
	includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
	links { "SDL", "GL", "GLU", "GLee", "Cg", "CgGL", "openal", "alut",
           "png", "vorbisfile", "tinyxml", "freetype", "Box2D", "SOIL" }

end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	targetsuffix "d"
	flags { "Symbols" }
	links { "Core", "Script" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
	links { "Core", "Script" }
