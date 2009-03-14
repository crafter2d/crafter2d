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

INLINE int GuiMenuItem::getId() const
{
   return _id;
}

INLINE void GuiMenuItem::setId(int id)
{
   _id = id;
}

INLINE const std::string& GuiMenuItem::text() const
{
   return MText;
}

INLINE void GuiMenuItem::text(const std::string& text)
{  
   MText = text;
}

INLINE bool GuiMenuItem::hasSubMenu() const
{
   return MPSubMenu != NULL;
}

INLINE GuiMenu& GuiMenuItem::subMenu()
{
   ASSERT_PTR(MPSubMenu)
   return *MPSubMenu;
}

INLINE void GuiMenuItem::subMenu(GuiMenu* psubmenu)
{
   MPSubMenu = psubmenu;
}
