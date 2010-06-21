-- File: box2d.lua
-- Copyright (c) Jeroen Broekhuizen, 2010
--
-- box2d lua interface

function box2d_getSimulator(world)
	return tolua.cast(world:getSimulator(), "Box2DSimulator")
end

function box2d_createRevoluteJoint(world, definition)
	local simulator = box2d_getSimulator(world)
	
	simulator:createRevoluteJoint(definition)
end
