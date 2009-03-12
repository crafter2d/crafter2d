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
#include "layoutmanager.h"

LayoutManager::LayoutManager(GuiWnd& owner):
   mOwner(owner)
{
}

LayoutManager::~LayoutManager()
{
}

//////////////////////////////////////////////////////////////////////////
// - Control maintenance
//////////////////////////////////////////////////////////////////////////

void LayoutManager::AddControl(GuiControl& control)
{
   virAddControl(control);
}

bool LayoutManager::virAddControl(GuiControl& control)
{
   return false;
}

void LayoutManager::RemoveControl(GuiControl& control)
{
   virRemoveControl(control);
}

bool LayoutManager::virRemoveControl(GuiControl& control)
{
   return false;
}
