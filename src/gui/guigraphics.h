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
#ifndef _GUIGRAPHICS_H_
#define _GUIGRAPHICS_H_

class GuiPoint;
class GuiRect;
class GuiColor;
class GuiCanvas;
class Texture;

class GuiGraphics
{
public:
               GuiGraphics();
               ~GuiGraphics();

   const GuiCanvas&  canvas() const;
         GuiCanvas&  canvas();
   void              canvas(GuiCanvas* pcanvas);

   void        setColor(float r, float g, float b) const;
   void        setColor(const GuiColor& color) const;

   void        drawImage(const Texture& tex, const GuiRect& dest) const;
   void        drawImage(const Texture& tex, const GuiRect& src, const GuiRect& dest) const;

   void        drawLine(const GuiPoint& p1, const GuiPoint& p2) const;

   void        drawRect(const GuiRect& rect) const;
   void        drawWireRect(const GuiRect& rect) const;
   void        drawRaisedRect(const GuiRect& rect) const;
   void        drawDefaultRect(const GuiRect& rect) const;
   void        drawSunkenRect(const GuiRect& rect) const;

private:
   const GuiGraphics& me();

   GuiCanvas* MPCanvas;
};

#ifdef JENGINE_INLINE
# include "guigraphics.inl"
#endif

#endif
