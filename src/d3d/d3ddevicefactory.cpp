
#include "d3ddevicefactory.h"

#include "d3ddevice.h"

Device* D3DDeviceFactory::createDevice()
{
   return new D3DDevice();
}
