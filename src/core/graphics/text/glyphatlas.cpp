
#include "glyphatlas.h"

#include <string.h>

#include "core/defines.h"

#include "glyph.h"
#include "glyphmap.h"
#include "glyphprovider.h"
#include "glyphsheet.h"

namespace Graphics
{
   GlyphAtlas::GlyphAtlas(Device& device, GlyphProvider* pprovider, int maxsheets) :
      mDevice(device),
      mpProvider(pprovider),
      mGlyphMaps(),
      mpSheets(new GlyphSheet * [maxsheets]),
      mMaxSheets(maxsheets),
      mCurSheets(0)
   {
      memset(mpSheets, 0, sizeof(*mpSheets) * maxsheets);
   }

   GlyphAtlas::~GlyphAtlas()
   {
      for ( auto it : mGlyphMaps )
      {
         delete it.second;
      }

      for ( int index = 0; index < mMaxSheets; ++index )
      {
         delete mpSheets[index];
      }

      delete[] mpSheets;
      mpSheets = nullptr;
   }

   // - Get/set

   GlyphProvider& GlyphAtlas::getProvider()
   {
      ASSERT_PTR(mpProvider);
      return *mpProvider;
   }

   // - Query

   GlyphMap& GlyphAtlas::getGlyphMap(float size)
   {
      GlyphMaps::iterator it = mGlyphMaps.find(size);
      if ( it != mGlyphMaps.end() )
      {
         return *it->second;
      }

      GlyphMap* pmap = new GlyphMap(size);
      mGlyphMaps.insert(std::make_pair(size, pmap));
      return *pmap;
   }
   
   // - Operations

   uint32_t GlyphAtlas::getGlyph(GlyphMap& map, UChar ch)
   {
      uint32_t glyphindex = map.lookup(ch);
      if ( glyphindex == 0xffffff )
      {
         Glyph* pglyph = mpProvider->getGlyph(ch, map.getFontSize());
         if ( pglyph != NULL )
         {
            glyphindex = insertGlyph(*pglyph);
            if ( glyphindex != 0xffffff )
            {
               map.insert(ch, glyphindex);
            }

            delete pglyph;
         }
      }
      return glyphindex;
   }

   const GlyphVertexData& GlyphAtlas::getGlyphVertexData(uint32_t glyphindex) const
   {
      uint32_t sheetindex = glyphindex >> 16;
      ASSERT(sheetindex != 0xfffff);
      return mpSheets[sheetindex]->getGlyphVertexData(glyphindex);
   }

   const Texture& GlyphAtlas::getGlyphTexture(uint32_t glyphindex) const
   {
      uint32_t sheetindex = glyphindex >> 16;
      ASSERT(sheetindex != 0xfffff);
      return mpSheets[sheetindex]->getGlyphTexture();
   }

   uint32_t GlyphAtlas::insertGlyph(const Glyph& glyph)
   {
      uint32_t glyphindex = 0xffffff;
      int32_t sheetindex = 0;

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
         psheet->create(mDevice);
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

   void GlyphAtlas::flush(RenderContext& context)
   {
      for ( int sheet = 0; sheet < mCurSheets; ++sheet )
      {
         mpSheets[sheet]->flush(context);
      }
   }
}
