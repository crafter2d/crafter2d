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
	
require "proj-math"
require "proj-engine"
require "proj-jengine"
