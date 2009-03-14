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
#include <assert.h>
#include "../defines.h"

INLINE const GuiListBox& GuiListBoxItem::listbox() const
{
   return *_box;
}

INLINE int GuiListBoxItem::count() const
{
   return static_cast<int>(_items.size());
}

INLINE GuiListBox& GuiListBoxItem::listbox()
{
   return const_cast<GuiListBox&>(me().listbox());
}

INLINE const GuiListBoxSubItem& GuiListBoxItem::operator[](int index) const
{
   assert(index >= 0 && index < _items.size() && "Index is out of range!");
   return *(_items[index]);
}

INLINE GuiListBoxSubItem& GuiListBoxItem::operator[] (int index)
{
   return const_cast<GuiListBoxSubItem&>(me()[index]);
}

INLINE void GuiListBoxItem::data(Uint32 value)
{
   _data = value;
}

INLINE Uint32 GuiListBoxItem::data() const
{
   return _data;
}

INLINE const GuiListBoxItem& GuiListBoxItem::me()
{
   return static_cast<const GuiListBoxItem&>(*this);
}

INLINE void GuiListBoxItem::selected(bool select)
{
   _selected = select;
}

INLINE bool GuiListBoxItem::selected() const
{
   return _selected;
}
