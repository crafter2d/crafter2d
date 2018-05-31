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

template <typename K, class E>
HashMapIterator<K,E>::HashMapIterator(HashMap<K,E>& hashmap):
   mpHashMap(&hashmap),
   mpBucket(nullptr),
   mIndex(-1)
{
   initialize();
}

template <typename K, class E>
HashMapIterator<K,E>& HashMapIterator<K,E>::operator=(const HashMapIterator<K,E>& that)
{
   mpHashMap = that.mpHashMap;
   mpBucket  = that.mpBucket;
   mIndex    = that.mIndex;
   return *this;
}

// - Operations

template <typename K, class E>
void HashMapIterator<K,E>::initialize()
{
   for ( mIndex = 0; mIndex < mpHashMap->mSize; ++mIndex )
   {
      if ( mpHashMap->mpBuckets[mIndex] != nullptr )
      {
         mpBucket = mpHashMap->mpBuckets[mIndex];
         break;
      }
   }
}

// - Overloads

template <typename K, class E>
void HashMapIterator<K,E>::operator++()
{
   if ( isValid() )
   {
      mpBucket = mpBucket->getNext();
      if ( mpBucket == nullptr )
      {
         for ( ++mIndex; mIndex < mpHashMap->mSize; ++mIndex )
         {
            if ( mpHashMap->mpBuckets[mIndex] != nullptr )
            {
               mpBucket = mpHashMap->mpBuckets[mIndex];
               break;
            }
         }
      }
   }
}

template <typename K, class E>
void* HashMapIterator<K,E>::key()
{
   return mpBucket->getKey();
}

template <typename K, class E>
void HashMapIterator<K,E>::reset()
{
   initialize();
}

template <typename K, class E>
E& HashMapIterator<K,E>::item()
{
   return mpBucket->getElement();
}

template <typename K, class E>
void HashMapIterator<K,E>::remove()
{
   HashMapIterator<K,E> current(*this);
   operator++();
   mpHashMap->remove(current);
}

template <typename K, class E>
bool HashMapIterator<K,E>::isValid() const
{
   return mpBucket != nullptr;
}
