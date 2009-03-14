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
#include <string.h>

#include "netobject.h"
#include "../math/vector.h"

#include "netobjectfactory.h"

#include "bitstream.h"
#ifndef JENGINE_INLINE
#  include "bitstream.inl"
#endif

BitStream::BitStream(): 
   buf(NULL),
   pos(0), 
   size(0),
   bufsize(1024)
{
   buf = new char[1024];
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
      memset(pnewbuf, 0, bufsize);
      memmove(pnewbuf, buf, size);

      delete[] buf;
      buf = pnewbuf;
   }
}

void BitStream::clear()
{
   buf[0] = 0;
   pos = size = 0;
}

bool BitStream::empty()
{
   return (size == 0);
}

void BitStream::writeRaw(char* data, int dataSize)
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
   int len = sizeof(float)*3;
   ensureFits(len);

   memcpy(&(buf[size]), &v.x, len);
   size += len;
   return *this;
}

BitStream& BitStream::operator<<(const char* s)
{
   int len = strlen(s);
   ensureFits(len);

   *this << len;
   memcpy(&buf[size], s, len);
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

BitStream& BitStream::operator<<(Uint32 i)
{
   int len = sizeof(Uint32);
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
   return operator<<(text.c_str());
}

BitStream& BitStream::operator<<(const NetObject* obj)
{
   *this << obj->getRuntimeInfo()->getId();
   obj->pack(*this);
   return *this;
}

BitStream& BitStream::operator<<(const BitStream* stream)
{
   ensureFits(stream->size);

   memcpy(&buf[size], stream->buf, stream->size);
   size += stream->size;
   return *this;
}

BitStream& BitStream::operator>>(Vector& v)
{
   memcpy(&v.x, &buf[pos], sizeof(float)*3);
   pos += sizeof(float) * 3;
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

BitStream& BitStream::operator>>(Uint32& i)
{
   memcpy (&i, &buf[pos], sizeof(Uint32));
   pos += sizeof(Uint32);
   return *this;
}

BitStream& BitStream::operator>>(char* s)
{
   int len;
   *this >> len;
   memcpy (s, &buf[pos], len);
   s[len] = 0;
   pos += len;
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

BitStream& BitStream::operator>>(NetObject** obj)
{
   int type;
   *this >> type;
   (*obj) = NetObjectFactory::getInstance().createObject(type);
   (*obj)->unpack(*this);
   return *this;
}
