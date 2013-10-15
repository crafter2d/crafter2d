
#include "driver.h"

#include "core/defines.h"

Driver::Driver():
   mpInfo(NULL)
{
}

// - Get/set

bool Driver::hasDeviceInfo() const
{
   return mpInfo != NULL;
}
   
Graphics::IDeviceInfo& Driver::getDeviceInfo()
{
   ASSERT_PTR(mpInfo);
   return *mpInfo;
}

void Driver::setDeviceInfo(Graphics::IDeviceInfo* pinfo)
{
   mpInfo = pinfo;
}
