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

/**
@author Jeroen Broekhuizen
\brief A templated hash map implementation.

@section hashmap_Introduction Introduction
A hash map can be used to quickly lookup an element based on a key value. It does so by generating
a hash value of the key. Based on the hash value it possibly can limit the number of key compares
(depending on the hashing algorithm).

@section hashmap_Hashfunction Hash Function
Use the setHashFunction method to set the hash function. This function is used by most of the
methods of the hash map. The function prototype should match the HashFnc declaration. It is called
to whenever a hash value is needed (e.g. during insertion). The resulting hash value of this method
should adhere to the following rules:

 * It should return always the same for value for a key
 * Multiple keys may result in the same hash value

The HashInterface namespace contains some implementations for default values that can be used.

@section hashmap_Usage Usage
Ensure that the hash function has already been set before going on with the following part.
Inserting a new key-element pair is done with the insert method. This implementation requires a key
to be unique. If the same key value is already stored in the map, it's element is overriden by the
new.

Retrieval of the object is done with the get method. You will need the key to lookup the element.
*/

template <typename K, class E>
class HashMap
{
public:
   typedef unsigned int(*HashFnc)(const K& key);

   HashMap(int size = 256);

 // get/set
   void setHashFunction(HashFnc hashfunc);

   HashMapIterator<K, E> getIterator();

 // query
   bool isEmpty() const;
   bool contains(const K& key) const;
   E* get(const K& key);

   int size() const;

 // operations
   void insert(const K& key, E& element);
   void remove(HashMapIterator<K, E>& it);
   void remove(const K& key);
   void clear();

 // searching
   HashMapIterator<K, E> find(const K& k);

private:
   friend class HashMapIterator<K,E>;

   HashFnc               mHashFnc;
   HashMapBucket<K, E>** mpBuckets;
   int                   mSize;
   int                   mCount;
};

#include "hashmap.inl"

#endif // HASHMAP_H
