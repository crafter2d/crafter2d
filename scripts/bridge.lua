-- File: bridge.lua
-- Copyright (c) Jeroen Broekhuizen, 2010
--
-- Bridge object

include('box2d.lua')

Bridge = {
};

function Bridge:new(o)
    o = o or {} 
    setmetatable(o, self)
    self.__index = self
    return o
end

function Bridge:create(world, p1, p2)
	local simulator = box2d_getSimulator(world)
	
	self.world = world
	
	local jointdef = Box2DRevoluteJointDefinition:new()
	jointdef.pleft = self:createSide(p1, 0)
	jointdef.anchor:set(p1.x + 6.0, p1.y)
	
	local dif = p2.x - p1.x
	local num = dif / 12	
	local x = p1.x + 12
	
	for i=1, num-1, 1 do
		local pos = Vector:new(x, p1.y)
		
		jointdef.pright = self:createPart(pos, i)
				
		local joint = simulator:createRevoluteJoint(jointdef)
		
		x = x + 12
		
		jointdef.pleft = jointdef.pright
		jointdef.anchor.x = jointdef.anchor.x + 12
	end
	
	jointdef.pright = self:createSide(p2, num+1)
	simulator:createRevoluteJoint(jointdef)
end

function Bridge:createPart(point, index)
	-- create bridge part
	local box = Creature:new()
	box:setPosition(point)
	box:create("bridgepart.xml")
	box:setName("bridgepart" .. index)
	
	self.world:add(box)
	
	return box2d_getBody(box)
end

function Bridge:createSide(point, index)
	-- create bridge part
	local box = Creature:new()
	box:setPosition(point)
	box:create("bridgeside.xml")
	box:setName("bridgepart" .. index)
	
	self.world:add(box)
	
	return box2d_getBody(box)
end
