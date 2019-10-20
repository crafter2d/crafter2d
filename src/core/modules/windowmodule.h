#ifndef WINDOW_MODULE_H
#define WINDOW_MODULE_H

#include "module.h"

#include "core/defines.h"

class GameWindow;

namespace c2d
{
   class CORE_API WindowModule : public Module
   {
   public:
      explicit WindowModule(GameWindow* pwindow);

    // get/set
      GameWindow& Window() { 
         ASSERT(mpWindow);
         return *mpWindow;
      }

   private:

    // data
      GameWindow* mpWindow;
   };
}

#endif // WINDOW_MODULE_H
