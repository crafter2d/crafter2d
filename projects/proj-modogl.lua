-- JEngine SSE PreMake 5 configuration file
-- Copyright 2010-2013, Jeroen Broekhuizen

-- create the project
project "mod_ogl"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/mods/mod_ogl"
	
-- set project files
files { "src/mods/mod_ogl/**.cpp", "src/mods/mod_ogl/**.h", "src/mods/mod_ogl/**.inl" }
includedirs { "src", "src/mods" }

-- set the include and library
if ( os.is("windows") ) then
   defines { "WIN32", "OGL_EXPORTS" }
	
   includedirs {
      path.join(libdir, "glee/include"),
	  path.join(libdir, "sdl/include"),
      path.join(libdir, "soil/include"),
	  path.join(libdir, "freetype2/include"),
   }

   libdirs {
      path.join(libdir, "glee/lib"),
	  path.join(libdir, "sdl/lib"),
      path.join(libdir, "soil/lib"),
	  path.join(libdir, "freetype2/lib"),
   }

	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
       
		configuration "Debug"
			links { "GLee_d", "SOIL_d", "freetype242MT_D" } 
		 
		configuration "Release"
			links { "GLee_d", "SOIL", "freetype242MT" }

	else
		links { "opengl32", "glu32" }
		
		configuration "Debug"
			links { "GLee_d", "SOIL_d", "freetype242MT_D" }
					
		configuration "Release"
			links { "GLee", "SOIL", "freetype242MT_D" }
   end -- win32
   
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
	
	includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
	links { "GLee", "freetype", "SOIL" }

end

configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "d"
	flags { "Symbols" }
	links { "Core", "SDL" }
	
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	links { "Core", "SDL" }
