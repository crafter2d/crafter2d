
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
   }

   Image::~Image()
   {
      stbi_image_free(mpBytes);
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
}
