-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_d3d"
	kind "SharedLib"
	location "../build/mods/mod_d3d"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/mods/mod_d3d/**.cpp", "../src/mods/mod_d3d/**.h", "../src/mods/mod_d3d/**.inl" }
	includedirs { "../src" }
	
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		links { "Core", "d2d1", "dwrite" }
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		links { "Core", "d2d1", "dwrite" }

	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		libdirs { }

	-- Linux is actually not supported for DirectX
	
	filter "system:Linux"
		buildoptions { "-W", "-Wall", "-O0" }   
		defines { "LINUX" }
		includedirs {  }
		links {  }
		
	filter "action:cb-gcc"
		buildoptions { "-W", "-Wall", "-O0" }
		linkoptions { "--allow-multiple-definition", "-Xlinker", "-zmuldefs" }
		links {}
