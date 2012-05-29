/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "sortedpackagelist.h"

// - Statics

static
int Compare(const PackageHandle& left, const PackageHandle& right)
{
   if ( left->getNumber() < right->getNumber() )
   {
      return -1;
   }
   else if ( left->getNumber() > right->getNumber() )
   {
      return 1;
   }

   return 0;
}

// - Sorted Package List implementation

SortedPackageList::SortedPackageList():
   Impl()
{
   setCompareFnc((Impl::CompareFnc)Compare);
}

// - Operations

void SortedPackageList::removeOldPackages(uint number)
{
   ListIterator<PackageHandle> it = getFront();
   while ( it.isValid() )
   {
      PackageHandle& package = *it;
      if ( package->getNumber() < number )
      {
         it.remove();
      }
   }
}
