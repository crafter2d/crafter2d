#ifndef IMAGE_H
#define IMAGE_H

#include "core/core_base.h"

#include "core/math/rect.h"

class DataStream;
class String;

namespace c2d
{
   class CORE_API Image final
   {
   public:
      Image();
      Image(int width, int height, int format);
      Image(Image&& that) noexcept;
      ~Image();
      Image& operator=(Image&& that) noexcept;

    // get/set
      unsigned char* getBytes() const {
         return mpBytes;
      }

      int getDataSize() const {
         return mWidth * mHeight * mFormat;
      }

      int getWidth() const {
         return mWidth;
      }

      int getHeight() const {
         return mHeight;
      }

      int getFormat() const {
         return mFormat;
      }

      bool load(DataStream& data);
      bool load(const String& filename);
      void save(const String& filename);

      void addAlphaChannel();

      bool paint(int x, int y, const Image& image);
      bool paintRotated90(int x, int y, const Image& image);

   private:

    // data
      unsigned char* mpBytes;
      int mWidth;
      int mHeight;
      int mFormat;
   };
}

#endif // IMAGE_H

