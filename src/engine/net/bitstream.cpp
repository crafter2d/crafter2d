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

#include "bitstream.h"
#ifndef JENGINE_INLINE
#  include "bitstream.inl"
#endif

#include <stdio.h>
#include <string.h>

#include "core/math/vector.h"

#include "netobject.h"
#include "netobjectfactory.h"

BitStream::BitStream():
   buf(NULL),
   pos(0),
   size(0),
   bufsize(1024)
{
   buf = new char[1024];
}

BitStream::BitStream(int reserve):
   buf(NULL),
   pos(0),
   size(0),
   bufsize(0)
{
}

BitStream::BitStream(const BitStream& that):
   buf(NULL),
   pos(0),
   size(that.size),
   bufsize(that.bufsize)
{
   buf = new char[bufsize];
   memcpy(buf, that.buf, size);
}

BitStream::~BitStream()
{
   try
   {
      delete[] buf;
   }
   catch (...) {}
}

void BitStream::ensureFits(int datasize)
{
   int newsize = size + datasize;
   if ( newsize >= bufsize )
   {
      do
      {
         bufsize *= 2;
      }
      while ( newsize >= bufsize );

      char* pnewbuf = new char[bufsize];
      memmove(pnewbuf, buf, size);

      delete[] buf;
      buf = pnewbuf;
   }
}

void BitStream::setBuffer(const char* data, int _size)
{
   ASSERT_MSG(data != NULL, "BitStream::setBuf : data pointer is null");

   ensureFits(_size);

   size = _size;
   memcpy(buf, data, size);
}

void BitStream::clear()
{
   pos = size = 0;
}

bool BitStream::empty()
{
   return (size == 0);
}

void BitStream::writeRaw(const char* data, int dataSize)
{
   ensureFits(dataSize);

   memcpy(&buf[size], data, dataSize);
   size += dataSize;
}

void BitStream::readRaw(char* data, int amount)
{
   memcpy(data, &buf[pos], amount);
   pos += amount;
}

BitStream& BitStream::operator<<(const Vector& v)
{
   int len = sizeof(float) * 2;
   ensureFits(len);

   memcpy(&(buf[size]), &v.x, len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(float f)
{
   int len = sizeof(float);
   ensureFits(len);

   memcpy(&buf[size], &f, len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(uint i)
{
   int len = sizeof(uint);
   ensureFits(len);

   memcpy(&buf[size], &i, len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(int i)
{
   int len = sizeof(int);
   ensureFits(len);

   memcpy(&buf[size], &i, len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(bool b)
{
   ensureFits(1);

   buf[size] = b;
   size++;
   return *this;
}

BitStream& BitStream::operator<<(char c)
{
   ensureFits(1);

   buf[size] = c;
   size++;
   return *this;
}

BitStream& BitStream::operator<<(const std::string& text)
{
   int len = text.length();
   ensureFits(len);

   *this << len;
   memcpy(&buf[size], text.c_str(), len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(const NetObject* obj)
{
   *this << obj->getRuntimeInfo().getId();
   obj->pack(*this);
   return *this;
}

BitStream& BitStream::operator<<(const BitStream* stream)
{
   int totalsize = stream->size + sizeof(int);
   ensureFits(totalsize);

   writeInt(stream->size);
   memcpy(&buf[size], stream->buf, stream->size);
   size += totalsize;
   return *this;
}

BitStream& BitStream::operator>>(Vector& v)
{
   int len = sizeof(float) * 2;
   memcpy(&v.x, &buf[pos], len);
   pos += sizeof(float) * 2;
   return *this;
}

BitStream& BitStream::operator>>(float& f)
{
   memcpy(&f, &buf[pos], sizeof(float));
   pos += sizeof(float);
   return *this;
}

BitStream& BitStream::operator>>(int& i)
{
   memcpy (&i, &buf[pos], sizeof(int));
   pos += sizeof(int);
   return *this;
}

BitStream& BitStream::operator>>(uint& i)
{
   memcpy (&i, &buf[pos], sizeof(uint));
   pos += sizeof(uint);
   return *this;
}

BitStream& BitStream::operator>>(bool& b)
{
   b = static_cast<bool>(buf[pos]);
   pos++;
   return *this;
}

BitStream& BitStream::operator>>(char& c)
{
   c = buf[pos];
   pos++;
   return *this;
}

BitStream& BitStream::operator>>(std::string& str)
{
  int len;
  *this >> len;
  str.assign(&buf[pos], len);
  pos += len;
  return *this;
}

BitStream& BitStream::operator>>(BitStream& stream)
{
   int streamsize = readInt();
   stream.writeRaw(&buf[pos], streamsize);
   pos += streamsize;

   return *this;
}

BitStream& BitStream::operator>>(NetObject** obj)
{
   int type;
   *this >> type;
   (*obj) = NetObjectFactory::getInstance().createObject(type);
   (*obj)->setReplica();
   (*obj)->unpack(*this);
   return *this;
}
