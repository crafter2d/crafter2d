-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "mod_ui"
	kind "SharedLib"
	location "../build/mods/mod_ui"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../src/mods/mod_ui/**.cpp", "../src/mods/mod_ui/**.h", "../src/mods/mod_ui/**.inl" }
	includedirs { "../src" }
	links { "Core", "Xml" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		targetsuffix "d"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	-- Set system specific settings
	filter "system:Windows"
		defines { "WIN32", "MOD_EXPORTS" }
		links { "d3dcompiler.lib" }

	filter "system:linux"
		defines { "LINUX" }
		buildoptions { "-W", "-Wall", "-O0" }
		
