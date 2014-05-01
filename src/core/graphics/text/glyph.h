
#ifndef GLYPH_H
#define GLYPH_H

#include <cstdint>

#include "core/core_base.h"
#include "core/math/size.h"

namespace Graphics
{
   class CORE_API Glyph
   {
   public:
      Glyph();
      ~Glyph();
      
    // get/set
      const Size& getSize() const;
      void        setSize(const Size& size);

      int            getPitch() const;
      const uint8_t* getPixels() const;
      void           setPixels(uint8_t* ppixels, uint32_t pitch);

   private:

    // data
      Size     mSize;
      uint8_t* mpPixels;
      uint32_t mPitch;
   };
}

#endif // GLYPH_H
