-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "mod_d3d"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/mods/mod_d3d"
	
-- set project files
files { "src/mods/mod_d3d/**.cpp", "src/mods/mod_d3d/**.h", "src/mods/mod_d3d/**.inl" }
includedirs { "src" }

-- set the include and library
if ( os.is("windows") ) then
	defines { "WIN32", "MOD_EXPORTS" }
	
	libdirs { }

	-- set IDE specific settings
	if ( _ACTION == "cb-gcc" ) then
	
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition" }
	  
		configuration "Debug"
			links { } 
		 
		configuration "Release"
			links {  }

	else
		links {  }
		
		configuration "Debug"
			links {  }
					
		configuration "Release"
			links {  }
   end -- win32
   
elseif ( os.is("linux") ) then
	
	buildoptions { "-W", "-Wall", "-O0" }
	if ( _ACTION == "cb-gcc" ) then
		linkoptions { "-Xlinker", "-zmuldefs" }
	end
   
	defines { "LINUX" }
	
	includedirs {  }
	links {  }

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
