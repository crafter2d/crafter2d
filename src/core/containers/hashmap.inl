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
   mHashFnc(nullptr),
   mpBuckets(nullptr),
   mSize(size),
   mCount(0)
{
   mpBuckets = new HashMapBucket<K,E>*[size];
   for ( int index = 0; index < size; index++ )
   {
      mpBuckets[index] = nullptr;
   }
}

template <typename K, class E>
HashMap<K,E>::~HashMap()
{
   clear();

   delete[] mpBuckets;
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
bool HashMap<K,E>::isEmpty() const
{
   return mCount == 0;
}

template <typename K, class E>
bool HashMap<K,E>::contains(const K& key) const
{
   ASSERT_PTR(mHashFnc);
   unsigned int hash = (*mHashFnc)(key) % mSize;
   if ( mpBuckets[hash] != nullptr )
   {
      HashMapBucket<K,E>* pbucket = mpBuckets[hash];
      for ( ; pbucket != nullptr; pbucket = pbucket->getNext() )
      {
         if ( pbucket->getKey() == key )
         {
            return true;
         }
      }
   }

   return false;
}

template <typename K, class E>
E* HashMap<K,E>::get(const K& key)
{
   E* presult = nullptr;
   ASSERT_PTR(mHashFnc);
   unsigned int hash = (*mHashFnc)(key) % mSize;
   HashMapBucket<K,E>* pbucket = mpBuckets[hash];
   if ( pbucket != nullptr )
   {
      if ( pbucket->hasNext() )
      {
         for ( ; pbucket != nullptr; pbucket = pbucket->getNext() )
         {
            if ( pbucket->getKey() == key )
            {
               presult = & pbucket->getElement();
               break;
            }
         }
      }
      else
      {
         presult = & pbucket->getElement();
      }
   }
   
   return presult;
}

template <typename K, class E>
int HashMap<K,E>::size() const
{
   return mCount;
}

// - Operations

template <typename K, class E>
void HashMap<K,E>::insert(const K& key, E& element)
{
   unsigned int hash = (*mHashFnc)(key) % mSize;
   if ( mpBuckets[hash] == nullptr )
   {
      // first element
      HashMapBucket<K,E>* pbucket = new HashMapBucket<K,E>(key, element);
      mpBuckets[hash] = pbucket;
   }
   else
   {
      // look if something is already there
      HashMapBucket<K,E>* pbucket = mpBuckets[hash];
      for ( ; pbucket != nullptr; pbucket = pbucket->getNext() )
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

   mCount++;
}

template <typename K, class E>
void HashMap<K,E>::remove(const K& key)
{
   unsigned int hash = (*mHashFnc)(key) % mSize;
   HashMapBucket<K,E>* pbucket = mpBuckets[hash];
   if ( pbucket->getNext() != nullptr )
   {
      for ( HashMapBucket<K,E>* pprev = nullptr; pbucket != nullptr; pprev = pbucket, pbucket = pbucket->getNext() )
      {
         if ( pbucket->getKey() == key )
         {
            if ( pprev != nullptr )
            {
               pprev->setNext(pbucket->getNext());
            }
            else
            {
               mpBuckets[hash] = pbucket->getNext();
            }

            delete pbucket;
            return;
         }
      }
   }
   else
   {
      mpBuckets[hash] = nullptr;
      delete pbucket;
   }

   mCount--;
}

template <typename K, class E>
void HashMap<K,E>::remove(HashMapIterator<K,E>& it)
{
   remove(it.mpBucket->getKey());
   /*
   unsigned int hash = (*mHashFnc)(it.mpBucket->getKey()) % mSize;
   HashMapBucket<K,E>* pbucket = mpBuckets[hash];
   if ( pbucket->getNext() != nullptr )
   {
      for ( HashMapBucket<K,E>* pprev = nullptr; pbucket != nullptr; pprev = pbucket, pbucket = pbucket->getNext() )
      {
         if ( pbucket == it.mpBucket )
         {
            if ( pprev != nullptr )
            {
               pprev->setNext(pbucket->getNext());
            }
            else
            {
               mpBuckets[hash] = pbucket->getNext();
            }

            delete pbucket;
            return;
         }
      }
   }
   else
   {
      ASSERT(pbucket == it.mpBucket);
      mpBuckets[hash] = nullptr;
      delete pbucket;
   }

   mCount--;
   */
}

template <typename K, class E>
void HashMap<K,E>::clear()
{
   for ( int index = 0; index < mSize; index++ )
   {
      HashMapBucket<K,E>* pbucket = mpBuckets[index];
      while ( pbucket != nullptr )
      {
         HashMapBucket<K,E>* pnext = pbucket->getNext();
         delete pbucket;
         pbucket = pnext;
      }

      mpBuckets[index] = nullptr;
   }
}
