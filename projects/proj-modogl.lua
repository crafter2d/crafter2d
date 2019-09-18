-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_ogl"
	kind "SharedLib"
	location "../build/mods/mod_ogl"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/mods/mod_ogl/**.cpp", "../src/mods/mod_ogl/**.h", "../src/mods/mod_ogl/**.inl", "../src/mods/mod_ogl/**.c" }
	includedirs { "../src", "../src/mods", "../src/mods/mod_ogl" }
	links { "SDL" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
			
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	-- set the system specific settings
	filter "system:Windows"
		defines { "WIN32", "OGL_EXPORTS" }
        links { "Core", "opengl32", "glu32", "freetype" }
	
		includedirs {
			path.join(libdir, "sdl/include"),
			path.join(libdir, "freetype2/include"),
		}

		libdirs {
			path.join(libdir, "sdl/lib"),
			path.join(libdir, "freetype2/lib"),
		}
		
	filter "system:Linux"
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
		defines { "LINUX" }
		includedirs { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
        libdirs { "../bin" }
		links { "freetype" }

    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }
