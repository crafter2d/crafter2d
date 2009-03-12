-- File: splash.lua
-- Copyright (c) Jeroen Broekhuizen, 2005
--
-- Script file for the games splash screen

function Splash_create()
	splashScreen = GuiScriptWnd:new()
	splashScreen:create(1, GuiRect:new(0,800,0,600), "", 264)
	splashScreen:setBackgroundImage("../images/splash.png")
	splashScreen:setKeyDownFnc("Splash_keydown")
	splashScreen:setLButtonDownFnc("Splash_click")

	canvas:pushWindow(splashScreen)
end

function Splash_hide()
	canvas:popWindow(splashScreen)
end

function Splash_end()
	unschedule(splashId)
	showMenu()
end

function Splash_click(point)
	Splash_end()
end

function Splash_keydown(key)
	Splash_end()
end
