
#include "systemmodule.h"

#include "core/defines.h"

namespace c2d
{
   SystemModule::SystemModule() :
      Module(Module::eSystem)
   {
   }

   // - Get/set

   System& SystemModule::getSystem()
   {
      ASSERT_PTR(mpSystem);
      return *mpSystem;
   }

   void SystemModule::setSystem(System* psystem)
   {
      mpSystem = psystem;
   }

} // namespace c2d
