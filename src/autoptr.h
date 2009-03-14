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
#ifndef AUTO_PTR_H_
#define AUTO_PTR_H_

/**
@author Jeroen Broekhuizen
\brief The AutoPtr class can be used to store pointers that will be freed automatically
when the class goes out of scope.
*/

template<class T>
class AutoPtr
{
public:
   typedef AutoPtr<T> autoptr_type;

   AutoPtr(T* pvalue);
   AutoPtr(const AutoPtr<T>& ptr);
   ~AutoPtr();

   AutoPtr& operator=(const AutoPtr<T>& ptr);
   AutoPtr& operator=(T* pvalue);

   T* operator->() const;
   T& operator*() const;

   bool  hasPointer() const;
   T*    getPointer() const;

   T*    release();

private:
   T*    mpValue;
   bool  mOwned;
};

#include "autoptr.inl"

#endif // AUTO_PTR_H_
