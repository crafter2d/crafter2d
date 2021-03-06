
#include "oglfont.h"

#include FT_GLYPH_H

#include "core/defines.h"

using namespace Graphics;

OGLFont::OGLFont(GlyphProvider* pprovider, FT_Face face):
   Font(pprovider),
   mFace(face),
   mWidth(0),
   mHeight(0),
   mBaseHeight(0)
{
}

OGLFont::~OGLFont()
{
   FT_Done_Face(mFace);
}

bool OGLFont::initialize(float pointsize)
{
   if ( mFace->charmap == 0 && mFace->num_charmaps > 0 )
      FT_Select_Charmap(mFace, mFace->charmaps[0]->encoding );

	FT_Set_Char_Size(mFace, 0, pointsize * 64, 96, 96);

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

// - Get/set

FT_Face OGLFont::getFace() const
{
   return mFace;
}

// - Query

int OGLFont::getBaseLine() const
{
   return mBaseHeight;
}

// - Helpers

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
