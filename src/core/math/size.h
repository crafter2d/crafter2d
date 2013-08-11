
#ifndef SIZE_H
#define SIZE_H

struct Size
{
   Size();
   Size(const Size& that);
   Size(float width, float height);

   Size& operator=(const Size& that);
   Size operator/(float value);

   float width;
   float height;
};

#include "size.inl"

#endif // SIZE_H
