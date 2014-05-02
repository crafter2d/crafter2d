
#include "core/defines.h"

namespace ui
{
   template <class T>
   Property<T>::Property() :
      mValue()
   {
   }

   template <class T>
   Property<T>::Property(T& value) :
      mValue(value)
   {
   }
}
