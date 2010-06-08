-- File: server.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- Server scripts

include('actionmap.lua')

-- initialize the server connection
function Server_startup()
	console:print("Server: starting up game.")

	server:create()

	-- bind the server to the well-known port
	local conn = server:getConnection()
	if not conn:create(port) then
		return false
	end

	-- initialize the randomizer
	math.randomseed(os.time())

	-- set up server side variables
	server:setActionMap(actionmap)
	server.players = {}
	server.maxPlayers = 1
	server.gameStarted = false
	server.stream = BitStream:new()

	console:print("Server: ready")

	return true
end

-- when the server should shutdown, call this function
function Server_shutdown()
	-- disconnect the connection
	console:print("Server shutdown...")
	server:destroy()
	console:print("Server shutdown complete.")
end

-- Called by the JEngine SSE framework during shutdown
function Server_onShutdown()
	Server_shutdown()
end

-- Before a client is allowed to join this function is called.
-- Return:
--    0 to allow the connection
--   -1 the game has already started
--   -2 the game is full
function Server_onClientConnecting()
	if server.gameStarted then
		return -1
	elseif table.getn(server.players) >= server.maxPlayers then
		return -2
	else
		return 0
	end
end

-- Called whenever a new client is connected to the server
function Server_onClientConnect(player)
	-- set up the player
	-- add the player to the server list
	table.insert(server.players, player)

	-- print message to the console
	console:print("Server: " .. player.name .. " has joined the game.")

	-- start the game, as this is a single player demo only
	Server_startGame("map1", player)
end

-- A client has disconnected, remove him from the game
function Server_onClientDisconnect(client, player)
	local playernum = Server_getPlayerId(player)
	if playernum ~= -1 then
		table.remove(server.players, playernum)
		console:print (player.name .. " left the game.")
	end
end

-- Returns the id of a player
function Server_getPlayerId(player)
	for i,splayer in server.players do
		if splayer.client == player.client then
			return i
		end
	end

	return -1
end

-- Returns the controler at position (x,y), or nil if there is no controler there
function Server_getControlerAt(x,y)
	for i,player in server.players do
		local controler = player.controler
		local pos = controler:getPosition()

		if pos.x == x and pos.y == y then
			return controler
		end
	end

	return nil
end

-- Called when a scriptEvent is received from a client
function Server_onEvent(player, stream)
	eventType = stream:readInt()

	-- handle the event
end

-- This is an example script which can be used during start up of the game
function Server_startGame(worldFile, player)
	server.gameStarted = true

	world = World:new()
	world:setName("world")
	if not world:create(worldFile) then
		return
	end

	-- insert the world in the scenegraph
	local graph = server:getSceneGraph()
	graph:setWorld(world)

	-- create the controler for the player
	local controler = Creature:new()
	if not controler:create("char.xml") then
		console:print("Failed to load character.")
		return
	end
	
	world:add(controler)
	player.controler = controler
	
	controler:setName(player.name)
	controler:setPosition(Vector:new(400, 300))
	controler:setAnimation(1)

	local gravity = GravityForceGenerator:new(Vector:new(0, 9.81));
	local body = tolua.cast(controler:getBody(), "PhysicsBody");
	body:addForceGenerator(gravity);
	
	controler.input = InputForceGenerator:new();
	body:addForceGenerator(controler.input);
	
	-- notify the client
	local stream = server.stream
	stream:clear()
	stream:writeInt(PLAYER_CONTROLER_EVENT)
	stream:writeString(controler:getName())
	server:sendScriptEvent(stream, player.client)
end
