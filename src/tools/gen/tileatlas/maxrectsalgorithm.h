#ifndef MAX_RECTS_ALGORITHM_H
#define MAX_RECTS_ALGORITHM_H

#include <vector>

#include "core/math/rect.h"

namespace c2d::gen
{
   class ImageNode;

   class MaxRectsAlgorithm
   {
   public:
      MaxRectsAlgorithm(int width, int height);

      void insert(std::vector<ImageNode>& images, std::vector<ImageNode>& used);

   private:
      using Rects = std::vector<RectI>;

    // operations
      void init();

      struct FindInfo
      {
         FindInfo() :
            bestNode(),
            freerectIndex(-1),
            bestShortSideFit(std::numeric_limits<int>::max()),
            bestLongSideFit(std::numeric_limits<int>::max()),
            rotated(false)
         {}

         RectI bestNode;
         size_t freerectIndex;
         int bestShortSideFit;
         int bestLongSideFit;
         bool rotated;
      };

      bool findPositionForNewNode(const ImageNode& node, FindInfo& info);
      void insertRect(const FindInfo& info);
      bool splitFreeNode(const RectI& freeNode, const RectI& usedNode);
      void pruneFreeList();

    // members
      int mWidth;
      int mHeight;
      Rects mFreeRects;
   };
}

#endif // MAX_RECTS_ALGORITHM_H
