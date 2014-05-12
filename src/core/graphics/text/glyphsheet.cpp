
#include "glyphsheet.h"

#include "core/defines.h"
#include "core/graphics/device.h"
#include "core/graphics/texture.h"
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
      mTop(0),
      mDirty(false)
   {
   }

   // - Operations

   bool GlyphSheet::create(Device& device)
   {
      mTextureWidth = 512;
      mTextureHeight = 512;

      mpTexture = device.createTexture(mTextureWidth, mTextureHeight, 1);
      if ( mpTexture == NULL )
      {
         return false;
      }

      uint32_t size = mTextureWidth * mTextureHeight * sizeof(uint8_t);
      mpTextureData = new uint8_t[size];
      memset(mpTextureData, 255, size);

      return true;
   }
   
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

      uint8_t* pdest = mpTextureData + (mTextureWidth * mTop) + mLeft;
      const uint8_t* psrc = glyph.getPixels();
      for ( int y = 0; y < size.height; ++y )
      {
         for ( int x = 0; x < size.width; ++x )
         {
            uint8_t value = psrc[x + y * (int)size.width];
            if ( value != 0 && value != 255 )
            {
               int aap = 5;
            }
         }
         memcpy(pdest, psrc, static_cast<int>(size.width));
         psrc += static_cast<int>(size.width);
         pdest += mTextureWidth;
      }

      GlyphVertexData data;
      data.mGlyphSize.set(size.width, size.height);
      data.mBaseLine = glyph.getBaseLine();
      data.mAdvance = glyph.getAdvance();
      data.mTexturePos.set((1.0f / mTextureWidth) * mLeft, (1.0f / mTextureHeight) * mTop);
      data.mTextureDim.set(static_cast<float>(size.width) / mTextureWidth, static_cast<float>(size.height) / mTextureHeight);
      
      mCoords.push_back(data);

      mLeft += size.width;

      mDirty = true;

      return mCoords.size() - 1;
   }

   void GlyphSheet::flush(RenderContext& context)
   {
      if ( mDirty )
      {
         mpTexture->update(context, mpTextureData, mTextureWidth);
      }
   }

   const GlyphVertexData& GlyphSheet::getGlyphVertexData(uint32_t glyphindex) const
   {
      return mCoords[glyphindex & 0xffff];
   }

   const Texture& GlyphSheet::getGlyphTexture() const
   {
      return *mpTexture;
   }
}
