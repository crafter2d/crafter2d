
#include "font.h"

#include FT_GLYPH_H

#include <GL/GLee.h>

#include "core/defines.h"

#include "fontchar.h"

#define CHECK_ERROR(msg) if ( error != 0 ) return false

Font::Font():
   Resource(),
   mFace(NULL),
   mWidth(0),
   mHeight(0),
   mBaseHeight(0)
{
}

// - Initialization

bool Font::load(FT_Library lib, const std::string& name, int size)
{
   FT_Error error = FT_New_Face(lib, name.c_str(), 0, &mFace);
	CHECK_ERROR("Not valid!");
   setName(name);

   if ( mFace->charmap == 0 && mFace->num_charmaps > 0 )
      FT_Select_Charmap(mFace, mFace->charmaps[0]->encoding );
 
	FT_Set_Char_Size(mFace, 0, size << 6, 72, 72);

   if ( !FT_IS_SCALABLE(mFace) )
   {
      FT_BBox textsize1 = measure('W');
      FT_BBox textsize2 = measure('M');
      FT_BBox textsize3 = measure('g');

      int max = textsize1.yMax > textsize2.yMax ? textsize1.yMax : textsize2.yMax;
      
      mWidth      = textsize1.xMax / 64;
      mHeight     = (max - textsize3.yMin) / 64;
      mBaseHeight = mHeight + (textsize3.yMin / 64);
   }
   else
   {
      FT_Size_Metrics& metrics = mFace->size->metrics;

      mWidth      = metrics.max_advance / 64;
      mHeight     = metrics.height / 64;
      mBaseHeight = mHeight + (metrics.descender / 64);
   }

   return true;
}

void Font::destroy()
{
   FT_Done_Face(mFace);
   mFace = NULL;
}

// - Loading

FontChar* Font::loadGlyph(char character)
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
   error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, NULL, true);
   if ( error != 0 )
   {
      FT_Done_Glyph(glyph);
      return NULL;
   }

   FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

   GLubyte* pdata = invertMonoBitmap(bitmap_glyph->bitmap);          

   FontChar* pfontchar = new FontChar(-bitmap_glyph->left,
                                       bitmap_glyph->bitmap.rows - bitmap_glyph->top,
                                       bitmap_glyph->bitmap.width,
                                       bitmap_glyph->bitmap.rows,
                                       slot->advance.x >> 6,
                                       slot->advance.y >> 6,
                                       pdata);
   ASSERT_PTR(pfontchar);

   mCharacters[character] = pfontchar;

   return pfontchar;
}

// - Rendering

void Font::render(const std::string& text)
{
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   FontChar* pfontchar = NULL;

   for ( std::size_t index = 0; index < text.length(); ++index )
   {
      char character = text[index];
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

FT_BBox Font::measure(char c)
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

uchar* Font::invertMonoBitmap(FT_Bitmap& bitmap)
{
   int width = bitmap.width / 8 + ( ( bitmap.width & 7 ) > 0 ? 1 : 0 );

    uchar* inverse = new uchar[ bitmap.rows * width ];
    uchar* inverse_ptr = inverse;

    for ( int r = 0; r < bitmap.rows; r++ ) {

      uchar* bitmap_ptr = &bitmap.buffer[bitmap.pitch * ( bitmap.rows - r - 1 )];

      for ( int p = 0; p < width; p++ )
         *inverse_ptr++ = *bitmap_ptr++;
    }

    return inverse;
}
