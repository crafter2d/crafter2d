#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "core/core_base.h"
#include "core/math/size.h"

#include "quadnode.h"

namespace c2d
{
   template <typename T>
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

      void insert(const T& element, const RectF& bounds) {
         mRoot.insert(element, bounds);
      }

      void erase(const T& element) {
         mRoot.erase(element);
      }

      std::vector<const T*> get(const RectF& bounds) const {
         return mRoot.get(bounds);
      }

      void resize(const Size& newdimension) {
         mRoot.resize(newdimension);
      }

   private:

      QuadNode<T> mRoot;
   };
}

#endif // QUAD_TREE_H
