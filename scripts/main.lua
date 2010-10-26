-- File: main.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- Main entry script for the game

PLAYER_CONTROLER_EVENT = 1

include('config.lua')
include('console.lua')
include('msgbox.lua')
include('server.lua')
include('client.lua')
include('splash.lua')
include('mainmenu.lua')

-- called by engine initialization
function game_initialize()
end

-- called when the game is shutdown
function game_shutdown()
end

-- called each frame
function game_run()
end

function showMenu()
	canvas:setCaption("JEngine SSE Framework")

	Splash_hide()

	MM_show(true)
end

MM_create()
Splash_create()

splashId = schedule("showMenu", 2000)
