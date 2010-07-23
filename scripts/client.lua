-- File: client.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- In this file you can add functionality for the client.

include('editor.lua')
include('keymap.lua')

-- Start up the client
-- host	: true is this client is host, false otherwise
function Client_startup(host)
	console:print("Client: starting up client")
	client:create()

	-- determine the address to connect to
	local address = ""
	if not host then
		address = ip
		client.isHost = false
	else
		address = "localhost"
		client.isHost = true
	end

	-- try to connect to the server
	if not client:connect(address, port, name) then
		MsgBox("Could not connect to server.", "", "")
	else
		-- set up the client
		Client_setup()

		console:print("Client: ready")
	end
end

-- Sets up the client
function Client_setup()
	client:setActionMap(clientMap)
	client:setKeyMap(keymap)

	-- set the client states
	client.id = -1
	client.playerNames = {}
end

-- Disconnect and shutdown the client
function Client_shutdown()
	console:print("Client: shutdown...")
	client:disconnect()
	client:destroy()
	console:print("Client: shutdown complete.")
end

-- When the client is connected to the server, this script is called
function Client_onConnected()
end

-- We were not allowed to connect to the game
-- reason	: numeric value with reason
function Client_onConnectionDenite(reason)
	Client_shutdown()

	local msg = "Game has already started."
	if reason == -2 then
		msg = "The game has already eight players."
	end
	MsgBox(msg, "", "")
end

-- Called if a new player joined the game
-- id		: the id of the player who joined
-- name	: the name of the player
function Client_onJoined(id, name)
	client.playerNames[id] = name
end

-- A player disconnected from the game
-- id		: the id of the leaving player
function Client_onPlayerLeft(id)
	client.playerNames[id] = ""
end

-- Called when a new world has been loaded
function Client_onWorldChanged()
end

-- Called whenever the server is going down
function Client_onServerDown()
	if not client.isHost then
		-- display message, and leave the game
		MsgBox("The server has disconnected. You will be returned to the menu.", "Client_onShutdown()", "Client_onShutdown()")
	else
		console:print("Client: server connection closed gracefully")
		Client_onShutdown()
	end
end

-- Called whenever the client must shutdown
function Client_onShutdown()
	console:print("Client - shutdown")
	Client_shutdown()

	MM_show(true)
end

-- Called by JEngine when a user clicks on the field
-- object 	: selected object or nil if no object was selected
-- pos 	: is the position of the cursor
function Client_onPick(object, pos)
	if object ~= nil then
		-- an object is picked
	else
		-- nothing was picked
	end
end

-- The client received an event from the server
-- stream	: contains the rest of the event data
function Client_onEvent(stream)
	type = stream:readInt()

	if type == PLAYER_CONTROLER_EVENT then
		local graph = client:getSceneGraph()
		local world = graph:getWorld()

		-- received the clients controler name
		local controler = stream:readInt()
		local object    = tolua.cast(graph:find(controler), "Object")
		graph:setControler(object)

		world:setScrollMode(0)
        world:setObjectLayer(0)
		world:setFollowObject(object)
		world:setFollowBorders(150, 650, 100, 500)
	end
end
