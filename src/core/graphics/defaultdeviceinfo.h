
#ifndef DEFAULT_DEVICE_INFO_H
#define DEFAULT_DEVICE_INFO_H

#include "deviceinfo.h"

namespace Graphics
{
   class DefaultDeviceInfo : public IDeviceInfo
   {
   public:

      virtual bool needsCustomDevice() const override;
      virtual bool buildCustomDevice(Device& device) override;
   };
}

#endif // DEFAULT_DEVICE_INFO_H
