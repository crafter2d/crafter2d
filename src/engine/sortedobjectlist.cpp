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
#include "sortedobjectlist.h"
#ifndef JENGINE_INLINE
#  include "sortedobjectlist.inl"
#endif

#include "core/math/vector.h"

#include "actor.h"

//////////////////////////////////////////////////////////////////////////
// - Statics
//////////////////////////////////////////////////////////////////////////

static
int Compare(const Actor& left, const Actor& right)
{
   if ( left.getPosition().y < right.getPosition().y )
      return -1;
   else if ( left.getPosition().y >= right.getPosition().y )
   {
      // if Y coordinate is equal or bigger, we should render from left to right
      if ( left.getPosition().x < right.getPosition().x )
         return -1;
      else
         return 1;
   }

   return 0;
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

SortedObjectList::SortedObjectList():
   SortedList<Actor*>()
{
   setCompareFnc((SortedList<Actor*>::CompareFnc)Compare);
}
