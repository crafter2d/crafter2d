-- Copyright (c) 2005, Jeroen Broekhuizen
--
-- Client & server side action mapping

function goLeft(obj, pressed)
	local vel = obj:getVelocity()
	if pressed == 1 then
		obj:setAnimation(2)
		vel.x = -20
	else
		obj:setAnimation(1)
		vel.x = 0
	end
	
	obj.input:setVelocity(vel)

	if obj:direction() then
		obj:flip()
	end
end

function goRight(obj, pressed)
	local vel = obj:getVelocity()
	if pressed == 1 then
		obj:setAnimation(2)
		vel.x = 20
	else
		obj:setAnimation(1)
		vel.x = 0
	end
	
	obj.input:setVelocity(vel);

	if not obj:direction() then
		obj:flip()
	end
end

function goJump(obj, pressed)
	local vel = obj:getVelocity()
 	if pressed == 1 and obj.onground > 0 then
		vel.y = -25
		obj.input:setImpulse(vel);
	end
end

function gotoMainMenu()
	if client.isHost then
		Server_onShutdown()
	else
		Client_onShutdown()
	end
end

-- shut the application down
function quitGame(val)
	local dialog = guimanager:loadFromXML("ingamemenu")
	dialog:doModal()
end

-- create new action map and bind
actionmap = ActionMap:new()
actionmap:bind(moveLeft, "goLeft")
actionmap:bind(moveRight, "goRight")
actionmap:bind(jump, "goJump")

-- create action map for client-side actions
clientMap = ActionMap:new()
clientMap:bind(swapConsole, "displayConsole")
clientMap:bind(quit, "quitGame")
