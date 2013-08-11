-- JEngine SSE PreMake 4 configuration file
-- Copyright 2010, Jeroen Broekhuizen

solution "JEngine"
	configurations { "Debug", "Release" }
	location "build"

if ( os.is("windows") ) then
	defines { "WIN32" }
elseif ( os.is("linux") ) then
	defines { "LINUX" }
end

newoption 
{
	trigger     = "libdir",
	value       = "path",
	description = "Path to the external libraries"
}

if not _OPTIONS["libdir"] then
	_OPTIONS["libdir"] = path.join(path.join(os.getcwd(), "../externallibs"), _ACTION)
end

libdir = _OPTIONS["libdir"]
	
require "projects/proj-core"
require "projects/proj-d3d"
require "projects/proj-ogl"
require "projects/proj-script"
require "projects/proj-engine"
-- require "projects/proj-gui"
require "projects/proj-game"
require "projects/proj-gen"
require "projects/proj-unittest"
