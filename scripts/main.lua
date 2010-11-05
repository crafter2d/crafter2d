-- File: main.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- Main entry script for the game

PLAYER_CONTROLER_EVENT = 1

include('scripts/config.lua')
--include('console.lua')
--include('msgbox.lua')
--include('server.lua')
--include('client.lua')
--include('splash.lua')
--include('mainmenu.lua')

-- called by engine initialization
function Game_initialize()
	server = Server:new()
	server:create()
	server:listen(port)
	
	client = Client:new()
	client:create()
	client:connect(ip, port, name)
	
	self:setActive(true)
end

-- called when the game is shutdown
function Game_shutdown()
	client:disconnect()
	client:destroy()
	
	server:shutdown()
	server:destroy()
end

-- called each frame
function Game_run(delta)
	server:update(delta)
	client:update(delta)
	
	-- game:setActive(false)
end

--function showMenu()
--	canvas:setCaption("JEngine SSE Framework")
--
--	Splash_hide()
--
--	MM_show(true)
--end

--MM_create()
--Splash_create()

--splashId = schedule("showMenu", 2000)
