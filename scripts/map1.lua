-- World1 objects

include("grudge.lua") 

function createObjects(world)
    -- create the box
	local box = Creature:new()
	box:setPosition(Vector:new(180, 100))
	box:create("box.xml")
	box:setName("box")
	world:add(box)
	
	-- create another box
	box = Creature:new()
	box:setPosition(Vector:new(180, 60));
	box:create("box.xml")
	box:setName("box2")
	world:add(box)
	
	-- create another box
	box = Creature:new()
	box:setPosition(Vector:new(180, 20));
	box:create("box.xml")
	box:setName("box3")
	world:add(box)

	-- create another box
	box = Creature:new()
	box:setPosition(Vector:new(180, -20));
	box:create("box.xml")
	box:setName("box4")
	world:add(box)
	
	-- create bridge
	local bridge = Bridge:new()
	bridge:create(world, Vector:new(727, 422), Vector:new(943, 422))
	
	-- create grudge
	local grudge = Grudge:new()
	grudge:create(world, Vector:new(1500, 10))

end
