
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
      uint32_t sheetindex = 0;

      for ( sheetindex = 0; sheetindex < mCurSheets; ++sheetindex )
      {
         GlyphSheet* psheet = mpSheets[sheetindex];

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
         sheetindex = mCurSheets++;

         glyphindex = psheet->insertGlyph(glyph);
      }

      if ( glyphindex == 0xffffff )
      {
         return 0xffffff;
      }

      return (sheetindex << 16) | glyphindex;
   }
}
