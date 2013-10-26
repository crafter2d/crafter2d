-- Crafter 2D PreMake 4 configuration file
-- Copyright 2010-2013, Jeroen Broekhuizen

solution "Crafter2D"
	configurations { "Debug", "Release" }
	location "build"
	startproject "game"

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
	

package.path = package.path .. ";./projects/?.lua"
 
require "proj-core"
require "proj-d3d"
require "proj-ogl"
require "proj-script"
require "proj-engine"
-- require "projects/proj-gui"
require "proj-game"
require "proj-gen"
require "proj-unittest"
