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
#include "../defines.h"

INLINE void GuiList::Iterator::operator++()
{
   if ( cur != NULL )
      cur = cur->next;
}

INLINE void GuiList::Iterator::operator--()
{
   if ( cur != NULL )
      cur = cur->prev;
}

INLINE GuiWnd* GuiList::Iterator::operator* ()
{ 
   return (cur ? cur->wnd : NULL);
}

INLINE bool GuiList::Iterator::operator!=(const Iterator& it) const
{
   return (it.cur != cur);
}

INLINE bool GuiList::Iterator::valid() const
{ 
   return cur != NULL;
}

INLINE GuiList::Iterator GuiList::begin() const
{
   return Iterator(head);
}

INLINE int GuiList::count() const
{
   return size;
}

INLINE bool GuiList::empty() const
{
   return (size == 0);
}
