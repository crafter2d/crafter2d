#pragma once

#include "core/input/input.h"
#include "core/input/key.h"

class AppInputDevice : public Input::InputDevice
{
public:
   AppInputDevice();
   virtual ~AppInputDevice();

   virtual bool create(GameWindow& window) override;
   virtual void update() override;

   void set(int key, bool pressed) {
      mPressed[key] = pressed;
   }

   virtual bool isKeyDown(int key) override;

private:

   bool mPressed[Key::MAX_KEY];
};

