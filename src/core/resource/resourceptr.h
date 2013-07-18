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
#ifndef RESOURCE_PTR_H_
#define RESOURCE_PTR_H_

#include "resourcehandle.h"

template <class T>
class ResourcePtr
{
public:
            ResourcePtr();
   explicit ResourcePtr(ResourceHandle<T>* phandle);
            ResourcePtr(const ResourcePtr<T>& that);
            ~ResourcePtr();

 // operators
   ResourcePtr<T>& operator=(const ResourcePtr<T>& that);
   ResourcePtr<T>& operator=(T* presource);

 // query
   bool isValid() const;
   
   const T* ptr() const;
         T* ptr();

   const String& getName() const;

 // dereferencing
   T* operator->() const;
   T& operator*() const;

private:

 // members
   ResourceHandle<T>* mpHandle;
};

#include "resourceptr.inl"

#endif // RESOURCE_PTR_H_