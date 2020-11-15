#ifndef QUAD_NODE_H
#define QUAD_NODE_H

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

#include "core/core_base.h"
#include "core/math/rect.h"
#include "core/math/size.h"

namespace c2d
{
   class Sprite;

   template <typename T>
   class CORE_API QuadNode
   {
   public:
      QuadNode() : mChildren{}, mElements{}, mBounds{} {}
      explicit QuadNode(const Size& dimensions) : mChildren{}, mElements{}, mBounds{ 0, 0, dimensions.width, dimensions.height } {}
      explicit QuadNode(const RectF& bounds) : mBounds(bounds) {}

      bool empty() const {
         return mElements.empty();
      }

      int count() const {
         if ( !mChildren.empty() )
         {
            int total = 0;
            for ( auto& node : mChildren )
            {
               total += node.count();
            }
            return total;
         }
         else
         {
            return mElements.size();
         }
      }

      void resize(const Size& newdimension) {
         mBounds.set(0, 0, newdimension.width, newdimension.height);

         Elements elements;
         retrieveElements(elements);
         mChildren.clear();
         mElements.clear();

         for ( auto& element : elements )
         {
            insert(*element.pdata, element.bounds);
         }
      }

      std::vector<const T*> get(const RectF& bounds) const {
         std::vector<const T*> result;
         retrieve(bounds, result);
         return result;
      }

      void insert(const T& element, const RectF& bounds) {
         if ( !mChildren.empty() )
         {
            auto& node = findQuadrant(element, bounds);
            if ( node.fits(bounds) )
            {
               node.insert(element, bounds);
            }
            else
            {
               mElements.emplace_back(element, bounds);
            }
         }
         else if ( mElements.size() >= 3 && canSplit() )
         {
            split();
            insert(element, bounds);
         }
         else
         {
            mElements.emplace_back(element, bounds);
         }
      }

      void erase(const T& element) {
         auto it = std::find_if(mElements.begin(), mElements.end(), [&element](const Element<T>& e) {
            return e.pdata == &element;
         });
         if ( it != mElements.end() ) {
            mElements.erase(it);
         }
         
         for ( auto& node : mChildren )
         {
            node.erase(element);
         }
      }

   private:

      template <typename T>
      struct Element {
         const T* pdata;
         RectF    bounds;

         Element(const T& data, const RectF& bounds) : pdata(&data), bounds(bounds) {}
      };

      enum class Quadrant : uint8_t { eTopLeft, eTopRight, eBottomRight, eBottomLeft };
      using Elements = std::vector<Element<T>>;
      using Nodes = std::vector<QuadNode<T>>;

      bool fits(const RectF& bounds) const {
         return mBounds.contains(bounds);
      }

      void retrieve(const RectF& bounds, std::vector<const T*>& data) const {
         if ( mBounds.overlaps(bounds) )
         {
            if ( !mChildren.empty() )
            {
               for ( auto& node : mChildren ) {
                  node.retrieve(bounds, data);
               }
            }

            for ( auto& element : mElements )
            {
               data.push_back(element.pdata);
            }
         }
      }

      void retrieveElements(Elements& elements)
      {
         if ( mChildren.size() > 0 )
         {
            for ( auto& node : mChildren ) {
               node.retrieveElements(elements);
            }
         }

         std::move(mElements.begin(), mElements.end(), std::back_inserter(elements));
      }

      bool canSplit() {
         return mBounds.width() > 10 && mBounds.height() > 10;
      }

      void split() {
         const float childwidth = mBounds.width() / 2;
         const float childheight = mBounds.height() / 2;

         mChildren.reserve(4);
         mChildren.emplace_back(RectF(mBounds.left, mBounds.top, childwidth, childheight));
         mChildren.emplace_back(RectF(mBounds.left + childwidth, mBounds.top, childwidth, childheight));
         mChildren.emplace_back(RectF(mBounds.left + childwidth, mBounds.top + childheight, childwidth, childheight));
         mChildren.emplace_back(RectF(mBounds.left, mBounds.top + childheight, childwidth, childheight));

         Elements elements;
         elements.swap(mElements);
         for ( auto& element : elements )
         {
            insert(*element.pdata, element.bounds);
         }
      }

      QuadNode<T>& findQuadrant(const T& element, const RectF& bounds) {
         Vector center = mBounds.center();
         Quadrant quadrant;
         if ( bounds.right < center.x )
         {
            if ( bounds.bottom < center.y )
               quadrant = Quadrant::eTopLeft;
            else
               quadrant = Quadrant::eBottomLeft;
         }
         else
         {
            if ( bounds.bottom < center.y )
               quadrant = Quadrant::eTopRight;
            else
               quadrant = Quadrant::eBottomRight;
         }

         return mChildren[static_cast<int>(quadrant)];
      }

    // data
      Nodes mChildren;
      Elements mElements;
      RectF mBounds;
   };
}

#endif // QUAD_NODE_H 
