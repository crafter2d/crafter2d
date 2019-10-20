
#include "oglinputdevice.h"

#include "core/defines.h"

#include <SDL/SDL.h>

static int Keys[] =
{
   0,
   SDLK_LEFT,
   SDLK_RIGHT,
   SDLK_UP,
   SDLK_DOWN,
   SDLK_SPACE,
   SDLK_RETURN,
   SDLK_HOME,
   SDLK_END,
   SDLK_PAGEUP,
   SDLK_PAGEDOWN,
   SDLK_TAB,
   SDLK_CAPSLOCK,
   SDLK_1,
   SDLK_2,
   SDLK_3,
   SDLK_4,
   SDLK_5,
   SDLK_6,
   SDLK_7,
   SDLK_8,
   SDLK_9,
};

OGLInputDevice::OGLInputDevice():
   InputDevice(),
   mKeys(),
   mpKeyState(nullptr)
{
}

bool OGLInputDevice::create(GameWindow& window)
{
   C2D_UNUSED(window);

   for ( auto& key : Keys )
   {
      mKeys.push_back(SDL_GetScancodeFromKey(key));
   }
   return true;
}

void OGLInputDevice::update()
{
   mpKeyState = SDL_GetKeyboardState(nullptr);
}

bool OGLInputDevice::isKeyDown(int key)
{
   int scancode = mKeys[key];
   return mpKeyState[scancode];
}

bool OGLInputDevice::isMouseButtonDown(MouseButton button)
{
   C2D_UNUSED(button);
   return false;
}
