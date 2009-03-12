-- File: main.lua
-- Copyright (c) Jeroen Broekhuizen, 2006
--
-- Main menu script for Pong

-- Create the main menu
function MM_create()
	mm_wnd =  GuiDialog:new()
	mm_wnd:create(1, GuiRect:new(0,200,0,65), "Main menu", 392)
	mm_wnd:center()

	mm_start = GuiButton:new()
	mm_start:create(4, GuiRect:new(10, 190, 25, 40), "Start", 268, mm_wnd)
	mm_start:setCommand("MM_startGame()")

	mm_quit = GuiButton:new()
	mm_quit:create(5, GuiRect:new(10,190, 45,60), "Quit", 268, mm_wnd)
	mm_quit:setCommand("canvas:quit()")
end

-- Show or hide the main menu
function MM_show(show)
	if show then
		canvas:pushWindow(mm_wnd)
	else
		canvas:popWindow(mm_wnd)
	end
end

-- Start the game
function MM_startGame()
	if not Server_startup() then
		return
	end

	Client_startup(true)

	gamewnd:setVisible(true);
	MM_show(false)
end
