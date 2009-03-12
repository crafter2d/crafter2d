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
#include <GL/GLee.h>
#include "../texture.h"

#include "guiwnd.h"
#include "guicanvas.h"

#include "guigraphics.h"
#ifndef JENGINE_INLINE
# include "guigraphics.inl"
#endif

GuiGraphics::GuiGraphics():
   MPCanvas(NULL)
{
}

GuiGraphics::~GuiGraphics()
{
   MPCanvas = NULL;
}

void GuiGraphics::setColor(float r, float g, float b) const
{
   glColor3f(r,g,b);
}

void GuiGraphics::setColor(const GuiColor& color) const
{
   glColor3fv(&color.r);
}

void GuiGraphics::drawImage(const Texture& tex, const GuiRect& dest) const
{
   drawImage(tex, GuiRect(0.0f, tex.getSourceWidth(), 0.0f, tex.getSourceHeight()), dest);
}

void GuiGraphics::drawImage(const Texture& tex, const GuiRect& src, const GuiRect& dest) const
{
   tex.enable();
   glBegin(GL_QUADS);
   glTexCoord2f(src.left(),   src.top());
	glVertex2f  (dest.left(),  dest.top());
   glTexCoord2f(src.left(),   src.bottom());
	glVertex2f  (dest.left(),  dest.bottom());
   glTexCoord2f(src.right(),  src.bottom());
	glVertex2f  (dest.right(), dest.bottom());
   glTexCoord2f(src.right(),  src.top());
	glVertex2f  (dest.right(), dest.top());
   glEnd();
   tex.disable();
}

void GuiGraphics::drawLine(const GuiPoint& p1, const GuiPoint& p2) const
{
   glBegin(GL_LINES);
   glVertex2i(p1.x, p1.y);
   glVertex2i(p2.x, p2.y);
   glEnd();
}

void GuiGraphics::drawRect(const GuiRect& rect) const
{
   glRectf(rect.left(), rect.top(), rect.right(), rect.bottom());
}

void GuiGraphics::drawRaisedRect(const GuiRect& rect) const
{
   glColor4f (0.47f, 0.639f, 0.674f, 0.6f);
   glRectf(rect.left(), rect.top(), rect.right()-1, rect.bottom()-1);
   glColor4f (0.07f, 0.239f, 0.274f, 0.6f);
   glRectf(rect.left()+1, rect.top()+1, rect.right(), rect.bottom());
   glColor4f (0.27f, 0.439f, 0.474f, 0.6f);
   glRectf(rect.left()+1, rect.top()+1, rect.right()-1, rect.bottom()-1);
}

void GuiGraphics::drawDefaultRect(const GuiRect& rect) const
{
   glColor4f (0.67f, 0.839f, 0.874f, 0.6f);
   glRectf(rect.left(), rect.top(), rect.right()-1.5, rect.bottom()-1.5);
   glColor4f (0.07f, 0.239f, 0.274f, 0.6f);
   glRectf(rect.left()+1.5, rect.top()+1.5, rect.right(), rect.bottom());
   glColor4f (0.27f, 0.439f, 0.474f, 0.6f);
   glRectf(rect.left()+1.5, rect.top()+1.5, rect.right()-1.5, rect.bottom()-1.5);
}

void GuiGraphics::drawSunkenRect(const GuiRect& rect) const
{
   glColor4f (0.07f, 0.239f, 0.274f, 0.6f);
   glRectf(rect.left(), rect.top(), rect.right()-1, rect.bottom()-1);
   glColor4f (0.47f, 0.639f, 0.674f, 0.6f);
   glRectf(rect.left()+1, rect.top()+1, rect.right(), rect.bottom());
   glColor4f (0.356f, 0.584f, 0.631f, 0.5f);
   glRectf(rect.left()+1, rect.top()+1, rect.right()-1, rect.bottom()-1);
}

void GuiGraphics::drawWireRect(const GuiRect& rect) const
{
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(rect.left(), rect.top(), rect.right(), rect.bottom());
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
