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
#ifndef MEMORY_BUFFER_H_
#define MEMORY_BUFFER_H_

#include "buffer.h"

#include "core/defines.h"

class MemoryBuffer : public Buffer
{
public:
            MemoryBuffer();
   explicit MemoryBuffer(void* pdata, int size);
   virtual ~MemoryBuffer();

 // get/set
           uint8_t*        getData();
           int             getDataSize();

 // query
   virtual bool            isMemoryBuffer() const;
   virtual MemoryBuffer&   asMemoryBuffer();

 // operations
   virtual int          read(void* ptr, int size);
   virtual int          write(const void* ptr, int size);
   virtual char         getchar();
   virtual char         peekchar();
   virtual void         seek(int pos, int mode);
   virtual int          tell() const;
   virtual bool         eof() const;

   virtual int          size();

private:
           void assign(void* pdata, int size);
           void free();

   uint8_t* mpData;
   int      mDataSize;
   int      mCursor;
};

#ifdef JENGINE_INLINE
#  include "memorybuffer.inl"
#endif

#endif // MEMORY_BUFFER_H_
