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
#include "guiheadercolumn.h"
#ifndef JENGINE_INLINE
#  include "guiheadercolumn.inl"
#endif

#include "gui/guifont.h"
#include "gui/guigraphics.h"
#include "gui/guimanager.h"
#include "gui/guitext.h"

#include "guiheadercontrol.h"

GuiHeaderColumn::GuiHeaderColumn(GuiHeaderCtrl* parent, int pixels):
   _header(parent),
   _text(),
   _width(pixels),
   _editable(false)
{
   const GuiRect& frame = header().getWindowRect();
   _rect.set(frame.left(), frame.left()+pixels, frame.top(), frame.bottom());
}

GuiHeaderColumn::GuiHeaderColumn(GuiHeaderCtrl* parent, int left, int pixels):
   _header(parent),
   _text(),
   _width(pixels),
   _editable(false)
{
   const GuiRect& frame = header().getWindowRect();
   _rect.set(left+frame.left(), left+frame.left()+pixels, frame.top(), frame.bottom());
}

void GuiHeaderColumn::paint(const GuiGraphics& graphics)
{
   graphics.drawSunkenRect(_rect);

   const GuiFont& font = *header().getFont();
   int y = _rect.top() + font.getBaseHeight();

   graphics.setColor(GuiManager::getInstance().getDefaultTextColor());
   GuiText::printf(font, _rect.left()+3, y, _text.c_str());
}

void GuiHeaderColumn::resize(int relx)
{
   _rect.resize(_rect.getWidth()+relx, _rect.getHeight());
}
