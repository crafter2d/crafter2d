
#include "glyph.h"

#include "core/defines.h"

namespace Graphics
{
   Glyph::Glyph() :
      mSize(),
      mpPixels(NULL),
      mPitch(0),
      mBaseLine(0.0f),
      mAdvance(0.0f)
   {
   }

   Glyph::~Glyph()
   {
      delete mpPixels;
      mpPixels = NULL;
   }

   // - Get/set

   const Size& Glyph::getSize() const
   {
      return mSize;
   }

   void Glyph::setSize(const Size& size)
   {
      mSize = size;
   }

   float Glyph::getBaseLine() const
   {
      return mBaseLine;
   }

   void Glyph::setBaseLine(float baseline)
   {
      mBaseLine = baseline;
   }

   int Glyph::getPitch() const
   {
      return mPitch;
   }

   const uint8_t* Glyph::getPixels() const
   {
      return mpPixels;
   }

   void Glyph::setPixels(uint8_t* ppixels, uint32_t pitch)
   {
      mpPixels = ppixels;
      mPitch = pitch;
   }

   float Glyph::getAdvance() const
   {
      return mAdvance;
   }

   void Glyph::setAdvance(float advance)
   {
      mAdvance = advance;
   }
}
