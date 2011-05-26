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

libdir = "d:/Development/jengine sse/externallibs/" .. _ACTION .. "/"
	
require "projects/proj-core"
require "projects/proj-script"
require "projects/proj-engine"
require "projects/proj-gui"
require "projects/proj-game"
