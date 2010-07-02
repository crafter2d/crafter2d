-- File: mainmenu.lua
-- Copyright (c) Jeroen Broekhuizen, 2006
--
-- Main menu script for Pong

-- Create the main menu
function MM_create()
	mm_wnd = guimanager:loadFromXML("mainmenu")
end

-- Show or hide the main menu
function MM_show(show)
	if show then
		client:loadWorld("intro", "intro")
		
		local world = client:getSceneGraph():getWorld()
		world:setScrollMode(0)
        world:setObjectLayer(0)
		world:setFollowBorders(150, 650, 100, 500)
		
		canvas:pushWindow(mm_wnd)
	else
		canvas:popWindow(mm_wnd)
	end
end
