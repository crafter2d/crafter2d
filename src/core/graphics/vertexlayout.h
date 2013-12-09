
#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <vector>

#include "core/core_base.h"

namespace Graphics
{
   struct VertexLayoutElement;

   class CORE_API VertexLayout
   {
   public:
      VertexLayout();
      ~VertexLayout();

            VertexLayoutElement& operator[](int index);
      const VertexLayoutElement& operator[](int index) const;

    // get/set
      int getSize() const;
      int getStride() const;

    // operations
      void add(VertexLayoutElement* pelement);
      void clear();

   private:
      typedef std::vector<VertexLayoutElement*> Elements;

    // data
      Elements mElements;
      int      mStride;
   };
}

#endif // VERTEX_LAYOUT_H
