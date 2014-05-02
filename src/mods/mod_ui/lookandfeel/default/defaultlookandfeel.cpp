
#include "defaultlookandfeel.h"

#include "ui/window.h"

namespace ui
{
   DefaultLookAndFeel::DefaultLookAndFeel() :
      LookAndFeel()
   {
   }

   // - Overrides

   WindowUI* DefaultLookAndFeel::createUI(Window& window)
   {
      return NULL;
   }
}
