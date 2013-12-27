
#ifndef __InputSystem_h__
#define __InputSystem_h__

#include "core/system/system.h"
#include "core/core_base.h"

namespace Input
{
   class InputDevice;
}

namespace c2d
{
   class CORE_API InputSystem : public System
   {
   public:
      InputSystem(Input::InputDevice* pdevice);

    // get/set
      Input::InputDevice& getDevice();

   private:

    // data
      Input::InputDevice* mpDevice;
   };
}

#endif // __InputSystem_h__
