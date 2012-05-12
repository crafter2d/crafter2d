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
#include "bufferedstream.h"

#include "core/defines.h"

BufferedStream::BufferedStream():
   DataStream()
{
}

BufferedStream::BufferedStream(int reservesize):
   DataStream(),
   mpBuffer(NULL),
   mSize(0),
   mPos(0)
{
   reserve(reservesize);
}

BufferedStream::~BufferedStream()
{
   free(mpBuffer);
   mpBuffer = NULL;
}

// - Allocation

void BufferedStream::reserve(int size)
{
   if ( mSize < size )
   {
      realloc(mpBuffer, size);
   }
}

// - Query

int BufferedStream::size() const
{
   return mSize;
}

// - Operations

void BufferedStream::reset()
{
   mPos = 0;
}

// - Reading

void BufferedStream::readBytes(void* pbuffer, int amount)
{
   ASSERT(mSize > 0);
   ASSERT(mPos + amount < mSize);

   memcpy(pbuffer, &mpBuffer[mPos], amount);
   mPos += amount;
}

char BufferedStream::readByte()
{
   ASSERT(mSize > 0 && mPos < mSize);
   return mpBuffer[mPos++];
}

// - Writting

void BufferedStream::writeBytes(const void* pbuffer, int amount)
{
   if ( mSize <= mPos + amount )
   {
      reserve(mSize * 2);
   }
   memmove(&mpBuffer[mPos], pbuffer, amount);
   mPos += amount;
}