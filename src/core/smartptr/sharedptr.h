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
#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include "core/defines.h"

#include "sharedcount.h"

template<class E> class WeakPtr;

template<class T>
class SharedPtr
{
public:
   explicit SharedPtr(T* pointer = NULL);
   explicit SharedPtr(WeakPtr<T>& that);
            SharedPtr(const SharedPtr<T>& that);
           ~SharedPtr();

   const SharedPtr<T>& operator=(const SharedPtr<T>& that);

 // pointer access
   const T& operator*() const;
         T& operator*();

   bool     hasPointer() const;
   const T* getPointer() const;
         T* getPointer();

 // query
   bool isUnique() const;
   int useCount() const;

 // operations
   void reset();

private:
   template<class E> friend class WeakPtr;

 // operations
   void inc();
   void dec();

   CountBase*  mpCount;
   T*          mpPointer;
};

#include "sharedptr.inl"

#endif // SHARED_PTR_H_
