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
#include "core/defines.h"

template<class T>
ObjectHandle<T>::ObjectHandle():
   mpAllocator(NULL),
   mpObject(NULL),
   mOwned(false)
{
}

template<class T>
ObjectHandle<T>::ObjectHandle(const ObjectHandle<T>& that):
   mpAllocator(that.mpAllocator),
   mpObject(that.mpObject),
   mOwned(true)
{
   that.mOwned = false;
}

template<class T>
ObjectHandle<T>::ObjectHandle(ObjectAllocator<T>& allocator):
   mpAllocator(&allocator),
   mpObject(allocator.get()),
   mOwned(true)
{
}

template<class T>
ObjectHandle<T>::~ObjectHandle()
{
   if ( mOwned )
   {
      ASSERT_PTR(mpAllocator);
      mpAllocator->release(mpObject);
   }
}

template<class T>
ObjectHandle<T>& ObjectHandle<T>::operator=(const ObjectHandle& that)
{
   mpAllocator = that.mpAllocator;
   mpObject = const_cast<ObjectHandle&>(that).release();
   mOwned = true;

   return *this;
}

// query

template<class T>
bool ObjectHandle<T>::hasObject() const
{
   return mpObject != NULL;
}

// - Operations

template<class T>
const T* ObjectHandle<T>::operator->() const
{
   return mpObject;
}

template<class T>
T* ObjectHandle<T>::operator->()
{
   return mpObject;
}

template<class T>
T& ObjectHandle<T>::operator*()
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

template<class T>
T* ObjectHandle<T>::ptr()
{
   return mpObject;
}

template<class T>
T* ObjectHandle<T>::release()
{
   mOwned = false;
   return mpObject;
}
