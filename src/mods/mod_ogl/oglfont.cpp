
#include "oglfont.h"

#include FT_GLYPH_H

#include "core/defines.h"

#include "fontchar.h"

using namespace Graphics;

OGLFont::OGLFont(FT_Face face):
   Font(),
   mFace(face)
{
}

OGLFont::~OGLFont()
{
   FT_Done_Face(mFace);
}

bool OGLFont::initialize(int pointsize)
{
   if ( mFace->charmap == 0 && mFace->num_charmaps > 0 )
      FT_Select_Charmap(mFace, mFace->charmaps[0]->encoding );

	FT_Set_Char_Size(mFace, 0, pointsize << 6, 96, 96);

   if ( !FT_IS_SCALABLE(mFace) )
   {
      FT_BBox textsize1 = measure('W');
      FT_BBox textsize2 = measure('M');
      FT_BBox textsize3 = measure('g');

      int max = textsize1.yMax > textsize2.yMax ? textsize1.yMax : textsize2.yMax;

      mWidth      = textsize1.xMax >> 6;
      mHeight     = (max - textsize3.yMin) >> 6;
      mBaseHeight = mHeight + (textsize3.yMin >> 6);
   }
   else
   {
      FT_Size_Metrics& metrics = mFace->size->metrics;

      mWidth      = metrics.max_advance >> 6;
      mHeight     = metrics.height >> 6;
      mBaseHeight = mHeight + (metrics.descender >> 6);
   }

   return true;
}

// - Query

int OGLFont::getBaseLine() const
{
   return mBaseHeight;
}

// - Size calculation

int OGLFont::getTextWidth(const String& text) const
{
   int result = 0;
   FontChar* pfontchar = NULL;

   for ( uint32_t index = 0; index < text.length(); ++index )
   {
      UChar character = text[index];
      Characters::const_iterator it = mCharacters.find(character);
      if ( it == mCharacters.end() )
      {
         pfontchar = const_cast<OGLFont*>(this)->loadGlyph(character);
      }
      else
      {
         pfontchar = it->second;
      }

      result += pfontchar->getAdvanceX();
   }

   return result;
}

int OGLFont::getTextHeight(const String& text) const
{
   return mHeight;
}


// - Rendering

void OGLFont::render(const String& text)
{
   FontChar* pfontchar = NULL;

   for ( uint32_t index = 0; index < text.length(); ++index )
   {
      UChar character = text[index];
      Characters::iterator it = mCharacters.find(character);
      if ( it == mCharacters.end() )
      {
         pfontchar = loadGlyph(character);
      }
      else
      {
         pfontchar = it->second;
      }

      pfontchar->render();
   }
}

// - Helpers

FontChar* OGLFont::loadGlyph(UChar character)
{
   ASSERT_PTR(mFace);

   FT_GlyphSlot slot = mFace->glyph;

   int faceindex = FT_Get_Char_Index(mFace, character);
   int error = FT_Load_Glyph(mFace, faceindex, FT_LOAD_DEFAULT);
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

   FontChar* pfontchar = new FontChar(-bitmap_glyph->left,
                                       bitmap_glyph->bitmap.rows - bitmap_glyph->top,
                                       bitmap_glyph->bitmap.width,
                                       bitmap_glyph->bitmap.rows,
                                       slot->advance.x >> 6,
                                       slot->advance.y >> 6);

   ASSERT_PTR(pfontchar);
   pfontchar->fromBitmap(bitmap_glyph->bitmap);

   mCharacters[character] = pfontchar;

   FT_Done_Glyph(glyph);

   return pfontchar;
}

FT_BBox OGLFont::measure(char c)
{
   FT_BBox bbox;

   bbox.xMin = bbox.yMin = bbox.xMax = bbox.yMax = 0;

   // For starters, just get the unscaled glyph bounding box

   FT_UInt glyph_index = FT_Get_Char_Index(mFace, c );
   if ( glyph_index == 0 )
      return bbox;

   FT_Error error = FT_Load_Glyph(mFace, glyph_index, FT_LOAD_DEFAULT);

   if ( error != 0 )
      return bbox;

   FT_Glyph glyph;
   error = FT_Get_Glyph(mFace->glyph, &glyph );
   if ( error != 0 )
      return bbox;

   FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_GRIDFIT, &bbox );
   FT_Done_Glyph( glyph );

   return bbox;
}
