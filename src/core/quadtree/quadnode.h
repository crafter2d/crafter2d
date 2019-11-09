#ifndef QUAD_NODE_H
#define QUAD_NODE_H

#include <memory>
#include <vector>

#include "core/core_base.h"
#include "core/math/rect.h"
#include "core/math/size.h"

namespace c2d
{
   class Sprite;

   class CORE_API QuadNode
   {
   public:
      QuadNode() : mBounds() {}
      explicit QuadNode(const Size& dimensions) : mBounds(0, 0, dimensions.width, dimensions.height) {}
      explicit QuadNode(const RectF& bounds) : mBounds(bounds) {}

      bool empty() const {
         return mSprites.empty();
      }

      int count() const;

      std::vector<const Sprite*> get(const RectF& bounds) const;
      void insert(const Sprite& sprite);

   private:

      enum class Quadrant : uint8_t { eTopLeft, eTopRight, eBottomRight, eBottomLeft };
      using Sprites = std::vector<const Sprite*>;
      using Nodes = std::vector<QuadNode>;

      void retrieve(const RectF& bounds, std::vector<const Sprite*>& sprites) const;
      void split();

      Quadrant findQuadrant(const Sprite& sprite) const;

    // data
      RectF mBounds;
      Sprites mSprites;
      Nodes mChildren;
   };
}

#endif // QUAD_NODE_H 
