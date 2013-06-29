
#include "d3ddevicefactory.h"

#include "d3ddevice.h"

namespace Graphics
{

Device* D3DDeviceFactory::createDevice()
{
   return new D3DDevice();
}

} // namespace Graphics
