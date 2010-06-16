-- File: editor.lua
-- Copyright (c) Jeroen Broekhuizen, 2007
--
-- Editor script for the game

EDIT_MODE_TILE   = 1
EDIT_MODE_BOUND  = 2
EDIT_MODE_OBJECT = 3

-- define the Editor class
Editor = { 
	mode = EDIT_MODE_TILE, 
	control = nil, 
	bound = nil, 
	boundpoint = -1,
	contextpoint = nil 	-- GuiPoint
};

function Editor:new (o)
    o = o or {} 
    setmetatable(o, self)
    self.__index = self
    return o
end

function Editor:setMode(mode)
	self.mode = mode
end

function Editor:setControl(control)
	self.control = control
end

function Editor:setContextPosition(point)
	-- convert the point to a local vector
	local lpoint = GuiPoint:new(point.x, point.y)
	self.control:windowToClient(lpoint)
	local pos  = Vector:new(lpoint.x, lpoint.y)
	
	-- if posible convert to layer position
	if self.control:hasWorld() and self.control:world():getLayerCount() > 0 then
		self.contextpoint = self.control:world():getLayer(0):screenToLayer(pos)
	else
		self.contextpoint = pos
	end
end

function Editor:getContextPosition()
	return self.contextpoint
end

function Editor:getMode()
	return self.mode
end

function Editor:isBoundMode()
	return self.mode == EDIT_MODE_BOUND
end

function Editor:hasSelectedBound()
	return self.bound ~= nil
end

function Editor:hasSelectedPoint()
	return self.boundpoint >= 0
end

function Editor:getSelectedBound()
	return self.bound
end

function Editor:addBound(point)
	local p     = Vector:new(point.x, point.y)
	local left  = self.control:world():getLayer(0):screenToLayer(p)
	local right = Vector:new(left.x + 10, left.y)
	
	self.bound = self.control:world():addBound(left, right)
	self.boundpoint = 1
end

function Editor:selectBound(point)
	self.bound = self.control:pickBound(point)
	if self.bound ~= nil then
		console:print("picked the bound")
		local pos  = Vector:new(point.x, point.y)
		local conv = self.control:world():getLayer(0):screenToLayer(pos)
		
		self.boundpoint = self.bound:findPoint(conv)
	else
		self.boundpoint = -1
	end
end

function Editor:moveBound(offset)
    console:print("moving the bound")
	local pos = Vector:new(offset.x, offset.y)
	if self.boundpoint >= 0 then
		self.bound:movePoint(self.boundpoint, pos)
	else
		self.bound:move(pos)
	end
end

function Editor:pickObject(point)
	-- convert the point to a local vector
	local pos  = Vector:new(point.x, point.y)
	local conv = self.control:world():getLayer(0):screenToLayer(pos)
	
	local pv = PickVisitor:new(conv)
	pv:traverse(self.control:world())
	if pv:hasPickedObject() then
		return pv:getPicked()
	else
		local ev = EntityVisitor:new(conv)
		ev:traverse(self.control:world())
		return ev:getPicked()
	end
end

editor = Editor:new()
