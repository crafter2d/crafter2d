
#include "oglglyphprovider.h"

#include FT_GLYPH_H

#include "core/graphics/text/glyph.h"
#include "core/defines.h"

#include "../oglfont.h"

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
      mpFont->initialize(12.0f);
   }

   Glyph* OGLGlyphProvider::getGlyph(UChar ch, float emsize)
   {
      FT_Face face = mpFont->getFace();
      FT_GlyphSlot slot = face->glyph;

      int faceindex = FT_Get_Char_Index(face, ch);
      int error = FT_Load_Glyph(face, faceindex, FT_LOAD_DEFAULT);
      if ( error != 0 )
      {
         return NULL;
      }

      FT_Glyph glyph;
      error = FT_Get_Glyph(slot, &glyph);
      error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, true);
      if ( error != 0 )
      {
         FT_Done_Glyph(glyph);
         return NULL;
      }

      FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

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

      Glyph* presult = new Glyph();
      presult->setSize(Size(width, height));
      presult->setAdvance(slot->advance.x >> 6);
      presult->setBaseLine(bitmap_glyph->top);
      presult->setPixels(pdata, width);

      return presult;
   }
}
