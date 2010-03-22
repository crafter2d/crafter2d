/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

#include "guicolorlistbox.h"

#ifndef JENGINE_INLINE
#  include "guicolorlistbox.inl"
#endif

#include "gui/guigraphics.h"
#include "gui/guifont.h"

GuiColorListBox::GuiColorListBox():
   GuiListBox()
{
}

GuiColorListBox::~GuiColorListBox()
{
}

void GuiColorListBox::addColor(GuiColor color)
{
   colors.push_back(color);
   addString(" ");
}

void GuiColorListBox::drawItem(int idx, int posX, int posY, const GuiGraphics& graphics)
{
   // calculate the corner coordinates
   float pLX = posX + 5;
   float pRX = pLX + (m_frameRect.getWidth() - 16 - 10);
   float pUY = posY + 2;
   float pBY = pUY + font->getHeight() - 4;

   graphics.setColor(colors[idx]);
   graphics.drawRect(GuiRect(pLX, pRX, pUY, pBY));

   drawSelection(idx, posX, posY);
}
