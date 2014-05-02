
#include "window.h"

#include <core/defines.h>

#include "mods/mod_ui/lookandfeel/lookandfeel.h"
#include "mods/mod_ui/lookandfeel/windowui.h"

#include "system.h"

namespace ui
{
   Window::Window(Window* pparent) :
      mpUI(NULL),
      mpParent(pparent),
      mChildren(),
      mEvents(),
      mBounds(),
      mState(0)
   {
      if ( mpParent != NULL )
      {
         mpParent->addChild(this);
      }
   }

   // - Get/set

   bool Window::hasParent() const
   {
      return mpParent != NULL;
   }

   Window& Window::getParent()
   {
      ASSERT_PTR(mpParent);
      return *mpParent;
   }

   const c2d::Rect& Window::getBounds() const
   {
      return mBounds;
   }

   // - Operations

   bool Window::create()
   {
      mpUI = System::getInstance().getLookAndFeel().createUI(*this);
      return mpUI != NULL;
   }

   void Window::draw(Graphics2D& graphics)
   {
      ASSERT_PTR(mpUI);
      mpUI->render(graphics);
   }

   void Window::show()
   {
      System::getInstance();
   }

   // - Maintenance

   void Window::addChild(Window* pchild)
   {
      mChildren.push_back(pchild);
   }

   void Window::removeChild(Window& child)
   {
      Children::iterator it = std::find(mChildren.begin(), mChildren.end(), &child);
      if ( it != mChildren.end() )
      {
         mChildren.erase(it);
      }
   }

   // - Notifications

   void Window::onLookAndFeelChanged()
   {
      mpUI = System::getInstance().getLookAndFeel().createUI(*this);
      
      for ( std::size_t index = 0; index < mChildren.size(); ++index )
      {
         Window* pchild = mChildren[index];
         pchild->onLookAndFeelChanged();
      }
   }
}
