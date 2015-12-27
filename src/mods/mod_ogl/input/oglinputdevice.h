
#ifndef OPENGL_INPUT_DEVICE_H
#define OPENGL_INPUT_DEVICE_H

#include <cstdint>

#include "core/input/input.h"

class OGLInputDevice : public Input::InputDevice
{
public:
   OGLInputDevice();

   virtual bool create(GameWindow& window) override;
   virtual void update() override;

   virtual bool isKeyDown(int key) override;
   virtual bool isMouseButtonDown(MouseButton button) override;

private:

 // data
   uint8_t* mpKeyState;
};

#endif // OPENGL_INPUT_DEVICE_H
