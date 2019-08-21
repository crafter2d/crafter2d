
#include "image.h"

#define STBI_NO_HDR
#define STBI_NO_LINEAR

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "core/defines.h"
#include "core/string/string.h"
#include "core/streams/datastream.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

namespace c2d
{
   Image::Image() :
      mpBytes(nullptr),
      mWidth(-1),
      mHeight(-1),
      mFormat(-1)
   {
   }

   Image::Image(int width, int height, int format):
      mpBytes((stbi_uc*)STBI_MALLOC(width * height * format)),
      mWidth(width),
      mHeight(height),
      mFormat(format)
   {
      memset(mpBytes, 0, width * height * format);
   }

   Image::Image(Image&& that) noexcept:
      mpBytes(that.mpBytes),
      mWidth(that.mWidth),
      mHeight(that.mHeight),
      mFormat(that.mFormat)
   {
      that.mpBytes = nullptr;
   }

   Image::~Image()
   {
      stbi_image_free(mpBytes);
   }

   Image& Image::operator=(Image&& that) noexcept
   {
      if ( this != &that )
      {
         mpBytes = that.mpBytes;
         mWidth = that.mWidth;
         mHeight = that.mHeight;
         mFormat = that.mFormat;

         that.mpBytes = nullptr;
      }
      return *this;
   }

   bool Image::load(DataStream& data)
   {
      stbi_image_free(mpBytes);
      mpBytes = stbi_load_from_memory((unsigned char*)data.getData(), data.getDataSize(), &mWidth, &mHeight, &mFormat, 0);
      if ( mpBytes == nullptr )
      {
         return false;
      }

      return true;
   }

   bool Image::load(const String& filename)
   {
      stbi_image_free(mpBytes);
      mpBytes = stbi_load(filename.toUtf8().c_str(), &mWidth, &mHeight, &mFormat, 0);
      if ( mpBytes == nullptr )
      {
         return false;
      }
      return true;
   }

   void Image::save(const String& filename)
   {
      stbi_write_png(filename.toUtf8().c_str(), mWidth, mHeight, mFormat, mpBytes, 0);
   }

   void Image::addAlphaChannel()
   {
      // the conversion functions frees the old image, so no need to do that here as well
      mpBytes = stbi__convert_format(mpBytes, mFormat, STBI_rgb_alpha, mWidth, mHeight);
      mFormat = 4;
   }

   /// For now we only support the same image format
   bool Image::paint(int xpos, int ypos, const Image& image)
   {
      ASSERT(image.mWidth <= mWidth);
      ASSERT(image.mHeight <= mHeight);

      if ( image.getFormat() != mFormat )
         return false;

      size_t srcpitch = mFormat * image.mWidth;
      size_t dstpitch = mFormat * mWidth;
      stbi_uc* psrcdata = image.mpBytes;
      stbi_uc* pdstdata = &mpBytes[ypos * dstpitch + xpos * mFormat];
      for ( int y = 0; y < image.mHeight; ++y )
      {
         memcpy(pdstdata, psrcdata, srcpitch);
         pdstdata += dstpitch;
         psrcdata += srcpitch;
      }

      return true;
   }

   bool Image::paintRotated90(int xpos, int ypos, const Image& image)
   {
      ASSERT(image.mWidth <= mWidth);
      ASSERT(image.mHeight <= mHeight);

      if ( image.getFormat() != mFormat )
         return false;

      size_t srcpitch = mFormat * image.mWidth;
      size_t dstpitch = mFormat * mWidth;
      for ( int r = 0; r < image.mHeight; ++r )
      {
         for ( int c = 0; c < image.mWidth; ++c )
         {
            stbi_uc* pdest = &mpBytes[(ypos + c) * dstpitch + ((xpos + image.mHeight - r - 1) * mFormat)];
            stbi_uc* psrc  = &image.mpBytes[r * srcpitch + c * mFormat];
            memcpy(pdest, psrc, mFormat);
         }
      }

      return true;
   }
}
