
#include "graphicssystem.h"

#include "core/defines.h"

namespace c2d
{
   GraphicsSystem::GraphicsSystem(Graphics::Device* pdevice) :
      System(SystemKind::eGraphicsSystem),
      mpDevice(pdevice)
   {
   }

   // - Get/set

   Graphics::Device& GraphicsSystem::getDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }
}
