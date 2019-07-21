-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

-- create the project
project "Yas"
	kind "ConsoleApp"
	debugdir "../bin"
	location "../build/yas"
	
	setDefaultProjectSettings()
	
	-- set project files
	files { "../tools/yas/**.cpp", "../tools/yas/**.c", "../tools/yas/**.h", "../tools/yas/**.inl" }
	includedirs { "../src", "../tools" }
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		targetsuffix "d"
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    -- Systems
	filter "system:Windows"
		defines { "WIN32" }
    	links { "Core" }
	
	filter "system:Linux"
		defines { "LINUX" }
		buildoptions { "-std=c++0x", "-W", "-Wall", "-O0" }
        libdirs { "../bin" }
    
    filter { "system:linux", "Debug" }
        linkoptions { "-lCored" }

    filter { "system:linux", "Release" }
        linkoptions { "-lCore" }

