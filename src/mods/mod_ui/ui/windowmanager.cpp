
#include "windowmanager.h"

#include "window.h"

namespace ui
{
   void WindowManager::registerWindow(Window* pwindow)
   {
      ASSERT(!pwindow->hasParent());
      Windows::const_iterator it = std::find(mWindows.begin(), mWindows.end(), pwindow);
      if ( it != mWindows.end() )
      {
         mWindows.push_back(pwindow);
      }
   }

   void WindowManager::onLookAndFeelChanged()
   {
      for ( std::size_t index = 0; index < mWindows.size(); ++index )
      {
         Window* pwindow = mWindows[index];
         pwindow->onLookAndFeelChanged();
      }
   }
}
