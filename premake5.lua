-- Crafter 2D PreMake 5 configuration file
-- Copyright 2010-2019, Jeroen Broekhuizen

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
	_OPTIONS["libdir"] = path.join(os.getcwd(), "../externallibs")
end

libdir = path.join(_OPTIONS["libdir"], _ACTION)
pkgconf.setPrefix(libdir)

function setDefaultProjectSettings()
	cppdialect "C++17"
	flags { "NoPCH" }
	language "C++"
	systemversion "10.0.17763.0"
	targetdir "../bin"
end

solution "Crafter2D"
	configurations { "Debug", "Release" }
	location "build"
	startproject "Game"
	floatingpoint "Fast"
	vectorextensions "SSE2"

require "proj-core"
require "proj-engine"
require "proj-unittest"
require "proj-game"
require "proj-xml"

group "Libraries"
	require "proj-libcil"

group "Mods"
	require "proj-modyas"
	require "proj-modc2d"
	require "proj-modogl"
	require "proj-modui"
if not _OPTIONS["travis"] then
	require "proj-modfmod"
	require "proj-modd3d"
end

group "Tools"
	require "proj-compiler"
	require "proj-yas"
	require "proj-yasc"
if not _OPTIONS["travis"] then
	require "proj-gen"
end

