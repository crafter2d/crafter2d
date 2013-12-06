
#include "vertexlayout.h"

#include "core/log/log.h"
#include "core/defines.h"

#include "vertexlayoutelement.h"

namespace Graphics
{

   VertexLayout::VertexLayout():
      mElements(),
      mStride(0)
   {
   }

   VertexLayout::~VertexLayout()
   {
      clear();
   }
   
   // - Statics

   const VertexLayoutElement& VertexLayout::operator[](int index) const
   {
      return *mElements[index];
   }

   // - Get/set

   int VertexLayout::getSize() const
   {
      return mElements.size();
   }

   int VertexLayout::getStride() const
   {
      return mStride;
   }

   // - Operations

   void VertexLayout::add(VertexLayoutElement* pelement)
   {
      mElements.push_back(pelement);

      mStride += sizeof(float)* pelement->size;
   }

   void VertexLayout::clear()
   {
      for ( std::size_t index = 0; index < mElements.size(); ++index )
      {
         VertexLayoutElement* pelement = mElements[index];
         delete pelement;
      }
      mElements.clear();
   }

} // namespace Graphics

