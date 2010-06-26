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

INLINE bool GuiListBox::isEditing() const
{
   return _editing;
}

INLINE void GuiListBox::setEditing(bool editing)
{
   _editing = editing;
}

INLINE bool GuiListBox::hasCurSel() const
{
   return curSel != -1;
}

INLINE int GuiListBox::getCurSel() const
{
   return curSel;
}

INLINE void GuiListBox::setItemData(int index, Uint32 data)
{
   assert(index>=0 && index<lines.size() && "setItemData: Invalid item index.");
   lines[index]->data(data);
}

const char* GuiListBox::getItemText(int index) const
{
   assert(index>=0 && index<lines.size() && "getItemText: Invalid item index.");
   return (*lines[index])[0].text().c_str();
}

Uint32 GuiListBox::getItemData(int index) const
{
   assert(index>=0 && index<lines.size() && "getItemData: Invalid item index.");
   return lines[index]->data();
}

INLINE Uint32 GuiListBox::getItemCount() const
{
   return lines.size();
}

INLINE void GuiListBox::removeAt(int index)
{
   lines.erase(lines.begin()+index);
}

INLINE void GuiListBox::onScroll(int pos)
{
   scrollPos = pos;
}

INLINE GuiHeaderCtrl& GuiListBox::header()
{
   return *_header;
}

INLINE const GuiHeaderCtrl& GuiListBox::header() const
{
   return *_header;
}

INLINE bool GuiListBox::hasHeader() const
{
   return IS_SET(m_style, GUI_LIST_HEADER);
}

INLINE GuiListBox::EditType GuiListBox::editType(int index) const
{
   return EEditBox;
}
