-- Crafter 2D PreMake 4 configuration file
-- Copyright 2010-2013, Jeroen Broekhuizen

solution "Crafter2D"
	configurations { "Debug", "Release" }
	location "build"
	startproject "Game"

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
require "proj-engine"
require "proj-game"
require "proj-gen"
require "proj-unittest"
require "proj-compiler"
require "proj-modyas"
require "proj-modc2d"
require "proj-modd3d"
require "proj-modogl"
require "proj-yasc"
