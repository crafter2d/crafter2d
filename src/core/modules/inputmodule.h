
#ifndef __InputSystem_h__
#define __InputSystem_h__

#include "module.h"

namespace Input
{
   class InputDevice;
}

namespace c2d
{
   class CORE_API InputModule : public Module
   {
   public:
      InputModule();
      virtual ~InputModule();

    // get/set
      Input::InputDevice& getDevice();
      void                setDevice(Input::InputDevice* pdevice);

   private:

    // data
      Input::InputDevice* mpDevice;
   };
}

#endif // __InputSystem_h__
