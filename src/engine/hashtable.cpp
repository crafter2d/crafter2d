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
//**************************************
// Name: Hash table functions
// Description:Hash table management by Jerry Coffin, with improvements by HenkJan Wolthuis.
// By: Bob Stout (republished under Open Content License)
// Ingmar Stieger: Added better hash function from Bob Jenkins (http://burtleburtle.net/bob/hash/)
//
// Inputs:None
//
// Returns:None
//
//Assumes:None
//
//Side Effects:None
//This code is copyrighted and has limited warranties.
//Please see http://www.Planet-Source-Code.com/xq/ASP/txtCodeId.710/lngWId.3/qx/vb/scripts/ShowCode.htm
//for details.
//**************************************

#include "hashtable.h"
#include <string.h>

HashTable::HashTable():
   size(0),
   table(0)
{
}

HashTable::~HashTable()
{
}

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k     : the key (the unaligned variable-length array of bytes)
  level : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 36+6len instructions.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (ub1 **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);

By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

See http://burlteburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

ub4 HashTable::hash(const std::string& key, register ub4 initval)
{
   const char* pkey = key.c_str();
   const register ub1 *k = (ub1*)pkey;
   register ub4 a,b,c,len;

   /* Set up the internal state */
   len = strlen((char*)k);
   a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
   c = initval;           /* the previous hash value */

   /*---------------------------------------- handle most of the key */
   while (len >= 12)
   {
      a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
      b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
      c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
      mix(a,b,c);
      k += 12; len -= 12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c += len;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c+=((ub4)k[10]<<24);
   case 10: c+=((ub4)k[9]<<16);
   case 9 : c+=((ub4)k[8]<<8);
      /* the first byte of c is reserved for the length */
   case 8 : b+=((ub4)k[7]<<24);
   case 7 : b+=((ub4)k[6]<<16);
   case 6 : b+=((ub4)k[5]<<8);
   case 5 : b+=k[4];
   case 4 : a+=((ub4)k[3]<<24);
   case 3 : a+=((ub4)k[2]<<16);
   case 2 : a+=((ub4)k[1]<<8);
   case 1 : a+=k[0];
     /* case 0: nothing left to add */
   }
   mix(a,b,c);
   /*-------------------------------------------- report the result */
   return c;
}

bool HashTable::create (ub4 s)
{
	table = new bucket*[s];
	if (table == nullptr)
		return false;

	for (ub4 i = 0; i < s; i++)
		table[i] = nullptr;
	size = s;
	return true;
}

void* HashTable::insert (const std::string& key, void* data)
{
	ub4 k = hash(key) % size;
	bucket* b = new bucket(key, data);

	if (table[k] == nullptr)
   {
		// just insert here
		table[k] = b;
	}
	else
   {
		// see if it is already there
		bucket* temp = table[k];
		for (; temp; temp = temp->next)
         if ( temp->key.compare(key) == 0)
         {
				void* old = temp->data;
				temp->data = data;
				return old;
			}

		// not there yet (otherwise returned)
		b->next = table[k];
		table[k] = b;
	}
	return nullptr;
}

void* HashTable::lookup (const std::string& key)
{
	if (size == 0) return nullptr;
	ub4 k = hash (key) % size;
	if (!table[k]) return nullptr;

	if (table[k]->next) {
		// multiple items on this spot, so do string compared
		// to find the correct item
		bucket* temp = table[k];
		for (; temp; temp = temp->next)
			if ( temp->key.compare(key) == 0 )
				return temp->data;

		// when we arrive here it wasn't there
		return nullptr;
	}
	else {
		// only one item on this spot, return it's data
      bucket* temp = table[k];
      if ( temp->key.compare(key) == 0 )
         return temp->data;
	}

   return nullptr;
}

void HashTable::enumerate(void (*func)(const std::string&, void *, void *), void* param)
{
   unsigned i;
    bucket *temp;
    for (i=0; i < size; i++)
        if ((table)[i] != nullptr)
            for (temp = (table)[i]; temp != nullptr; temp = temp->next)
                func(temp->key, temp->data, param);
}

void* HashTable::remove (const std::string& key)
{
	ub4 k = hash (key) % size;
	if (!table[k]) return nullptr;

	if (table[k]->next)
   {
		// multiple entries, remove correct one
		bucket* temp = table[k];
		for (bucket* prev = nullptr; temp; prev = temp, temp = temp->next)
         if ( temp->key.compare(key) == 0)
         {
				void* old = temp->data;
				if (prev)
					prev->next = temp->next;
				else
					table[k]->next = temp->next;
				delete temp;
				return old;
			}
		return nullptr;
	}
	else 
   {
		// remove the only object
		void *old = table[k]->data;
		delete table[k];
		table[k] = nullptr;
		return old;
	}
}
