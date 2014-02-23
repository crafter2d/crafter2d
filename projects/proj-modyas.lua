-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

project "mod_yas"
	kind "SharedLib"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/mods/mod_yas"
	
	-- set project files
	files { "src/mods/mod_yas/**.cpp", "src/mods/mod_yas/**.c", "src/mods/mod_yas/**.h", "src/mods/mod_yas/**.inl" }
	includedirs { "src", "src/mods" }
	links { "Core" }
	
	configuration "Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		flags { "Symbols" }
		links { "antlr3cd" }
	 
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		links { "antlr3c" }

	configuration "windows"
		defines { "WIN32", "SCRIPT_EXPORTS", "_ALLOW_KEYWORD_MACROS" }
		
		includedirs { 	path.join(libdir, "antlr/include"),
						path.join(libdir, "zlib/include") }
						
		libdirs { 	path.join(libdir, "antlr/lib"),
					path.join(libdir, "zlib/lib") }
