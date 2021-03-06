
#include "defaultdeviceinfo.h"

#include "core/defines.h"

namespace Graphics
{

   bool DefaultDeviceInfo::needsCustomDevice() const
   {
      return false;
   }
      
   bool DefaultDeviceInfo::buildCustomDevice(Device& device)
   {
      C2D_UNUSED(device);
      UNREACHABLE("default device info does not build custom device");
   }

} // namespace Graphics
