
#include "ogldevicefactory.h"

#include "ogldevice.h"

namespace Graphics
{

Device* OGLDeviceFactory::createDevice()
{
   return new OGLDevice();
}

};
