-- Copyright (c) 2005, Jeroen Broekhuizen

-- toggle visibility 
function console_toggle(val)
    if val then
	if canvas:isWindowDisplayed(console) then
		console:hide()
	else
		console:show()
	end
	end
end
