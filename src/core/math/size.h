
#ifndef SIZE_H
#define SIZE_H

#include "core/core_base.h"

struct CORE_API Size
{
   Size();
   Size(const Size& that);
   Size(float width, float height);

   Size& operator=(const Size& that);
   Size operator/(float value) const;
   Size operator*(float value) const {
      return Size(width * value, height * value);
   }

   float width;
   float height;
};

#include "size.inl"

#endif // SIZE_H
