
#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include "core/core_base.h"

namespace Graphics
{
   class Device;

   class CORE_API IDeviceInfo
   {
   public:

      virtual bool needsCustomDevice() const = 0;
      virtual bool buildCustomDevice(Device& device) = 0;
   };
}

#endif // DEVICE_INFO_H
