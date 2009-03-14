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
#include "guiclipper.h"
#include "guigraphics.h"
#include "guiheadercontrol.h"
#include "guiheadercolumn.h"
#include "guilistbox.h"
#include "guilistboxitem.h"
#include "guitextblock.h"

#include "guilistboxsubitem.h"
#ifndef JENGINE_INLINE
#  include "guilistboxsubitem.inl"
#endif

GuiListBoxSubItem::GuiListBoxSubItem(const GuiListBoxItem& i, const std::string& str):
   _item(i),
   _pblock(NULL)
{
   text(str);
}

GuiListBoxSubItem::GuiListBoxSubItem(const GuiListBoxItem& i):
   _item(i),
   _pblock(NULL)
{
}

GuiListBoxSubItem::~GuiListBoxSubItem()
{
   destroy();
}

void GuiListBoxSubItem::destroy()
{
   if ( _pblock != 0 )
   {
      delete _pblock;
      _pblock = NULL;
   }
}

void GuiListBoxSubItem::text(const std::string& t)
{
   destroy();

   _text = t;
   _pblock = new GuiTextBlock(&item().listbox(), t, item().listbox().getLinebreak());
}

void GuiListBoxSubItem::paint(int column, const GuiPoint& pos, const GuiGraphics& graphics)
{
   if ( hasBlock() )
   {
      GuiRect rect = calculateVisibleRect(column, pos);
      item().listbox().clientToWindow(rect);
      GuiClipper clipper(rect);

      block().paint(pos, graphics);
   }
}

GuiRect GuiListBoxSubItem::calculateVisibleRect(int column, const GuiPoint& pos)
{
   const GuiHeaderCtrl& header = item().listbox().header();
   return GuiRect(pos.x, pos.x + header[column].width(), pos.y, pos.y + block().height());
}
