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
#include "gridlayoutmanager.h"

#include "gridcontrol.h"

#include "../guicontrol.h"

GridLayoutManager::GridLayoutManager(GuiWnd& owner):
   LayoutManager(owner),
   mControls(),
   mColumns(),
   mRows()
{
}

GridLayoutManager::~GridLayoutManager()
{
}

//////////////////////////////////////////////////////////////////////////
// - Control maintenance
//////////////////////////////////////////////////////////////////////////

bool GridLayoutManager::virAddControl(GuiControl& control)
{
   GridControl* pcontrol = new GridControl(control);

   mControls.add(pcontrol);

   mColumns.insert(*pcontrol, control.getWindowRect().left(), control.getWindowRect().getWidth() , control.growHorizontally());
   mRows.insert   (*pcontrol, control.getWindowRect().top() , control.getWindowRect().getHeight(), control.growVertically());

   return false;
}

bool GridLayoutManager::virRemoveControl(GuiControl& control)
{
   return false;
}

//////////////////////////////////////////////////////////////////////////
// - Layouting
//////////////////////////////////////////////////////////////////////////

void GridLayoutManager::virLayout()
{
   mColumns.updateSize();
   mRows.updateSize();

   mControls.updateSpanning();

   mColumns.updatePosition();
   mRows.updatePosition();

   mControls.update();
}
