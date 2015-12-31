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
#include "core/defines.h"

// - Get/set

template<class T>
ResourceHandle<T>::ResourceHandle(ResourceManager& manager, T* presource):
   ResourceHandleBase(manager),
   mpResource(presource)
{
}

template<class T>
ResourceHandle<T>::~ResourceHandle()
{
   release();
}

// - Get/set

template<class T>
bool ResourceHandle<T>::hasResource() const
{
   return mpResource != NULL;
}

template<class T>
const T& ResourceHandle<T>::getResource() const
{
   ASSERT_PTR(mpResource);
   return *mpResource;
}

template<class T>
T& ResourceHandle<T>::getResource()
{
   ASSERT_PTR(mpResource);
   return *mpResource;
}

template<class T>
void ResourceHandle<T>::setResource(T* presource)
{
   mpResource = presource;
}

// - Operations
   
template<class T>
void ResourceHandle<T>::release()
{
   ResourceHandleBase::release();

   delete mpResource;
   mpResource = NULL;
}

