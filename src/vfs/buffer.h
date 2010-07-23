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
#ifndef BUFFER_H_
#define BUFFER_H_

#include <string>
#include <SDL/SDL.h>

class FileBuffer;
class MemoryBuffer;

class Buffer
{
public:
   static Buffer*   fromFile(FILE* pfile);
   static Buffer*   fromMemory(void* ptr, int size);

   Buffer();
   virtual ~Buffer();

 // get/set
   bool  isWritting() const;
   void  setWritting(bool writting);

   virtual SDL_RWops*      asRWops() = 0;

 // modification
   virtual int  read(void* ptr, int size) = 0;
   virtual int  write(void* ptr, int size) = 0;
   virtual char getc() = 0;
   virtual void seek(int pos, int mode) = 0;
   virtual int  tell() const = 0;
   virtual bool eof() const = 0;

   virtual int size() = 0;

 // casting
   virtual bool            isFileBuffer() const;
   virtual FileBuffer&     asFileBuffer();

   virtual bool            isMemoryBuffer() const;
   virtual MemoryBuffer&   asMemoryBuffer();

private:
   bool  mWritting;
};

#ifdef JENGINE_INLINE
#  include "buffer.inl"
#endif

#endif // BUFFER_H_
