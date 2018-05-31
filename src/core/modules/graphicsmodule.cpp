
#include "graphicsmodule.h"

#include "core/graphics/device.h"
#include "core/defines.h"

namespace c2d
{
   GraphicsModule::GraphicsModule() :
      Module(eGraphicsModule, UUID_GraphicsModule),
      mpDevice(nullptr)
   {
   }

   GraphicsModule::~GraphicsModule()
   {
      delete mpDevice;
      mpDevice = nullptr;
   }

   //  -Get/set

   Graphics::Device& GraphicsModule::getDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }

   void GraphicsModule::setDevice(Graphics::Device* pdevice)
   {
      mpDevice = pdevice;
   }
}
