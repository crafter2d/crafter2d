-- JEngine SSE PreMake configuration file
-- Copyright 2008, Jeroen Broekhuizen

-- create the project
package = newpackage()
package.path = "build"
package.name = "jengine"
package.language = "c++"
package.workdir = "../bin"

-- set all files
if ( OS == "windows" ) then
   exclude_files = matchfiles("../src/vfs/linux*", "../src/system/linux*")

   package.kind = "winexe"
elseif ( OS == "linux" ) then
   exclude_files = matchfiles("../src/vfs/win*", "../src/system/win*")

   package.kind = "exe"
end

package.files = { matchrecursive("../src/*.cpp", "../src/*.h", "../src/*.inl") }
package.excludes = exclude_files

-- make an option
if ( OS == "windows" ) then
   addoption("libdir", "Specify the location of the libraries")
end

-- definitions
if ( OS == "windows" ) then
   defines = "WIN32"
elseif ( OS == "linux" ) then
   defines = "LINUX"
end

package.config["Debug"].defines = { defines, "_DEBUG", "TIXML_USE_STL" }
package.config["Release"].defines = { defines, "TIXML_USE_STL" }

package.buildflags = { "no-pch" }

-- set object directories
package.config["Debug"].objdir = "../build/debug"
package.config["Release"].objdir = "../build/release"

-- set the include and library
if ( OS == "windows" ) then
   if ( options["libdir"] ) then
      libdir = options[libdir]
   else
      libdir = "../../externallibs/" .. target
   end

   package.includepaths = { libdir .. "/sdl/include",
                            libdir .. "/glee/include",
                            libdir .. "/cg/include",
                            libdir .. "/openal/include",
                            libdir .. "/zlib/include",
                            libdir .. "/png/include",
                            libdir .. "/ogg/include",
                            libdir .. "/vorbis/include",
                            libdir .. "/tinyxml/include",
                            libdir .. "/lua/include", 
                            libdir .. "/tolua++/include",
                            libdir .. "/glslang/include",
                            libdir .. "/freetype2/include",
                            libdir .. "/devil/include",
                            "../src" }

   package.libpaths = { libdir .. "/sdl/lib",
                        libdir .. "/glee/lib",
                        libdir .. "/cg/lib",
                        libdir .. "/openal/lib",
                        libdir .. "/zlib/lib",
                        libdir .. "/png/lib",
                        libdir .. "/ogg/lib",
                        libdir .. "/vorbis/lib",
                        libdir .. "/tinyxml/lib",
                        libdir .. "/lua/lib", 
                        libdir .. "/tolua++/lib",
                        libdir .. "/glslang/lib",
                        libdir .. "/freetype2/lib",
                        libdir .. "/devil/lib" }

   -- set the ignoring libs for studio
   package.ignorelinks = { "LIBC.lib", "msvcrt.lib" }

   -- set the libraries to link against
   package.links = { "SDLmain", "SDL", "opengl32", "glu32", "gdi32", "user32", "vfw32", "ws2_32",
                     "OpenAL32", "ALut", "GLee", "cg", "cgGL", "lua5.1", "tolua++", "devil", "ilu"  }

   -- set IDE specific settings
   if ( target == "cb-gcc" ) then
      -- deal with the MingW stuff
      table.insert(package.links, 1, "mingw32")
	tinsert(package.links, {"vorbisfile", "tinyxml_STL", "libpng13", "minizip", "zlib1", "freetype" })

      package.buildoptions = { "-W -Wall -O0" }
      package.linkoptions  = { "--allow-multiple-definition" }
   else
      tinsert(package.config["Debug"].links, { "vorbisfile_d", "tinyxmld_STL", "zlib1d", "libpng13d", "minizip_d", "freetyped" })
      tinsert(package.config["Release"].links, { "vorbisfile", "tinyxml_STL", "zlib1", "libpng13", "minizip", "freetype" })
      if ( target == "vs2008" ) then
         tinsert(package.ignorelinks, { "libcmt.lib" })
      end
   end

   -- set the pre-build event
   package.prebuildcommands = {
      "..\\tolua\\tolua++ -o ..\\src\\tolua_editor.cpp -H ..\\src\\tolua_editor.h -n editor ..\\tolua\\editor.cpp",
      "..\\tolua\\tolua++ -o ..\\src\\tolua_general.cpp -H ..\\src\\tolua_general.h -n general ..\\tolua\\general.cpp",
      "..\\tolua\\tolua++ -o ..\\src\\tolua_gui.cpp -H ..\\src\\tolua_gui.h -n gui ..\\tolua\\gui.cpp",
      "..\\tolua\\tolua++ -o ..\\src\\tolua_network.cpp -H ..\\src\\tolua_network.h -n network ..\\tolua\\network.cpp"
      "..\\tolua\\tolua++ -o ..\\src\\tolua_physics.cpp -H ..\\src\\tolua_physics.h -n network ..\\tolua\\physics.cpp" }

elseif ( OS == "linux" ) then
   package.buildoptions = { "-W -Wall -O0" }
   if ( target == "cb-gcc" ) then
      package.linkoptions = { "-Xlinker -zmuldefs" }
   end

   package.includepaths = { "/usr/include", "/usr/include/freetype2", "/usr/local/include" }
   package.links = { "SDL", "GL", "GLU", "GLee", "Cg", "CgGL",
                     "lua", "tolua++-5.1", "openal", "alut",
                     "png", "vorbisfile", "tinyxml", "minizip", "freetype" }

   -- set the pre-build event
   package.prebuildcommands = {
      "tolua++ -o ../src/tolua_editor.cpp -H ../src/tolua_editor.h -n editor ../tolua/editor.cpp",
      "tolua++ -o ../src/tolua_general.cpp -H ../src/tolua_general.h -n general ../tolua/general.cpp",
      "tolua++ -o ../src/tolua_gui.cpp -H ../src/tolua_gui.h -n gui ../tolua/gui.cpp",
      "tolua++ -o ../src/tolua_network.cpp -H ../src/tolua_network.h -n network ../tolua/network.cpp"
      "tolua++ -o ../src/tolua_physics.cpp -H ../src/tolua_physics.h -n physics ../tolua/physics.cpp" }

end
