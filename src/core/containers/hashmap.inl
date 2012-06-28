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
#include "hashmapbucket.h"

template <typename K, class E>
HashMap<K,E>::HashMap(int size):
   mHashFnc(NULL),
   mpBuckets(NULL),
   mSize(size)
{
   mpBuckets = new HashMapBucket<K,E>*[size];
   for ( int index = 0; index < size; index++ )
   {
      mpBuckets[index] = NULL;
   }
}

// - Get/set

template <typename K, class E>
void HashMap<K,E>::setHashFunction(HashFnc hashfunc)
{
   mHashFnc = hashfunc;
}

template <typename K, class E>
HashMapIterator<K,E> HashMap<K,E>::getIterator()
{
   return HashMapIterator<K,E>(*this);
}

// - Query

template <typename K, class E>
bool HashMap<K,E>::contains(K key) const
{
   unsigned int hash = (*mHashFnc)(key) % mSize;
   if ( mpBuckets[hash] != NULL )
   {
      HashMapBucket<K,E>* pbucket = mpBuckets[hash];
      for ( ; pbucket != NULL; pbucket = pbucket->getNext() )
      {
         if ( pbucket->getKey() == key )
         {
            return true;
         }
      }
   }

   return false;
}

// - Operations

template <typename K, class E>
void HashMap<K,E>::insert(K key, E& element)
{
   unsigned int hash = (*mHashFnc)(key) % mSize;
   if ( mpBuckets[hash] == NULL )
   {
      // first element
      HashMapBucket<K,E>* pbucket = new HashMapBucket<K,E>(key, element);
      mpBuckets[hash] = pbucket;
   }
   else
   {
      // look if something is already there
      HashMapBucket<K,E>* pbucket = mpBuckets[hash];
      for ( ; pbucket != NULL; pbucket = pbucket->getNext() )
      {
         if ( pbucket->getKey() == key )
         {
            pbucket->setElement(element);
            return;
         }
      }

      pbucket = new HashMapBucket<K,E>(key, element);
      pbucket->setNext(mpBuckets[hash]);
      mpBuckets[hash] = pbucket;
   }
}

template <typename K, class E>
void HashMap<K,E>::remove(HashMapIterator<K,E>& it)
{
}

template <typename K, class E>
void HashMap<K,E>::clear()
{
   for ( int index = 0; index < mSize; index++ )
   {
      if ( mpBuckets[index] != NULL )
      {
         HashMapBucket<K,E>* pbucket = mpBuckets[index];
         while ( pbucket != NULL )
         {
            HashMapBucket<K,E>* pnext = pbucket->getNext();
            delete pbucket;
            pbucket = pnext;
         }

         mpBuckets[index] = NULL;
      }
   }
}
