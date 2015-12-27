
#include "inputmodule.h"

#include "core/input/input.h"
#include "core/defines.h"

namespace c2d
{
   InputModule::InputModule() :
      InputModule(NULL)
   {
   }

   InputModule::InputModule(Input::InputDevice* pdevice) :
      Module(eInputModule, UUID_InputModule),
      mpDevice(pdevice)
   {
   }

   InputModule::~InputModule()
   {
      delete mpDevice;
      mpDevice = NULL;
   }

   // - Get/set

   Input::InputDevice& InputModule::getDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }

   void InputModule::setDevice(Input::InputDevice* pdevice)
   {
      mpDevice = pdevice;
   }
}
