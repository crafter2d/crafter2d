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
#ifndef HASHMAP_H
#define HASHMAP_H

#include "core/defines.h"

template<typename, class> class HashMapBucket;
template<typename, class> class HashMapIterator;

template <typename K, class E>
class HashMap
{
public:
   typedef int(*HashFnc)(K key);

   HashMap(int size = 256);

 // get/set
   void setHashFunction(HashFnc hashfunc);

   HashMapIterator<K,E> getIterator();

 // query
   bool isEmpty() const;
   bool contains(K key) const;

 // operations
   void insert(K key, E& element);
   void remove(HashMapIterator<K,E>& it);
   void clear();

private:
   friend class HashMapIterator<K,E>;

   HashFnc              mHashFnc;
   HashMapBucket<K,E>** mpBuckets;
   int                  mSize;
   int                  mCount;
};

#include "hashmap.inl"

#endif // HASHMAP_H
