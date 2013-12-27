
#ifndef __GraphicsSystem_h__
#define __GraphicsSystem_h__

#include "core/system/system.h"

namespace Graphics
{
   class Device;
}

namespace c2d
{
   class CORE_API GraphicsSystem : public System
   {
   public:
      explicit GraphicsSystem(Graphics::Device* pdevice);

    // query
      Graphics::Device& getDevice();

   private:

    // data
      Graphics::Device* mpDevice;
   };
}

#endif // __GraphicsSystem_h__
