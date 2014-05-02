
#ifndef __LookAndFeel_h__
#define __LookAndFeel_h__

namespace ui
{
   class Window;
   class WindowUI;

   class LookAndFeel
   {
   public:

      virtual WindowUI* createUI(Window& window) = 0;
   };
}

#endif // __LookAndFeel_h__
