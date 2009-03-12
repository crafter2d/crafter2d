-- File: main.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- Main entry script for the game

PLAYER_CONTROLER_EVENT = 1

include('config.lua')
include('console.lua')
include('gamewnd.lua')
include('msgbox.lua')
include('server.lua')
include('client.lua')
include('splash.lua')
include('mainmenu.lua')

function showMenu()
	canvas:setCaption("JEngine SSE Framework")

	Splash_hide()

	MM_show(true)
end

MM_create()
Splash_create()

splashId = schedule("showMenu", 2000)
