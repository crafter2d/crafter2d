
#include "core/defines.h"

namespace c2d
{
   INLINE float Rect::width() const
   {
      return right - left;
   }

   INLINE float Rect::height() const
   {
      return bottom - top;
   }
}
