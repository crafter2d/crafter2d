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
#include "memorybuffer.h"
#ifndef JENGINE_INLINE
#  include "memorybuffer.inl"
#endif

#include <string.h>
#include <stdio.h>

MemoryBuffer::MemoryBuffer():
   Buffer(),
   mpData(NULL),
   mDataSize(0),
   mCursor(0)
{
}

MemoryBuffer::MemoryBuffer(void* pdata, int size):
   Buffer(),
   mpData(),
   mDataSize(0),
   mCursor(0)
{
   assign(pdata, size);
}

MemoryBuffer::~MemoryBuffer(void)
{
   free();
}

bool MemoryBuffer::isMemoryBuffer() const
{
   return true;
}

MemoryBuffer& MemoryBuffer::asMemoryBuffer()
{
   return *this;
}

void MemoryBuffer::assign(void* pdata, int size)
{
   delete[] mpData;
   mpData = new uchar[size];
   memcpy(mpData, pdata, size);
   mDataSize = size;
}

void MemoryBuffer::free()
{
   delete[] mpData;
   mpData    = NULL;
   mDataSize = 0;
   mCursor   = 0;
}

int MemoryBuffer::read(void* ptr, int size)
{
   if ( mCursor + size >= mDataSize )
      size = mDataSize - mCursor;

   if ( size < 0 )
      return 0;

   memcpy(ptr, &mpData[mCursor], size);
   mCursor += size;

   return size;
}

int MemoryBuffer::write(const void* ptr, int size)
{
   if ( !isWritting() )
      return 0;

   if ( mCursor + size >= mDataSize )
   {
      int buffersize = mCursor + size;
      uchar* pbuffer = new uchar[buffersize];
      memmove(pbuffer, mpData, mDataSize);

      delete[] mpData;
      mpData    = pbuffer;
      mDataSize = buffersize;
   }

   memcpy(&mpData[mCursor], ptr, size);
   mCursor += size;

   return size;
}

int MemoryBuffer::size()
{
   return mDataSize;
}

char MemoryBuffer::getchar()
{
  char data = mpData[mCursor];
  mCursor++;

  return data;
}

char MemoryBuffer::peekchar()
{
   return mpData[mCursor];
}

void MemoryBuffer::seek(int pos, int mode)
{
  switch ( mode )
  {
  case SEEK_CUR:
    mCursor += pos;
    break;
  case SEEK_SET:
    mCursor = pos;
    break;
  case SEEK_END:
    mCursor -= pos;
    break;
  }
}

int MemoryBuffer::tell() const
{
  return mCursor;
}

bool MemoryBuffer::eof() const
{
  return mCursor == mDataSize;
}
