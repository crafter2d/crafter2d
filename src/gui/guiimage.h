/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GUIIMAGE_H_
#define GUIIMAGE_H_

#include "../texture.h"
#include "../movie.h"

#include "guicontrol.h"

class GuiImage : public GuiControl
{
public:
   DESIGNER_REGISTRATION(GuiImage)

   GuiImage();

   void           setImage(const char* image);
   void           setOffset(int x, int y);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   Texture tex;
   GuiRect texCoords;
};

class GuiMovie: public GuiImage
{
public:
   DESIGNER_REGISTRATION(GuiMovie)

   GuiMovie();

   void           setMovie(const char* mov);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   Movie movie;
};

#ifdef JENGINE_INLINE
#  include "guiimage.inl"
#endif

#endif
