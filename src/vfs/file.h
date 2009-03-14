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
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <string>

class Buffer;

/**
   @author Jeroen Broekhuizen <jeroen@jengine.homedns.org>
*/
class File
{
public:
   enum Mode { ERead = 1,
               EWrite = 2,
               EBinary = 4,
               EText = 8 };

   File();
   virtual ~File();

   Buffer&  getBuffer();

   bool open(const std::string& filename, int modus = ERead | EBinary);
   void close();

   int read(void* ptr, int size);

   int size();

protected:
   void     setBuffer(Buffer* pbuffer);

   virtual bool   virOpen(const std::string& filename, int modus) = 0;
   virtual void   virClose();

private:
   Buffer*  mpBuffer;
};

#endif
