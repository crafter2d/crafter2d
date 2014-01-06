
#ifndef __GraphicsSystem_h__
#define __GraphicsSystem_h__

#include "module.h"

namespace Graphics
{
   class Device;
}

namespace c2d
{
   class CORE_API GraphicsModule : public Module
   {
   public:
      GraphicsModule();
      virtual ~GraphicsModule(); 

    // query
      Graphics::Device& getDevice();
      void              setDevice(Graphics::Device* pdevice);

   private:

    // data
      Graphics::Device* mpDevice;
   };
}

#endif // __GraphicsSystem_h__
