
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "core/math/vertex.h"
#include "core/math/vertex4.h"

struct UIVertex
{
   Vertex pos;
   Vertex tex;
   Vertex4 color;
};

namespace Graphics
{
   template <class T> class GeometryBuffer;
}

namespace c2d
{
   class Rect;
}

namespace ui
{
   class Graphics2D
   {
   public:
      Graphics2D();

    // get/set
      void setGeometryBuffer(Graphics::GeometryBuffer<UIVertex>& buffer);

    // drawing
      void fillRectangle(const c2d::Rect& rect);

   private:

    // data
      Graphics::GeometryBuffer<UIVertex>* mpBuffer;
   };
}

#endif // GRAPHICS_H
