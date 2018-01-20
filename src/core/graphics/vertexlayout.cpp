
#include "vertexlayout.h"

#include "core/log/log.h"
#include "core/defines.h"

#include "vertexlayoutelement.h"

namespace Graphics
{

   VertexLayout::VertexLayout(size_t size):
      mElements(size),
      mStride(0)
   {
   }

   VertexLayout::~VertexLayout()
   {
      clear();
   }
   
   // - Statics

   VertexLayoutElement& VertexLayout::operator[](int index)
   {
      return mElements[index];
   }

   const VertexLayoutElement& VertexLayout::operator[](int index) const
   {
      return mElements[index];
   }

   // - Operations

   void VertexLayout::emplace_back(const String& semantic, uint32_t pos, uint32_t type)
   {
      mElements.emplace_back(semantic, pos, type);
   }

   void VertexLayout::clear()
   {
      mElements.clear();
   }

} // namespace Graphics

