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
#ifndef SORTED_PACKAGE_LIST_H
#define SORTED_PACKAGE_LIST_H

#include "core/memory/objecthandle.h"
#include "core/containers/sortedlist.h"

#include "netpackage.h"

class SortedPackageList : public SortedList< PackageHandle >
{
   typedef SortedList<PackageHandle> Impl;

public:
   SortedPackageList();

   void removeOldPackages(uint32_t number);
};

#endif // SORTED_PACKAGE_LIST_H
