
#ifndef VERTEX2_H
#define VERTEX2_H

#include "core/math/vector.h"

struct Vertex
{
   float x;
   float y;

   Vertex() :
      x(0),
      y(0)
   {
   }

   Vertex(const Vertex& that) :
      x(that.x),
      y(that.y)
   {
   }

   Vertex(float _x, float _y):
      x(_x),
      y(_y)
   {
   }

   Vertex& operator=(const Vector& v) {
      x = v.x;
      y = v.y;
      return *this;
   }

   void set(float _x, float _y) {
      x = _x;
      y = _y;
   }
};

#endif // VERTEX2_H
