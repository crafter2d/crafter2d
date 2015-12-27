#pragma once

#include "core/input/input.h"

namespace Input
{
   class D3DInput : public InputDevice
   {
   public:
      D3DInput();

      // overrides
      virtual bool create(GameWindow& window) override;
      virtual void update() override;

      virtual bool isKeyDown(int key) override;

   private:
      // data
   };
}
