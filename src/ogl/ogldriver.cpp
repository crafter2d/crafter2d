
#include "ogldriver.h"

#include "core/defines.h"

#include "ogldevice.h"
#include "input/oglinputdevice.h"

Graphics::Device* OGLDriver::createGraphicsDevice(ContentManager& manager)
{
   return new Graphics::OGLDevice(manager);
}

Input::InputDevice* OGLDriver::createInputDevice()
{
   return new OGLInputDevice();
}
