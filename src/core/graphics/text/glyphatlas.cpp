
#include "glyphatlas.h"

#include "core/defines.h"

#include "glyphsheet.h"

namespace Graphics
{
   GlyphAtlas::GlyphAtlas(int maxsheets):
      mpSheets(new GlyphSheet * [maxsheets]),
      mMaxSheets(maxsheets),
      mCurSheets(0)
   {
      memset(mpSheets, 0, sizeof(mpSheets));
   }

   // - Operations

   uint32_t GlyphAtlas::insertGlyph(const Glyph& glyph)
   {
      uint32_t glyphindex = 0;

      for ( int index = 0; index < mCurSheets; ++index )
      {
         GlyphSheet* psheet = mpSheets[index];

         glyphindex = psheet->insertGlyph(glyph);
         if ( glyphindex != 0xffffff )
         {
            break;
         }
      }
      
      if ( glyphindex == 0xffffff && mCurSheets < mMaxSheets )
      {
         GlyphSheet* psheet = new GlyphSheet();
         mpSheets[mCurSheets] = psheet;
         mCurSheets++;

         glyphindex = psheet->insertGlyph(glyph);
      }

      return glyphindex;
   }
}
