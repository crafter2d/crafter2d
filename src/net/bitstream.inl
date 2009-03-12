/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <assert.h>
#include "../defines.h"

INLINE void BitStream::setSize(int s)
{ 
   size = s;
}

INLINE void BitStream::setBuffer(const char* data, int _size)
{
   ASSERT_MSG(data != NULL, "BitStream::setBuf : data pointer is null");

   setSize(_size);
   ensureFits(0); 

   memcpy(buf, data, size);
}

INLINE void BitStream::reset()
{
   pos = 0;
}

INLINE int BitStream::getSize() const
{
   return size;
}

INLINE const char* BitStream::getBuf() const
{
   return buf;
}

INLINE int BitStream::getLen() const
{
   return pos;
}

INLINE bool BitStream::end() const
{ 
   return (pos >= size);
}

INLINE char* BitStream::readString()
{ 
   char buf[256]; 
   *this >> buf;
   return strdup(buf); 
}

INLINE int BitStream::readInt()
{
   int i;
   *this >> i;
   return i;
}

INLINE bool BitStream::readBool()
{
   bool b; 
   *this >> b;
   return b;
}

INLINE void BitStream::writeString(const std::string& s)
{
   *this << s.c_str();
}

INLINE void BitStream::writeString(const char* string)
{
   *this << string;
}

INLINE void BitStream::writeInt(int i)
{
   *this << i;
}

INLINE void BitStream::writeBool(bool b)
{ 
   *this << b;
}
