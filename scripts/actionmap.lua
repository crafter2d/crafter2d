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
	--obj:setVelocity(vel)

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
	--obj:setVelocity(vel)

	if not obj:direction() then
		obj:flip()
	end
end

function goJump(obj, pressed)
	local vel = obj:getVelocity()
 	if pressed == 1 then
		vel.y = -25
		--obj:setVelocity(vel)
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
	if val == 1 then
		MsgBox("Are you sure you want to quit the game?", "gotoMainMenu()", "")
	end
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
