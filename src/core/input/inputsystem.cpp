
#include "inputsystem.h"

namespace c2d
{
   InputSystem::InputSystem(Input::InputDevice* pdevice):
      System(SystemKind::eInputSystem),
      mpDevice(pdevice)
   {
   }

   // - Get/set

   Input::InputDevice& InputSystem::getDevice()
   {
      return *mpDevice;
   }
}