
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

bool OGLInputDevice::isKeyDown(int key)
{
   return mpKeyState[SDLK_LEFT];
}

bool OGLInputDevice::isMouseButtonDown(MouseButton button)
{
   return false;
}
