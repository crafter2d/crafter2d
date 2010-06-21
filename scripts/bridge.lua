-- File: bridge.lua
-- Copyright (c) Jeroen Broekhuizen, 2010
--
-- Bridge object

Bridge = {
};

function Bridge:new(o)
    o = o or {} 
    setmetatable(o, self)
    self.__index = self
    return o
end

function Bridge:create(p1, p2)
	self:createPart(p1, 1)
	self:createPart(p2, 2)
end

function Bridge:createPart(point, index)
	-- create bridge part
	local box = Creature:new()
	box:setPosition(point)
	box:create("bridgepart.xml")
	box:setName("bridgepart" .. index)
	
	world:add(box)
end
