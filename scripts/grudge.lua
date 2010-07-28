-- Grudge class

GRUDGE_STATE_PATROL = 1
GRUDGE_STATE_ATTACK = 2
GRUDGE_STATE_DEAD   = 3

Grudge = {
};

function Grudge:new(o)
    o = o or {} 
    setmetatable(o, self)
    self.__index = self
	self.state = GRUDGE_STATE_PATROL
	self.controler = nil
	self.world = nil
    return o
end

function Grudge:create(world, pos)
	self.input = InputForceGenerator:new()
	self.world = world

	-- create the creature
	self.controler = Creature:new()
	self.controler:setPosition(pos)
	self.controler:create("grudge.xml")
	self.controler.grudge   = self
	self.controler.onground = 0
	self.controler.type     = OBJ_GRUDGE
		
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
		self:handlePatrol(delta)
	elseif self.state == GRUDGE_STATE_ATTACK then
		self:handleAttack(delta)
	end
end

function Grudge:handlePatrol(delta)
	-- see if we can attack someone
end

function Grudge:handleAttack(delta)
end

function Grudge:handleCollision(bound, side, on)
	if on and side == COL_SIDE_BOTTOM then
	
		self.input:setVelocity(Vector:new(-10, 0))
		
	elseif on and side > 1 then
	
		local vel = self.input:getVelocity()
		
		if side == COL_SIDE_LEFT then
			if self.controler:direction() then
				self.controler:flip()
			end
			vel.x = 10
		elseif side == COL_SIDE_RIGHT then
			if not self.controler:direction() then
				self.controler:flip()
			end
			vel.x = -10
		end
		
		self.input:setVelocity(vel)
	end
end

function Grudge_updateCallback(object, delta)
	object.grudge:update(delta)
end
