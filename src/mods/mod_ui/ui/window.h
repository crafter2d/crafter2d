
#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

#include "core/math/rect.h"
#include "core/defines.h"

#include "property.h"

namespace ui
{
   class Event;
   class Graphics2D;
   class WindowUI;

   enum WindowState
   {
      eActive,
      eVisible,
      eDirty,
   };

   class Window
   {
   public:
      Window(Window* pparent = NULL);

    // get/set
      bool     hasParent() const;
      Window&  getParent();

      const c2d::Rect& getBounds() const;

    // operations
      virtual bool create();
      virtual void draw(Graphics2D& graphics);

      void show();

    // parent-child
      void addChild(Window* pchild);
      void removeChild(Window& child);

    // notifications
      void onLookAndFeelChanged();

   private:
      typedef std::vector<Window*> Children;
      typedef std::vector<Event*> Events;

    // data
      WindowUI*   mpUI;
      Window*     mpParent;
      Children    mChildren;
      Events      mEvents;
      c2d::Rect   mBounds;
      int         mState;
   };
}

#endif // WINDOW_H
