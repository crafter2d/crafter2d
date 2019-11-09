#include "quadnode.h"

#include "core/graphics/sprites/sprite.h"

namespace c2d
{
   std::vector<const Sprite*> QuadNode::get(const RectF& bounds) const
   {
      std::vector<const Sprite*> result;
      retrieve(bounds, result);
      return result;
   }

   void QuadNode::retrieve(const RectF& bounds, std::vector<const Sprite*>& sprites) const
   {
      if ( mBounds.overlaps(bounds) )
      {
         if ( !mChildren.empty() )
         {
            for ( auto& node : mChildren ) {
               node.retrieve(bounds, sprites);
            }
         }

         for ( auto psprite : mSprites )
         {
            RectF spritebounds = psprite->getBounds();
            if ( bounds.overlaps(spritebounds) )
            {
               sprites.push_back(psprite);
            }
         }
      }
   }

   int QuadNode::count() const
   {
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
         return mSprites.size();
      }
   }

   void QuadNode::insert(const Sprite& sprite)
   {
      if ( !mChildren.empty() )
      {
         Quadrant q = findQuadrant(sprite);
         mChildren[static_cast<int>(q)].insert(sprite);
      }
      else if ( mSprites.size() >= 3 )
      {
         split();
         insert(sprite);
      }
      else
      {
         mSprites.push_back(&sprite);
      }
   }

   void QuadNode::split()
   {
      float childwidth = mBounds.width() / 2;
      float childheight = mBounds.height() / 2;
      mChildren.reserve(4);
      mChildren.emplace_back(RectF(mBounds.left, mBounds.top, childwidth, childheight));
      mChildren.emplace_back(RectF(mBounds.left + childwidth, mBounds.top, childwidth, childheight));
      mChildren.emplace_back(RectF(mBounds.left + childwidth, mBounds.top + childheight, childwidth, childheight));
      mChildren.emplace_back(RectF(mBounds.left, mBounds.top, childwidth + childheight, childheight));

      std::vector<const Sprite*> local_sprites;
      local_sprites.swap(mSprites);
      for ( auto psprite : local_sprites )
      {
         insert(*psprite);
      }
   }

   auto QuadNode::findQuadrant(const Sprite& sprite) const -> Quadrant
   {
      RectF bounds = sprite.getBounds();
      Vector center = mBounds.center();
      if ( bounds.right < center.x )
      {
         if ( bounds.bottom < center.y )
            return Quadrant::eTopLeft;
         else
            return Quadrant::eBottomLeft;
      }
      else
      {
         if ( bounds.bottom < center.y )
            return Quadrant::eTopRight;
         else
            return Quadrant::eBottomRight;
      }
      return Quadrant::eTopLeft;
   }
}
