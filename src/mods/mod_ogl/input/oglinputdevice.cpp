
#include "oglinputdevice.h"

#include <SDL/SDL.h>

OGLInputDevice::OGLInputDevice():
   InputDevice(),
   mpKeyState(NULL)
{
}

bool OGLInputDevice::create(int windowhandle)
{
   return true;
}

void OGLInputDevice::update()
{
   mpKeyState = SDL_GetKeyState(NULL);
}

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

bool OGLInputDevice::isKeyDown(int key)
{
   int sdlkey = Keys[key];
   return mpKeyState[sdlkey];
}

bool OGLInputDevice::isMouseButtonDown(MouseButton button)
{
   return false;
}
