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
#include "guigraphics.h"
#include "guiheadercontrol.h"
#include "guiheadercolumn.h"
#include "guilistbox.h"
#include "guilistboxsubitem.h"
#include "guitextblock.h"

#include "guilistboxitem.h"
#ifndef JENGINE_INLINE
#  include "guilistboxitem.inl"
#endif

GuiListBoxItem::GuiListBoxItem(const GuiListBox* own, const std::string& str):
   _box(own),
   _data(0),
   _selected(false)
{
   _items.push_back(new GuiListBoxSubItem(*this, str));
}

void GuiListBoxItem::resize(int newsize)
{
   if ( newsize > count() )
   {
      for (int i = count(); i < newsize; ++i)
      {
         _items.push_back(new GuiListBoxSubItem(*this));
      }
   }
}

void GuiListBoxItem::paint(const GuiPoint& pos, const GuiGraphics& graphics)
{
   GuiPoint p(pos);
   const GuiHeaderCtrl& header = listbox().header();
   for (int i=0; i < count(); ++i)
   {
      _items[i]->paint(i, p, graphics);
      p.x += header[i].width();
   }
}
