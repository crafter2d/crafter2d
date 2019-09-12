-- Crafter 2D Premake 5 system file
-- Copyright 2010-2019, Jeroen Broekhuizen

premake.path = premake.path .. ';projects'

pkgconf = require 'pkg-config'
if pkgconf == nil then
	print("Failed to load the pkgconfig module")
end

if _TARGET_OS == 'windows' then
	local p = path.join(os.getcwd(), 'projects/pkgconfig')
	pkgconf.addPath(p)
end

