

#ifndef WIN32_DEVICE_INFO_H
#define WIN32_DEVICE_INFO_H

#include <cstdint>

#include "core/graphics/deviceinfo.h"

class Win32DeviceInfo : public Graphics::IDeviceInfo
{
public:
   Win32DeviceInfo(uint32_t wnd);

 // overrides
   virtual bool needsCustomDevice() const override;
   virtual bool buildCustomDevice(Graphics::Device& device) override;

private:
 // data
   uint32_t mWnd;
};

#endif // WIN32_DEVICE_INFO_H
