-- File: gamewnd.lua
--
-- Copyright (c) 2005, Jeroen Broekhuizen

include("keymap.lua")

-- Function GuiGameWnd_onActivate
-- This function is called when ever it is pushed or popped from
-- the active window list on the canvas.
function GuiGameWnd_onActivate(active)
	if active == 1 then
		gamewnd:pushMap(keymap)
	else
		gamewnd:popMap()
	end
end

local rect = canvas:getWindowRect()

-- create the main game window
gamewnd = GuiGameWnd:new()
gamewnd:create(1, rect, "Main view")
gamewnd:setVisible(true)

-- show it by default
canvas:pushWindow(gamewnd)
