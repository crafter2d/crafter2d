#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "core/core_base.h"
#include "core/math/size.h"

#include "quadnode.h"

namespace c2d
{
   class Sprite;

   class CORE_API QuadTree
   {
   public:
      explicit QuadTree(const Size& dimensions): mRoot(dimensions) {}

      bool empty() const {
         return mRoot.empty();
      }

      int count() const {
         return mRoot.count();
      }

      void insert(const Sprite& sprite) {
         mRoot.insert(sprite);
      }

      std::vector<const Sprite*> get(const RectF& bounds) const {
         return mRoot.get(bounds);
      }

   private:

      QuadNode mRoot;
   };
}

#endif // QUAD_TREE_H
