
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION

#include "core/string/string.h"
#include "core/streams/datastream.h"
#include "stb/stb_image.h"

c2d::Image::Image():
   mpBytes(nullptr),
   mWidth(-1),
   mHeight(-1),
   mFormat(-1)
{
}

c2d::Image::~Image()
{
   delete[] mpBytes;
}

bool c2d::Image::load(DataStream& data)
{
   delete[] mpBytes;
   mpBytes = stbi_load_from_memory((unsigned char*) data.getData(), data.getDataSize(), &mWidth, &mHeight, &mFormat, 0);
   if ( mpBytes == nullptr )
   {
      return false;
   }

   return true;
}

bool c2d::Image::load(const String& filename)
{
   delete[] mpBytes;
   mpBytes = stbi_load(filename.toUtf8().c_str(), &mWidth, &mHeight, &mFormat, 0);
   if ( mpBytes == nullptr )
   {
      return false;
   }
   return true;
}
