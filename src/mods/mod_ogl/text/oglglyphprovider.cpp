
#include "oglglyphprovider.h"

#include FT_GLYPH_H

#include "core/graphics/text/glyph.h"
#include "core/defines.h"

#include "oglfont.h"

namespace Graphics
{
   OGLGlyphProvider::OGLGlyphProvider(FT_Library library) :
      mLibrary(library),
      mpFont(NULL)
   {
   }

   void OGLGlyphProvider::initialize(OGLFont& font)
   {
      mpFont = &font;
      mpFont->initialize(12);
   }

   bool OGLGlyphProvider::getGlyph(UChar ch, float emsize, Glyph& glyph)
   {
      C2D_UNUSED(emsize);
      
      FT_Face face = mpFont->getFace();
      FT_GlyphSlot slot = face->glyph;

      int faceindex = FT_Get_Char_Index(face, ch);
      int error = FT_Load_Glyph(face, faceindex, FT_LOAD_DEFAULT);
      if ( error != 0 )
      {
         return false;
      }

      FT_Glyph ftglyph;
      error = FT_Get_Glyph(slot, &ftglyph);
      error = FT_Glyph_To_Bitmap(&ftglyph, FT_RENDER_MODE_NORMAL, NULL, true);
      if ( error != 0 )
      {
         FT_Done_Glyph(ftglyph);
         return false;
      }

      FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)ftglyph;

      int width = bitmap_glyph->bitmap.width;
      int height = bitmap_glyph->bitmap.rows;

      uint32_t datasize = width * height;
      uint8_t* pdata = new uint8_t[datasize];
      memset(pdata, 0, datasize);

      for ( int j = 0; j < height; j++ )
      {
         for ( int i = 0; i < width; i++ )
         {
            uint8_t pos = i + j * width;
            pdata[pos] = bitmap_glyph->bitmap.buffer[pos];
         }
      }

      glyph.setSize(Size(static_cast<float>(width), static_cast<float>(height)));
      glyph.setAdvance(static_cast<float>(slot->advance.x >> 6));
      glyph.setBaseLine(static_cast<float>(bitmap_glyph->top));
      glyph.setPixels(pdata, width);

      FT_Done_Glyph(ftglyph);

      return true;
   }
}
