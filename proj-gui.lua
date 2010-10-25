-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "GUI"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/gui"
	defines { "GUI_EXPORTS" }
	debugworkingdir "$(TargetDir)"
	
-- set project files
files { "src/gui/**.cpp", "src/gui/**.h", "src/gui/**.inl" }

-- set the include and library
if ( os.is("windows") ) then
	libdir = "../externallibs/" .. _ACTION .. "/"
	defines { "WIN32" }

	includedirs { libdir .. "sdl/include",
                    libdir .. "glee/include",
                    libdir .. "cg/include",
					libdir .. "openal/include",
					libdir .. "zlib/include",
					libdir .. "ogg/include",
					libdir .. "vorbis/include",
					libdir .. "tinyxml/include",
					libdir .. "lua/include", 
					libdir .. "tolua++/include",
					libdir .. "freetype2/include",
					libdir .. "soil/include",
					libdir .. "box2d/include",
					"src" }

    libdirs { libdir .. "sdl/lib",
				libdir .. "glee/lib",
				libdir .. "cg/lib",
				libdir .. "openal/lib",
				libdir .. "zlib/lib",
				libdir .. "ogg/lib",
				libdir .. "vorbis/lib",
				libdir .. "tinyxml/lib",
				libdir .. "lua/lib", 
				libdir .. "tolua++/lib",
				libdir .. "freetype2/lib",
				libdir .. "soil/lib",
				libdir .. "box2d/lib" }
	
	-- set the pre-build event
	prebuildcommands {
		"..\\..\\tolua\\tolua++ -o ..\\..\\src\\engine\\tolua_editor.cpp -H ..\\..\\src\\engine\\tolua_editor.h -n editor ..\\..\\tolua\\editor.cpp",
		"..\\..\\tolua\\tolua++ -o ..\\..\\src\\engine\\tolua_gui.cpp -H ..\\..\\src\\engine\\tolua_gui.h -n gui ..\\..\\tolua\\gui.cpp" }

	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
		
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { "GLee_d", "SOIL_d", "mingw32", "SDL", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32", "ilu", "lua", "tolua++_d", "freetype241MT_D" } 
		 
		configuration "Release"
			links { "GLee_d", "SOIL", "mingw32", "SDL", "opengl32", "glu32", "gdi32", 
					"user32", "vfw32", "ws2_32",  "OpenAL32", "ALut", "cg", "cgGL", "ilu", "vorbisfile",
					"lua", "tolua++", "freetype241MT"  } 

	else
		if ( _ACTION > "vs2005" ) then
			ignoredefaultlibs { "libcmt.lib" }
		end
		
		links { "SDL", "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32", 
				"OpenAL32", "ALut", "cg", "cgGL", "devil", "ilu"  }
		
		configuration "Debug"
			links { "GLee_d", "box2d_d", "tolua++_d", " lua5.1_d", "soil_d", "vorbisfile_d", "tinyxmld_STL", "zlib1_d", 
	                "minizip_d", "freetype242MT_D" }
			ignoredefaultlibs { "LIBC.lib", "msvcrt.lib" }
					
		configuration "Release"
			links { "GLee", "box2d", "tolua++", "lua5.1", "soil", "vorbisfile", "tinyxml_STL", "zlib1", 
					"minizip", "freetype242MT" }
   end -- win32
   
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
   
	excludes { "src/engine/vfs/win*.*", "src/engine/system/win*.*" }
	
	includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
	links { "SDL", "GL", "GLU", "GLee", "Cg", "CgGL",
           "lua", "tolua++-5.1", "openal", "alut",
           "png", "vorbisfile", "tinyxml", "minizip", "freetype", "box2d" }

	-- set the pre-build event
	prebuildcommands {
		"tolua++ -o ../src/engine/tolua_gui.cpp -H ../src/tolua_gui.h -n gui ../tolua/gui.cpp",
		"tolua++ -o ../src/engine/tolua_editor.cpp -H ../src/tolua_editor.h -n editor ../tolua/editor.cpp" }

end

configuration "Debug"
	defines { "_DEBUG", "TIXML_USE_STL" }
	flags { "Symbols" }
	
configuration "Release"
	defines { "NDEBUG", "TIXML_USE_STL" }
	flags { "Optimize" }
