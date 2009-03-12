/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "guitreeboxitem.h"
#ifndef JENGINE_INLINE
#  include "guitreeboxitem.inl"
#endif

GuiTreeBoxItem::GuiTreeBoxItem():
   _icon(NULL),
   _text(),
   _puserdata(NULL),
   _selected(false)
{
}

GuiTreeBoxItem::~GuiTreeBoxItem()
{
}

void GuiTreeBoxItem::setIcon(const std::string& icon)
{
   _icon = ResourceManager::getInstance().loadTexture(icon.c_str());
}
