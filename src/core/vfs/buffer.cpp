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
#include "buffer.h"
#ifndef JENGINE_INLINE
#  include "buffer.inl"
#endif

#include <stdexcept>

#include "core/defines.h"

#include "filebuffer.h"
#include "memorybuffer.h"

Buffer* Buffer::fromFile(FILE* pfile)
{
   return new FileBuffer(pfile);
}

Buffer* Buffer::fromMemory(void* ptr, int size)
{
   return new MemoryBuffer(ptr, size);
}

Buffer::Buffer():
   mWritting(false)
{
}

Buffer::~Buffer()
{
}

bool Buffer::isFileBuffer() const
{
   return false;
}

FileBuffer& Buffer::asFileBuffer()
{
   throw std::exception();
}

bool Buffer::isMemoryBuffer() const
{
   return false;
}

MemoryBuffer& Buffer::asMemoryBuffer()
{
   throw std::exception();
}

int Buffer::size()
{
   PURE_VIRTUAL
   return -1;
}

char Buffer::getc()
{
  PURE_VIRTUAL
  return 0;
}

void Buffer::seek(int pos, int mode)
{
  PURE_VIRTUAL
}

int Buffer::tell() const
{
  PURE_VIRTUAL
  return 0;
}

bool Buffer::eof() const
{
  PURE_VIRTUAL
  return true;
}
