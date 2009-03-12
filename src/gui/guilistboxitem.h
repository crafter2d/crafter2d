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
#ifndef GUILISTBOXITEM_H_
#define GUILISTBOXITEM_H_

#include <vector>
#include <SDL/SDL.h>

class GuiGraphics;
class GuiListBox;
class GuiListBoxSubItem;
class GuiPoint;

class GuiListBoxItem
{
public:
   typedef std::vector<GuiListBoxSubItem*> SubItems;

                              GuiListBoxItem(const GuiListBox* own, const std::string& str);

   GuiListBox&                listbox();
   const GuiListBox&          listbox() const;

   GuiListBoxSubItem&         operator[](int index);
   const GuiListBoxSubItem&   operator[](int index) const;

   Uint32                     data() const;
   void                       data(Uint32 value);

   int                        count() const;

   bool                       selected() const;
   void                       selected(bool select);

 // Operations
   void                       paint(const GuiPoint& pos, const GuiGraphics& graphics);
   void                       resize(int newsize);

protected:
   const GuiListBoxItem&      me();

private:
   const GuiListBox* _box;
   SubItems          _items;
   Uint32            _data;
   bool              _selected;
};

#ifdef JENGINE_INLINE
#  include "guilistboxitem.inl"
#endif

#endif
