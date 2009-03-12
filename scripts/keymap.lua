-- Copyright 2005, Jeroen Broekhuizen
--
-- Client side key-to-action mapping

keymap = KeyMap:new()
keymap:bind(SDLK_LEFT, moveLeft)
keymap:bind(SDLK_RIGHT, moveRight)
keymap:bind(SDLK_UP, moveUp)
keymap:bind(SDLK_DOWN, moveDown)
keymap:bind(SDLK_SPACE, jump)
keymap:bind(SDLK_BACKQUOTE, swapConsole, true)
keymap:bind(SDLK_ESCAPE, quit, true)