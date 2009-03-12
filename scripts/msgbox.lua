-- MsgBox functionality for in game and scripting purposes. Call MsgBox with
-- a message to display the box with ok/cancel buttons. The cmdOk and cmdCancel
-- are strings that respresent some Lua code that should be performed when
-- respectively the Ok or Cancel button is pressed.
--
-- Copyright (c) 2005, Jeroen Broekhuizen

-- Function MsgBox
-- Displays the message box
function MsgBox(msg, cmdOk, cmdCancel)
	local msgwnd = msgbox:getItemById(2)
	if msgwnd ~= nil then
		msgbox.cmdOk = cmdOk
		msgbox.cmdCancel = cmdCancel

		msgwnd:setCaption(msg)
		canvas:doModal(msgbox)
	end
end

function ShowMessage(msg)
	local wnd = GuiWnd:new()
	wnd:create(2, GuiRect:new(0,400,0,75))
	wnd:addChild( GuiStatic:new (3, GuiRect:new(5,395,10,40), msg) )
	wnd:center()

	canvas:doModal(wnd)
end

-- Handler function when user presses the ok button
function onMsgBoxOk()
	canvas:endModal(0)
	loadstring(msgbox.cmdOk)()
end

-- Handler function when user presses the cancel button
function onMsgBoxCancel()
	canvas:endModal(1)
	loadstring(msgbox.cmdCancel)()
end

-- creation of the dialog box
msgbox = GuiDialog:new()
msgbox:create(1, GuiRect:new(0,400,0,75), "Quit game", 392)
msgbox:addChild( GuiStatic:new(2, GuiRect:new(5,395,10,40), "Message comes here", 8) )
msgbox:center()

button = GuiButton:new()
button:create(3, GuiRect:new(140,190,50,70), "Ok", 268, msgbox)
button:setCommand("onMsgBoxOk()")

button = GuiButton:new()
button:create(4, GuiRect:new(210,260,50,70), "Cancel", 268, msgbox)
button:setCommand("onMsgBoxCancel()")
