
#ifndef OGLDEVICE_FACTORY_H
#define OGLDEVICE_FACTORY_H

#include "core/graphics/devicefactory.h"

namespace Graphics
{

class OGLDeviceFactory : public DeviceFactory
{
public:
   virtual Device* createDevice();
};

};

#endif // OGLDEVICE_FACTORY_H
