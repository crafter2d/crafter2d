
#ifndef __WindowUI_h__
#define __WindowUI_h__

namespace ui
{
   class Graphics2D;

   class WindowUI
   {
   public:
      WindowUI();
      
      virtual void render(Graphics2D& graphics) = 0;
   };
}

#endif // __WindowUI_h__
