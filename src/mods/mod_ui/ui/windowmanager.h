
#ifndef __WindowManager_h__
#define __WindowManager_h__

#include <vector>

namespace ui
{
   class Window;

   class WindowManager
   {
   public:

    // registration
      void registerWindow(Window* pwindow);

    // notifications
      void onLookAndFeelChanged();

   private:
      typedef std::vector<Window*> Windows;

    // data
      Windows mWindows;
   };
}

#endif // __WindowManager_h__
