#include "imagenode.h"

namespace c2d::gen
{
   ImageNode::ImageNode(String&& filename):
      mFilename(std::move(filename)),
      mImage(),
      mCoords(),
      mRotated(false)
   {
      mImage.load(mFilename);
   }

   ImageNode::ImageNode(ImageNode&& node) noexcept:
      mFilename(std::move(node.mFilename)),
      mImage(std::move(node.mImage)),
      mCoords(node.mCoords),
      mRotated(node.mRotated)
   {
   }

   ImageNode& ImageNode::operator=(ImageNode&& that) noexcept
   {
      mFilename = std::move(that.mFilename);
      mImage = std::move(that.mImage);
      mCoords = that.mCoords;
      mRotated = that.mRotated;
      return *this;
   }
}