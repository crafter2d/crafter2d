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
#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#include <queue>

template<class T>
class MemoryPool
{
   typedef std::queue<T*> Pool;

public:
   MemoryPool(): mPool() {}
   ~MemoryPool() {
      while ( !mPool.empty() )
      {
         T* pitem = mPool.front();
         mPool.pop();
         delete pitem;
      }
   }

   T* alloc() {
      if ( mPool.empty() )
      {
         return new T();
      }
      else
      {
         T* pitem = mPool.front();
         mPool.pop();
         return pitem;
      }
   }

   void free(T* pitem) {
      mPool.push(pitem);
   }

private:
   Pool mPool;
};

#endif // MEMORY_POOL_H_
