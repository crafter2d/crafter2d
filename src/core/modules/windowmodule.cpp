
#include "windowmodule.h"

namespace c2d
{
   WindowModule::WindowModule(GameWindow* pwindow) :
      Module(eWindowModule, UUID_WindowModule),
      mpWindow(pwindow)
   {
   }
}
