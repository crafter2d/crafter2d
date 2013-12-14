
#ifndef OPENGL_DRIVER_H
#define OPENGL_DRIVER_H

#include "core/system/driver.h"

class OGLDriver : public Driver
{
public:

 // overrides
   virtual Graphics::Device* createGraphicsDevice(ContentManager& manager) override;
   virtual Input::InputDevice* createInputDevice() override;
};

#endif // OPENGL_DRIVER_H
