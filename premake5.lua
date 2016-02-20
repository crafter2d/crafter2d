-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2015, Jeroen Broekhuizen

newoption 
{
	trigger     = "libdir",
	value       = "path",
	description = "Path to the external libraries"
}

newoption
{
	trigger     = "travis",
	description = "Generates make files for Travis-testing"
}

if not _OPTIONS["libdir"] then
	_OPTIONS["libdir"] = path.join(path.join(os.getcwd(), "../externallibs"), _ACTION)
end

libdir = _OPTIONS["libdir"]

solution "Crafter2D"
	configurations { "Debug", "Release" }
	location "build"
	startproject "Game"
	floatingpoint "Fast"
	vectorextensions "SSE2"
	
package.path = package.path .. ";./projects/?.lua"
 
require "proj-core"
require "proj-engine"
require "proj-modyas"
require "proj-modc2d"
require "proj-modogl"
require "proj-modui"
require "proj-unittest"
require "proj-game"
require "proj-compiler"
require "proj-yasc"

if not _OPTIONS["travis"] then
	require "proj-gen"
	require "proj-modfmod"
	require "proj-modd3d"
end

