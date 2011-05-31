/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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

#include "sharedcount.h"

SharedCount::SharedCount():
   mpCounter(new int(0))
{
   (*mpCounter) = 1;
}

SharedCount::SharedCount(const SharedCount& that):
   mpCounter(that.mpCounter)
{
   inc();
}

SharedCount::~SharedCount()
{
   dec();

   if ( (*mpCounter) == 0 )
   {
      delete mpCounter;
      mpCounter = NULL;
   }
}

const SharedCount& SharedCount::operator=(const SharedCount& that)
{
   mpCounter = that.mpCounter;
   inc();

   return *this;
}

// - Query
   
bool SharedCount::isUnique() const
{
   ASSERT_PTR(mpCounter);
   return (*mpCounter) == 1;
}

// - Operations

void SharedCount::reset()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter) = 0;
}

void SharedCount::inc()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter)++;
}

void SharedCount::dec()
{
   ASSERT_PTR(mpCounter);
   (*mpCounter)--;
}
