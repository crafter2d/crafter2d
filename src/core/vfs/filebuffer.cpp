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
#include "filebuffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/defines.h"

FileBuffer::FileBuffer():
   Buffer(),
   mpFile(NULL)
{
}

FileBuffer::FileBuffer(FILE* pfile):
   Buffer(),
   mpFile(pfile)
{
}

FileBuffer::~FileBuffer()
{
}

bool FileBuffer::isFileBuffer() const
{
   return true;
}

FileBuffer& FileBuffer::asFileBuffer()
{
   return *this;
}

int FileBuffer::read(void* ptr, int size)
{
   return fread(ptr, 1, size, mpFile);
}

int FileBuffer::write(void* ptr, int size)
{
   return fwrite(ptr, 1, size, mpFile);
}

int FileBuffer::size()
{
   int currentpos = ftell(mpFile);
   fseek(mpFile, 0, SEEK_END);
   int size = ftell(mpFile);
   fseek(mpFile, currentpos, SEEK_SET);

   return size;
}

char FileBuffer::getchar()
{
  return fgetc(mpFile);
}

char FileBuffer::peekchar()
{
   int pos = ftell(mpFile);
   char peek = getchar();
   if ( peek != EOF )
   {
      fseek(mpFile, pos, SEEK_SET);
   }
   return peek;
}

void FileBuffer::seek(int pos, int mode)
{
  fseek(mpFile, pos, mode);
}

int FileBuffer::tell() const
{
  return ftell(mpFile);
}

bool FileBuffer::eof() const
{
  return feof(mpFile) != 0;
}
