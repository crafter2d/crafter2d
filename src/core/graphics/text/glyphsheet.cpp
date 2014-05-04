
#include "glyphsheet.h"

#include "core/defines.h"
#include "core/math/size.h"

#include "glyph.h"

namespace Graphics
{
   GlyphSheet::GlyphSheet() :
      mpTextureData(NULL),
      mpTexture(NULL),
      mCoords(),
      mTextureWidth(0),
      mTextureHeight(0),
      mLeft(0),
      mTop(0)
   {
   }

   // - Operations
   
   uint32_t GlyphSheet::insertGlyph(const Glyph& glyph)
   {
      const Size& size = glyph.getSize();
      
      int remainingX = mTextureWidth - mLeft;
      int remainingY = mTextureHeight - mTop;

      if ( remainingX >= size.width )
      {
         // fits at least on X axis
         if ( remainingY < size.height )
         {
            return 0xffffff;
         }
      }
      else if ( remainingY < size.height )
      {
         return 0xffffff;
      }

      uint8_t* pdest = mpTextureData + (mTextureWidth * mTop * 4);
      const uint8_t* psrc = glyph.getPixels();
      for ( int y = 0; y < size.height; ++y )
      {
         memcpy(pdest, psrc, static_cast<int>(size.width));
         psrc += mTextureWidth;
         pdest += mTextureHeight;
      }

      GlyphCoord coord;
      coord.pos.set((1.0f / mTextureWidth) * mLeft, (1.0f / mTextureHeight) * mTop);
      coord.size.set(static_cast<float>(mLeft) / mTextureWidth, static_cast<float>(mTop) / mTextureHeight);
      mCoords.push_back(coord);

      return mCoords.size();
   }
}
