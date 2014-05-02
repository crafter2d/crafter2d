
#ifndef __DefaultLookAndFeel_h__
#define __DefaultLookAndFeel_h__

#include "lookandfeel/lookandfeel.h"

namespace ui
{
   class DefaultLookAndFeel : public LookAndFeel
   {
   public:
      DefaultLookAndFeel();

      virtual WindowUI* createUI(Window& window) override;
   };
}

#endif // __DefaultLookAndFeel_h__
