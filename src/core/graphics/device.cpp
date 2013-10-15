
#include "device.h"

#include "core/defines.h"

namespace Graphics
{

Device::Device():
   mpDeviceInfo(NULL)
{
}

Device::~Device()
{
   delete mpDeviceInfo;
}

// - Get/set

bool Device::hasDeviceInfo() const
{
   return mpDeviceInfo != NULL;
}

IDeviceInfo& Device::getDeviceInfo()
{
   return *mpDeviceInfo;
}
      
void Device::setDeviceInfo(IDeviceInfo* pinfo)
{
   mpDeviceInfo = pinfo;
}

};