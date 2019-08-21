
#include "maxrectsalgorithm.h"

#include <algorithm>

#include "imagenode.h"

namespace c2d::gen
{
   MaxRectsAlgorithm::MaxRectsAlgorithm(int width, int height):
      mWidth(width),
      mHeight(height),
      mFreeRects()
   {
      init();
   }

   void MaxRectsAlgorithm::init()
   {
      RectI n(0, 0, mWidth, mHeight);
      mFreeRects.push_back(n);
   }

   void MaxRectsAlgorithm::insert(std::vector<ImageNode>& images, std::vector<ImageNode>& used)
   {
      while ( !images.empty() )
      {
         FindInfo bestImage;
         size_t bestImageIndex = images.size();

         for ( size_t index = 0; index < images.size(); index++ )
         {
            ImageNode& image = images[index];

            FindInfo info;
            if ( findPositionForNewNode(image, info) )
            {
               if ( info.bestShortSideFit < bestImage.bestShortSideFit
                || (info.bestShortSideFit == bestImage.bestShortSideFit && info.bestLongSideFit < bestImage.bestLongSideFit) )
               {
                  bestImage = info;
                  bestImageIndex = index;
               }
            }
         }

         if ( bestImageIndex == images.size() )
         {
            // There is no image anymore that fits in this atlas,
            // continue with next sheet.
            return;
         }

         insertRect(bestImage);

         ImageNode& image = images[bestImageIndex];
         image.mCoords = bestImage.bestNode;
         image.mRotated = bestImage.rotated;

         used.push_back(std::move(image));
         images.erase(images.begin() + bestImageIndex);
      }
   }
   
   bool MaxRectsAlgorithm::findPositionForNewNode(const ImageNode& node, FindInfo& info)
   {
      for ( size_t index = 0; index < mFreeRects.size(); index++ )
      {
         const RectI& rect = mFreeRects[index];

         if ( rect.width() >= node.width() && rect.height() >= node.height() )
         {
            int leftoverHoriz = rect.width() - node.width();
            int leftoverVert = rect.height() - node.height();
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);
            int longSideFit = std::max(leftoverHoriz, leftoverVert);

            if ( shortSideFit < info.bestShortSideFit
               || (shortSideFit == info.bestShortSideFit && longSideFit < info.bestLongSideFit) )
            {
               info.bestNode.set(rect.left, rect.top, node.width(), node.height());
               info.freerectIndex = index;
               info.bestShortSideFit = shortSideFit;
               info.bestLongSideFit = longSideFit;
               info.rotated = false;
            }
         }
         
         if ( rect.width() >= node.height() && rect.height() >= node.width() )
         {
            int flippedLeftoverHoriz = rect.width() - node.height();
            int flippedLeftoverVert = rect.height() - node.width();
            int flippedShortSideFit = std::min(flippedLeftoverHoriz, flippedLeftoverVert);
            int flippedLongSideFit = std::max(flippedLeftoverHoriz, flippedLeftoverVert);

            if ( flippedShortSideFit < info.bestShortSideFit
               || (flippedShortSideFit == info.bestShortSideFit && flippedLongSideFit < info.bestLongSideFit) )
            {
               info.bestNode.set(rect.left, rect.top, node.height(), node.width());
               info.freerectIndex = index;
               info.bestShortSideFit = flippedShortSideFit;
               info.bestLongSideFit = flippedLongSideFit;
               info.rotated = true;
            }
         }
      }

      return info.freerectIndex != -1;
   }

   void MaxRectsAlgorithm::insertRect(const FindInfo& info)
   {
      if ( info.bestNode.height() == 99 )
      {
         int aap = 53;
      }
      mFreeRects.reserve(mFreeRects.size() + 2);
      const RectI& freerect = mFreeRects[info.freerectIndex];
      
      splitFreeNode(freerect, info.bestNode);

      mFreeRects.erase(mFreeRects.begin() + info.freerectIndex);
   }

   bool MaxRectsAlgorithm::splitFreeNode(const RectI& freeNode, const RectI& usedNode)
   {
      if ( usedNode.width() < freeNode.width() )
      {
         RectI node = freeNode;
         node.left = freeNode.left + usedNode.width();
         node.bottom = freeNode.top + usedNode.height();
         mFreeRects.push_back(node);
      }

      if ( usedNode.height() < freeNode.height() )
      {
         RectI node = freeNode;
         node.top = freeNode.top + usedNode.height();
         mFreeRects.push_back(node);
      }
      return true;
   }

   void MaxRectsAlgorithm::pruneFreeList()
   {
      /// Go through each pair and remove any rectangle that is redundant.
      for ( size_t i = 0; i < mFreeRects.size(); ++i )
      {
         for ( size_t j = i + 1; j < mFreeRects.size(); ++j )
         {
            if ( mFreeRects[j].contains(mFreeRects[i]) )
            {
               mFreeRects.erase(mFreeRects.begin() + i);
               --i;
               break;
            }
            if ( mFreeRects[i].contains(mFreeRects[j]) )
            {
               mFreeRects.erase(mFreeRects.begin() + j);
               --j;
            }
         }
      }
   }
}
