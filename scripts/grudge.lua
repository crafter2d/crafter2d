-- Grudge class

Grudge = {
};

GRUDGE_STATE_PATROL = 1
GRUDGE_STATE_ATTACK = 2
GRUDGE_STATE_DEAD   = 3

function Grudge:new(o)
    o = o or {} 
    setmetatable(o, self)
    self.__index = self
	self.state = GRUDGE_STATE_PATROL
	self.controler = nil
    return o
end

function Grudge:create(world, pos)
	self.input = InputForceGenerator:new()

	-- create the creature
	self.controler = Creature:new()
	self.controler:setPosition(pos)
	self.controler:create("grudge.xml")
	self.controler.grudge = self
	self.controler.onground = 0
		
	local modai = ModifierAI:new("Grudge_updateCallback")
	self.controler:addModifier(modai)
	
	-- add it to the world
	world:add(self.controler)
	
	-- prepare box2d
	local body = box2d_getBody(self.controler)
	body:addForceGenerator(self.input)
	body:generateSensors()
end

function Grudge:update(delta)
	-- update the grudge
	if self.state == GRUDGE_STATE_PATROL then
		-- see if we can attack someone
	end
end

function Grudge_updateCallback(object, delta)
	object.grudge:update(delta)
end
