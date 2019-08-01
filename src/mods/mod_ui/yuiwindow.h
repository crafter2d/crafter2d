#ifndef YUI_WINDOW_H
#define YUI_WINDOW_H

#include <vector>

#include "core/math/rect.h"

namespace c2d
{
   class YuiBatchRenderer;
   class YuiSystem;

   class YuiWindow
   {
   public:
    // construct
      explicit YuiWindow(YuiSystem& system);

      bool create();
      void render(YuiBatchRenderer& renderer);

   private:
      using List = std::vector<std::pair<Rect, int>>;

      // members
      YuiSystem&     mSystem;
      Rect           mWindowRect;
      List           mRenderList;
   };
}

#endif // YUI_WINDOW_H

