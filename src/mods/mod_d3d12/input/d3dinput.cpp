
#include "d3dinput.h"

namespace Input
{
   D3DInput::D3DInput()
   {
   }

   // overrides
   bool D3DInput::create(GameWindow& window)
   {
      return true;
   }

   void D3DInput::update()
   {
   }

   bool D3DInput::isKeyDown(int key)
   {
      return false;
   }
}
