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
#ifndef GRID_LAYOUT_MANAGER_H_
#define GRID_LAYOUT_MANAGER_H_

#include "../layoutmanager/layoutmanager.h"

#include "gridcontrols.h"
#include "gridaxes.h"

class GridLayoutManager : public LayoutManager
{
public:
   explicit GridLayoutManager(GuiWnd& owner);
   virtual ~GridLayoutManager();

protected:
 // control maintenance
   virtual bool   virAddControl(GuiControl& control);
   virtual bool   virRemoveControl(GuiControl& control);

 // layouting
   virtual void   virLayout();

private:
   GridControls   mControls;
   GridAxes       mColumns;
   GridAxes       mRows;
};

#endif // GRID_LAYOUT_MANAGER_H_
