
#ifndef D3DDEVICE_FACTORY_H
#define D3DDEVICE_FACTORY_H

#include "core/graphics/devicefactory.h"

class D3DDeviceFactory : public DeviceFactory
{
public:
   virtual Device* createDevice();
};

#endif // D3DDEVICE_FACTORY_H
