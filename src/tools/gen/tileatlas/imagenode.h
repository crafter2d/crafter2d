#ifndef IMAGE_NODE_H
#define IMAGE_NODE_H

#include "core/graphics/image.h"
#include "core/string/string.h"
#include "core/math/rect.h"

namespace c2d::gen
{
   class ImageNode
   {
   public:
      explicit ImageNode(String&& filename);
      ImageNode(ImageNode&& node) noexcept;

      ImageNode& operator=(ImageNode&& that) noexcept;

    // get/set
      int width() const { return mImage.getWidth(); }
      int height() const { return mImage.getHeight(); }

      String mFilename;
      Image mImage;
      RectI mCoords;
      bool  mRotated;
   };
}

#endif // IMAGE_NODE_H
