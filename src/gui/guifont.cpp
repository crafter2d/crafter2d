/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "guifont.h"
#ifndef JENGINE_INLINE
#  include "guifont.inl"
#endif

#include <stdio.h>
#include <string.h>

#include FT_GLYPH_H

#include "../console.h"

#include "../math/vector.h"

#include "guimanager.h"

 GuiFont::GuiFont():
   mFace(NULL),
   base(0),
   height(0),
   baseHeight(0),
   averageWidth(0),
   maxWidth(0)
{
}

GuiFont::~GuiFont()
{
   destroy();
}

void GuiFont::initialize (const char* name, int size)
{
	FT_Library lib = GuiManager::getInstance().getFreeTypeLib();
	FT_Error error = FT_New_Face(lib, name, 0, &mFace);
	if ( error != 0 )
	{
	   Console::getInstance().error("Failed to open font");
	   return;
	}

	FT_Set_Char_Size(mFace, 0, size << 6, 72, 72);

   Vector textsize = getTextSize("W");
   
   averageWidth = textsize.x;
   maxWidth     = textsize.x;
   height       = mFace->height / 64;
   baseHeight   = 10;
}

void GuiFont::draw(const std::string& text) const
{
   draw(text.c_str(), text.length());
}

void GuiFont::draw(const char* ptext, int length) const
{
   glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

   for ( int index = 0; index < length; ++index )
   {
      drawGlyph(ptext[index]);
   }
}

GLubyte* invertMonoBitmap(FT_Bitmap& bitmap)
{
   int width = bitmap.width / 8 + ( ( bitmap.width & 7 ) > 0 ? 1 : 0 );

    GLubyte* inverse = new GLubyte[ bitmap.rows * width ];
    GLubyte* inverse_ptr = inverse;

    for ( int r = 0; r < bitmap.rows; r++ ) {

      GLubyte* bitmap_ptr = &bitmap.buffer[bitmap.pitch * ( bitmap.rows - r - 1 )];

      for ( int p = 0; p < width; p++ )
         *inverse_ptr++ = *bitmap_ptr++;
    }

    return inverse;
}

void GuiFont::drawGlyph(char ch) const
{
   int error = 0;
   FT_GlyphSlot slot = mFace->glyph;

   int faceindex = FT_Get_Char_Index(mFace, ch);
   error = FT_Load_Glyph(mFace, faceindex, FT_LOAD_DEFAULT);
   if ( error != 0 )
   {
      return;
   }

   FT_Glyph glyph;
   error = FT_Get_Glyph(slot, &glyph);
   error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, NULL, true);
   if ( error != 0 )
   {
      FT_Done_Glyph(glyph);
      return;
   }

   FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

   GLubyte* pdata = invertMonoBitmap(bitmap_glyph->bitmap);
   glBitmap(bitmap_glyph->bitmap.width,
            bitmap_glyph->bitmap.rows,
            -bitmap_glyph->left,
            bitmap_glyph->bitmap.rows - bitmap_glyph->top,
            slot->advance.x >> 6,
            slot->advance.y >> 6,
            pdata);

   delete[] pdata;

   FT_Done_Glyph(glyph);
}

void GuiFont::destroy()
{
   if ( base > 0 )
   {
      glDeleteLists(base, 1);
   }
}

Vector GuiFont::getTextSize(const std::string& text)
{
   FT_BBox bbox;
   int error = 0;
   int posx  = 0;
   int posy  = 0;

   bbox.xMin = bbox.yMin = 32000;
   bbox.xMax = bbox.yMax = -32000;

   for ( std::string::size_type index = 0; index < text.length(); ++index )
   {
      int glyph_index = FT_Get_Char_Index(mFace, text[index]);

      error = FT_Load_Glyph(mFace, glyph_index, FT_LOAD_DEFAULT );
      if ( error )
         break;

      FT_Glyph glyph;
      error = FT_Get_Glyph(mFace->glyph, &glyph );
      if ( error )
         break;

      FT_BBox glyph_bbox;
      FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_PIXELS, &glyph_bbox);

      glyph_bbox.xMin += posx;
      glyph_bbox.xMax += posx;
      glyph_bbox.yMin += posy;
      glyph_bbox.yMax += posy;

      if ( glyph_bbox.xMin < bbox.xMin )
         bbox.xMin = glyph_bbox.xMin;
      if ( glyph_bbox.yMin < bbox.yMin )
         bbox.yMin = glyph_bbox.yMin;
      if ( glyph_bbox.xMax > bbox.xMax )
         bbox.xMax = glyph_bbox.xMax;
      if ( glyph_bbox.yMax > bbox.yMax )
         bbox.yMax = glyph_bbox.yMax;

      posx += mFace->glyph->advance.x >> 6;
   }

   return Vector(bbox.xMax - bbox.xMin, bbox.yMax - bbox.yMin);
}
