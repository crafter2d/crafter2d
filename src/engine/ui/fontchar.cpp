/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#include "fontchar.h"
#ifndef JENGINE_INLINE
#  include "fontchar.inl"
#endif

#include <ft2build.h>
#include FT_GLYPH_H

#include <GL/GLee.h>

#include "core/defines.h"

FontChar::FontChar(int left, int top, int width, int height, int advancex, int advancey):
   mLeft(left),
   mTop(top),
   mWidth(width),
   mHeight(height),
   mAdvanceX(advancex),
   mAdvanceY(advancey),
   mTexture(-1),
   mList(-1)
{
}

FontChar::~FontChar()
{
   glDeleteLists(mList, 1);
   glDeleteTextures(1, &mTexture);
}

// operations

inline int next_p2 (int a )
{
   int rval=1;
   // rval<<=1 is a prettier way of writing rval*=2; 
   while(rval<a) rval<<=1;
   return rval;
}
   
bool FontChar::fromBitmap(FT_Bitmap& bitmap)
{
   // based on lesson 43 on Nehe 
   int width = next_p2( bitmap.width );
   int height = next_p2( bitmap.rows );

   GLuint datasize = 2 * width * height;
   GLubyte* pdata = new GLubyte[datasize];
   memset(pdata, 0, datasize);

   for( int j = 0; j < bitmap.rows; j++ )
   {
      for(int i=0; i < bitmap.width; i++)
      {
         GLuint pos = i + j * width;
         pdata[2 * pos] = pdata[2 * pos + 1] = bitmap.buffer[i + j * bitmap.width];
      }
   }

   // create the texture

   glGenTextures(1, &mTexture);
   glBindTexture(GL_TEXTURE_2D, mTexture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pdata);

   delete[] pdata;

   ASSERT(glGetError() == GL_NO_ERROR);
   
   // create the display list

   glPushMatrix();

   mList = glGenLists(1);
   glNewList(mList, GL_COMPILE);

   glBindTexture(GL_TEXTURE_2D, mTexture);

   glPushMatrix();
   glTranslatef((float)mLeft, 0, 0);
   glTranslatef(0, (float)mTop - bitmap.rows,0);

   float x=(float)bitmap.width / (float)width;
   float y=(float)bitmap.rows / (float)height;

   glBegin(GL_QUADS);
   glTexCoord2d(0,y); glVertex2f(0,bitmap.rows);
   glTexCoord2d(0,0); glVertex2f(0,0);
   glTexCoord2d(x,0); glVertex2f(bitmap.width,0);
   glTexCoord2d(x,y); glVertex2f(bitmap.width,bitmap.rows);
   glEnd();

   glPopMatrix();
   glTranslatef(mAdvanceX, mAdvanceY, 0);

   glEndList();

   ASSERT(glGetError() == GL_NO_ERROR);

   glPopMatrix();

   return true;
}

// - Rendering

void FontChar::render()
{
   glCallList(mList);

   ASSERT(glGetError() == GL_NO_ERROR);
}
