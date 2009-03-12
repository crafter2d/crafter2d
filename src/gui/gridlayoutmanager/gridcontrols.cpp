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
#include "gridcontrols.h"

#include "gridcontrol.h"

GridControls::GridControls()
{
}

GridControls::~GridControls()
{
}

void GridControls::add(GridControl* pcontrol)
{
   mControls.push_back(pcontrol);
}

//////////////////////////////////////////////////////////////////////////
// - Layouting
//////////////////////////////////////////////////////////////////////////

void GridControls::update()
{
   for ( Controls::iterator it = mControls.begin(); it != mControls.end(); ++it )
   {
      GridControl& control = *(*it);
      control.update();
   }
}

void GridControls::updateSpanning()
{
   for ( Controls::size_type idx = 0; idx < mControls.size(); ++idx )
   {
      GridControl& control = *(mControls[idx]);
      control.updateSpanningAxes();
   }
}
