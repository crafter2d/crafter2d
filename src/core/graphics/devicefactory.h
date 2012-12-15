
#ifndef DEVICE_FACTORY_H
#define DEVICE_FACTORY_H

namespace Graphics
{
   class Device;

   class DeviceFactory
   {
   public:
      virtual Device* createDevice() = 0;
   };
};

#endif // DEVICE_FACTORY_H
