
#include "graphics2d.h"

#include "core/graphics/geometrybuffer.h"
#include "core/math/rect.h"
#include "core/defines.h"

namespace ui
{
   Graphics2D::Graphics2D() :
      mpBuffer(NULL)
   {
   }

   // - Get/set

   void Graphics2D::setGeometryBuffer(Graphics::GeometryBuffer<UIVertex>& buffer)
   {
      mpBuffer = &buffer;
   }

   // - Drawing

   void Graphics2D::fillRectangle(const c2d::Rect& rect)
   {
      ASSERT_PTR(mpBuffer);

      UIVertex verts[4];
      verts[0].pos.set(rect.left, rect.top);
      verts[0].tex.set(0, 0);
      verts[0].color.set(1, 0, 0, 0);

      verts[1].pos.set(rect.right, rect.top);
      verts[1].tex.set(1, 0);
      verts[1].color.set(1, 0, 0, 0);

      verts[2].pos.set(rect.right, rect.bottom);
      verts[2].tex.set(1, 1);
      verts[2].color.set(1, 0, 0, 0);

      verts[3].pos.set(rect.left, rect.bottom);
      verts[3].tex.set(0, 1);
      verts[3].color.set(1, 0, 0, 0);

      mpBuffer->addVertices(verts, 4);
   }
}
