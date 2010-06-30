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
#include <assert.h>
#include "../defines.h"

INLINE void BitStream::setBuffer(const char* data, int _size)
{
   ASSERT_MSG(data != NULL, "BitStream::setBuf : data pointer is null");

   ensureFits(_size);

   size = _size;
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

INLINE std::string BitStream::readString()
{ 
  std::string str;
   *this >> str;
   return str; 
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
   *this << s;
}

INLINE void BitStream::writeInt(int i)
{
   *this << i;
}

INLINE void BitStream::writeBool(bool b)
{ 
   *this << b;
}
