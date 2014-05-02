
#include "frameui.h"

#include "ui/frame.h"
#include "ui/graphics2d.h"

namespace ui
{
   FrameUI::FrameUI(Frame& frame) :
      WindowUI(),
      mFrame(frame)
   {
   }

   void FrameUI::render(Graphics2D& graphics)
   {
      graphics.fillRectangle(mFrame.getBounds());
   }
}
