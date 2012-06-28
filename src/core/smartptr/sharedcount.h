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
#include "core/defines.h"

#include "countimpl.h"

class SharedCount
{
public:
   template<class T> SharedCount(T* pdata): mpCounter(NULL)
   {
      mpCounter = new CountImpl<T>(pdata);
   }

   SharedCount(const SharedCount& that): mpCounter(that.mpCounter)
   {
      if ( mpCounter != NULL )
      {
         mpCounter->addRef();
      }
   }

   ~SharedCount()
   {
      if ( mpCounter != NULL )
      {
         mpCounter->releaseRef();
      }
   }

   const SharedCount& operator=(const SharedCount& that)
   {
       if ( mpCounter != that.mpCounter )
      {
         mpCounter->releaseRef();
         mpCounter = that.mpCounter;
         mpCounter->addRef();
      }

      return *this;
   }

 // query
   bool isUnique() const
   {
      return count() == 1;
   }

   int count() const
   {
      return mpCounter == NULL ? 0 : mpCounter->useCount();
   }

 // operations

   void reset()
   {
      mpCounter->releaseRef();
      mpCounter = NULL;
   }


private:
   friend class WeakCount;

 // operations
   //void inc();
   //void dec();

   CountBase* mpCounter;
};

//#include "sharedcount.inl"

#endif // SHARED_COUNT_H_
