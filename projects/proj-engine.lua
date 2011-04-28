-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "Engine"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/engine"
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/engine/**.cpp", "src/engine/**.h", "src/engine/**.inl" }
includedirs { "src" }

-- set the include and library
if ( os.is("windows") ) then
	defines { "WIN32", "ENGINE_EXPORTS" }
	
	includedirs { 	path.join(libdir, "sdl/include"),
                    path.join(libdir, "glee/include"),
                    path.join(libdir, "cg/include"),
					path.join(libdir, "openal/include"),
					path.join(libdir, "zlib/include"),
					path.join(libdir, "ogg/include"),
					path.join(libdir, "vorbis/include"),
					path.join(libdir, "tinyxml/include"),
					path.join(libdir, "lua/include"), 
					path.join(libdir, "tolua++/include"),
					path.join(libdir, "freetype2/include"),
					path.join(libdir, "soil/include"),
					path.join(libdir, "box2d/include") }

    libdirs { 	path.join(libdir, "sdl/lib"),
				path.join(libdir, "glee/lib"),
				path.join(libdir, "cg/lib"),
				path.join(libdir, "openal/lib"),
				path.join(libdir, "zlib/lib"),
				path.join(libdir, "ogg/lib"),
				path.join(libdir, "vorbis/lib"),
				path.join(libdir, "tinyxml/lib"),
				path.join(libdir, "lua/lib"), 
				path.join(libdir, "tolua++/lib"),
				path.join(libdir, "freetype2/lib"),
				path.join(libdir, "soil/lib"),
				path.join(libdir, "box2d/lib") }
	
	-- set the pre-build event
	prebuildcommands {
		"..\\..\\tolua\\tolua++ -o ..\\..\\src\\engine\\tolua_general.cpp -H ..\\..\\src\\engine\\tolua_general.h -n general ..\\..\\tolua\\general.cpp",
		"..\\..\\tolua\\tolua++ -o ..\\..\\src\\engine\\tolua_network.cpp -H ..\\..\\src\\engine\\tolua_network.h -n network ..\\..\\tolua\\network.cpp",
		"..\\..\\tolua\\tolua++ -o ..\\..\\src\\engine\\tolua_physics.cpp -H ..\\..\\src\\engine\\tolua_physics.h -n physics ..\\..\\tolua\\physics.cpp" }

	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		-- no devil support yet in cb
		excludes { "src/engine/texture/textureloaderdevil.cpp", "src/engine/texture/textureloaderdevil.h" }
		
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "GLee_d", "SOIL_d", "mingw32", "SDL", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32",  "OpenAL32", "ALut", "cg", "cgGL", "ilu", "vorbisfile",
					"minizip_d", "zlib1", "lua", "tolua++_d", "box2d_d", "freetype241MT_D", "tinyxmld_STL", "Core"  } 
		 
		configuration "Release"
			links { "GLee_d", "SOIL", "mingw32", "SDL", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32",  "OpenAL32", "ALut", "cg", "cgGL", "ilu", "vorbisfile",
					"minizip", "zlib1", "lua", "tolua++", "box2d", "freetype241MT", "tinyxml_STL", "Core"  } 

	else
		if ( _ACTION > "vs2005" ) then
			ignoredefaultlibs { "libcmt.lib" }
		end
		
		includedirs { libdir .. "devil/include" }
		libdirs { libdir .. "devil/lib" }
		links { "SDL", "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32", 
				"OpenAL32", "ALut", "cg", "cgGL", "devil", "ilu"  }
		
		configuration "Debug"
			links { "GLee_d", "box2d_d", "tolua++_d", " lua5.1_d", "soil_d", "vorbisfile_d", "tinyxmld_STL", "zlib1_d", 
	                "minizip_d", "freetype242MT_D", "Core" }
			ignoredefaultlibs { "LIBC.lib", "msvcrt.lib" }
					
		configuration "Release"
			links { "GLee", "box2d", "tolua++", "lua5.1", "soil", "vorbisfile", "tinyxml_STL", "zlib1", 
					"minizip", "freetype242MT", "Core" }
   end -- win32
   
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
	
	includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
	links { "SDL", "GL", "GLU", "GLee", "Cg", "CgGL",
           "lua", "tolua++-5.1", "openal", "alut",
           "png", "vorbisfile", "tinyxml", "minizip", "freetype", "box2d", "Core" }

	-- set the pre-build event
	prebuildcommands {
		"tolua++ -o ../src/engine/tolua_general.cpp -H ../src/tolua_general.h -n general ../tolua/general.cpp",
		"tolua++ -o ../src/engine/tolua_network.cpp -H ../src/tolua_network.h -n network ../tolua/network.cpp",
		"tolua++ -o ../src/engine/tolua_physics.cpp -H ../src/tolua_physics.h -n physics ../tolua/physics.cpp" }

end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
