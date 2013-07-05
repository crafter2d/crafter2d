-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "OGL"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/ogl"
	
-- set project files
files { "src/ogl/**.cpp", "src/ogl/**.h", "src/ogl/**.inl" }
includedirs { "src" }

-- set the include and library
if ( os.is("windows") ) then
   defines { "WIN32", "OGL_EXPORTS" }
	
   includedirs {
      path.join(libdir, "glee/include"),
      path.join(libdir, "soil/include"),
	  path.join(libdir, "freetype2/include"),
      path.join(libdir, "icu/include")
   }

   libdirs {
      path.join(libdir, "glee/lib"),
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
		links { "opengl32", "glu32", "cg", "cgGL" }
		
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
	links { "vorbisfile", "tinyxml", "freetype", "Box2D", "SOIL" }

end

configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "d"
	flags { "Symbols" }
	links { "Core" }
	
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	links { "Core" }
