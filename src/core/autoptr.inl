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
#include "core/defines.h"

template<class T>
INLINE AutoPtr<T>::AutoPtr(T* pvalue):
   mpValue(pvalue),
   mOwned(true)
{
}

template<class T>
INLINE AutoPtr<T>::AutoPtr(const AutoPtr<T>& ptr):
   mpValue(ptr.mpValue),
   mOwned(true)
{
   operator=(ptr);
}

template<class T>
INLINE AutoPtr<T>::~AutoPtr()
{
   if ( mOwned )
   {
      delete mpValue;
      mpValue = NULL;
   }
}

template<class T>
INLINE AutoPtr<T>& AutoPtr<T>::operator=(const AutoPtr<T>& ptr)
{
   operator=(ptr.mpValue);

   AutoPtr<T>& nonconstptr = const_cast<autoptr_type&>(ptr);
   nonconstptr.mOwned = false;

   return *this;
}

template<class T>
INLINE AutoPtr<T>& AutoPtr<T>::operator=(T* pvalue)
{
   delete mpValue;
   mpValue = pvalue;
   return *this;
}

template<class T>
INLINE T* AutoPtr<T>::operator->() const
{
   return mpValue;
}

template<class T>
INLINE T& AutoPtr<T>::operator*() const
{
   ASSERT_PTR(mpValue);
   return *mpValue;
}

template<class T>
INLINE bool AutoPtr<T>::hasPointer() const
{
   return mpValue != NULL;
}

template<class T>
INLINE T* AutoPtr<T>::getPointer() const
{
   return mpValue;
}

template<class T>
INLINE T* AutoPtr<T>::release()
{
   mOwned = false;
   return mpValue;
}
