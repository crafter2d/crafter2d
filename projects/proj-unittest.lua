-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

-- create the project
project "UnitTest"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin"
	flags { "NoPCH" }
	location "build/unittest"
	debugdir "bin"
	
	-- set project files
	files { "src/unittest/**.cpp", "src/unittest/**.h", "src/unittest/**.inl" }
	includedirs { "src" }
	links { "Core", "Engine" }

	configuration "Debug"
		defines { "_DEBUG", "_CXXTEST_HAVE_EH" }
		targetsuffix "d"
		flags { "Symbols" }
		
	configuration "Release"
		defines { "NDEBUG", "_CXXTEST_HAVE_EH" }
		flags { "Optimize" }
		
	configuration "windows"
		includedirs { 	path.join(libdir, "cxxtest"),
						path.join(libdir, "zlib/include") }
						
		libdirs { 	path.join(libdir, "zlib/lib") }
		
		local sln = solution()
		local srcdir = path.join(sln.basedir, "src/unittest");
		local gencmd = path.join(libdir, "cxxtest/cxxtestgen.py")
		prebuildcommands { "python " .. gencmd .. " --gui=Win32Gui -o \"" .. path.join(srcdir, "runner.cpp") .. "\" \""  .. path.join(srcdir, "*.h") .. "\"" }
