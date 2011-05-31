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
#ifndef SHARED_COUNT_H_
#define SHARED_COUNT_H_

#include "core/core_base.h"

class CORE_API SharedCount
{
public:
   SharedCount();
   SharedCount(const SharedCount& that);
   ~SharedCount();

   const SharedCount& operator=(const SharedCount& that);

 // query
   bool isUnique() const;

 // operations
   void reset();

private:
 // operations
   void inc();
   void dec();

   int*  mpCounter;
};

#include "sharedcount.inl"

#endif // SHARED_COUNT_H_
