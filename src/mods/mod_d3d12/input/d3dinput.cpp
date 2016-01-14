
#include "d3dinput.h"

#include "core/window/gamewindow.h"

namespace Input
{
   D3DInput::D3DInput():
      mPressed()
   {
   }

   // overrides
   bool D3DInput::create(GameWindow& window)
   {
      window.setKeyEventDispatcher(*this);
      return true;
   }

   void D3DInput::update()
   {
   }

   bool D3DInput::isKeyDown(int key)
   {
      return mPressed[key];
   }
}
