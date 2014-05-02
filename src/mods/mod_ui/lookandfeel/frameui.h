
#ifndef __FrameUI_h__
#define __FrameUI_h__

#include "windowui.h"

namespace ui
{
   class Frame;

   class FrameUI : public WindowUI
   {
   public:
      FrameUI(Frame& frame);

      virtual void render(Graphics2D& graphics) override;

   private:

    // data
      Frame& mFrame;
   };
}

#endif // __FrameUI_h__
