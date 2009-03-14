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
#include "guidesigner.h"
#include "guiimage.h"
#ifndef JENGINE_INLINE
#  include "guiimage.inl"
#endif

REGISTER_DESIGNER(GuiImage, GuiImageId, "Image", 40, 15, 8)

GuiImage::GuiImage():
   GuiControl(),
   tex(),
   texCoords()
{
}

void GuiImage::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::onCreate(rect, caption, style, parent);
}

void GuiImage::setImage(const char* image)
{
   tex.load(image);

   texCoords.set(0, tex.getSourceWidth(), 0, tex.getSourceHeight());
}

void GuiImage::setOffset(int x, int y)
{
   float ax = tex.getSourceWidth() / tex.getWidth();
   float ay = tex.getSourceHeight() / tex.getHeight();

   texCoords.set(0, ax * getWindowRect().getWidth(), 0, ay * getWindowRect().getHeight());
   texCoords.offset(ax * x, ay * y);
}

void GuiImage::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( tex.isValid() )
      graphics.drawImage(tex, texCoords, m_frameRect);
}

REGISTER_DESIGNER(GuiMovie, GuiMovieId, "Movie", 40, 15, 8)

GuiMovie::GuiMovie():
   GuiImage(),
   movie()
{
}

void GuiMovie::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::onCreate(rect, caption, style, parent);

   // swap the vertical coordinates, as the movie is upside down otherwise.
   //texCoords.set(0, rect.getWidth(), rect.getHeight(), 0);
   texCoords.set(0,1, 1,0);
}

void GuiMovie::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( movie.isValid() )
   {
      movie.update(tick);
      graphics.drawImage(movie, texCoords, m_frameRect);
   }
}
