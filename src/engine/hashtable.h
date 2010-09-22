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
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <string>

/*
** Hash table implementation, with thanks to:
** By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
** code any way you wish, private, educational, or commercial.  It's free.
*/

/*
** A hash table consists of an array of these buckets. Each bucket
** holds a copy of the key, a pointer to the data associated with the
** key, and a pointer to the next bucket that collided with this one,
** if there was one.
*/
struct bucket
{
   const std::string key;
	void* data;
	bucket* next;

   bucket(const std::string& k, void* d): key(k), data(d), next(0) {}
};

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k     : the key (the unaligned variable-length array of bytes)
  len   : the length of the key, counting by bytes
  level : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 36+6len instructions.
*/
typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;

class HashTable
{
public:
	         HashTable();
	         ~HashTable();

	bool     create(ub4 s);

	void*    insert(const std::string& key, void* data);
	void*    lookup(const std::string& key);
	void*    remove(const std::string& key);

   void     enumerate(void (*func)(const std::string&, void *, void *), void* param);

private:
	ub4      hash(const std::string& key, register ub4 initval = 37);

	ub4 size;
	bucket** table;
};

#endif
