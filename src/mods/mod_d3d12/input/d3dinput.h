#pragma once

#include "core/input/input.h"
#include "core/input/key.h"
#include "core/input/keyeventdispatcher.h"
#include "core/input/keyevent.h"

namespace Input
{
   class D3DInput : public InputDevice, public KeyEventDispatcher
   {
   public:
      D3DInput();

      // overrides
      virtual bool create(GameWindow& window) override;
      virtual void update() override;

      virtual bool isKeyDown(int key) override;

      virtual void dispatch(const KeyEvent& event) override
      {
         mPressed[event.getKey()] = (event.getEventType() == KeyEvent::ePressed);
      }

   private:
      
    // data
      bool mPressed[Key::MAX_KEY];
   };
}
