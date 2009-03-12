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
#include "gridaxiscontrols.h"

#include <algorithm>

#include "../../defines.h"

#include "gridcontrol.h"

GridAxisControls::GridAxisControls():
   mControls()
{
}

GridAxisControls::~GridAxisControls()
{
}

//////////////////////////////////////////////////////////////////////////
// - Querying
//////////////////////////////////////////////////////////////////////////

float GridAxisControls::getPreferredWidth() const
{
   float result = 0.0f;
   for ( Controls::const_iterator it = mControls.begin(); it != mControls.end(); it++ )
   {
      const GridControl& control = *(*it);
      if ( !control.isHorizontalSpanning() )
      {
         result = MAX(result, control.getPreferredWidth());
      }
   }

   return result;
}

float GridAxisControls::getPreferredHeight() const
{
   float result = 0.0f;
   for ( Controls::const_iterator it = mControls.begin(); it != mControls.end(); it++ )
   {
      const GridControl& control = *(*it);
      if ( !control.isVerticalSpanning() )
      {
         result = MAX(result, control.getPreferredHeight());
      }
   }
   return 0;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

GridControl&  GridAxisControls::operator[](int index)
{
   return *mControls[index];
}

bool GridAxisControls::contains(const GridControl& control) const
{
   return std::find(mControls.begin(), mControls.end(), &control) != mControls.end();
}

void GridAxisControls::add(GridControl& control)
{
   if ( !contains(control) )
   {
      mControls.push_back(&control);
   }
}
