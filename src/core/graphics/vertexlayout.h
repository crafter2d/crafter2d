
#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <vector>
#include <cstdint>

#include "core/core_base.h"

#include "vertexlayoutelement.h"

namespace Graphics
{
   class CORE_API VertexLayout
   {
   public:
      using Elements = std::vector<VertexLayoutElement>;
      using iterator = Elements::iterator;
      using const_iterator = Elements::const_iterator;

      explicit VertexLayout(size_t size = 0);
      ~VertexLayout();

            VertexLayoutElement& operator[](int index);
      const VertexLayoutElement& operator[](int index) const;

      iterator begin() {
         return mElements.begin();
      }

      iterator end() {
         return mElements.end();
      }

      const_iterator begin() const {
         return mElements.begin();
      }

      const_iterator end() const {
         return mElements.end();
      }

    // get/set
      uint32_t getSize() const {
         return mElements.size();
      }

      uint32_t getStride() const {
         return mStride;
      }

      void setStride(uint32_t stride) {
         mStride = stride;
      }

    // operations
      void emplace_back(const String& semantic, uint32_t pos, uint32_t type);
      void clear();

   private:

    // data
      Elements mElements;
      uint32_t mStride;
   };
}

#endif // VERTEX_LAYOUT_H
