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
#include "file.h"

#include "core/defines.h"

#include "buffer.h"

// static 
std::string File::concat(const std::string& path, const std::string& filename)
{
   bool has = path[path.length()-1] == '/' || path[path.length()-1] == '\\'
           || filename[0] == '/' || filename[0] == '\\';
   if ( has )
   {
      return path + filename;
   }
   return path + '/' + filename;
}

// static
std::string File::extractPath(const std::string& filepath)
{
   std::size_t pos = filepath.rfind('/');
   if ( pos == std::string::npos )
   {
      pos = filepath.rfind('\\');
   }

   if ( pos != std::string::npos )
   {
      return filepath.substr(0, pos);
   }

   return "";
}

File::File():
   mpBuffer(NULL)
{
}

File::~File()
{
}

Buffer& File::getBuffer()
{
   ASSERT_PTR(mpBuffer)
   return *mpBuffer;
}

void File::setBuffer(Buffer* pbuffer)
{
   delete mpBuffer;
   mpBuffer = pbuffer;
}

bool File::open(const std::string& filename, int modus)
{
   if ( virOpen(filename, modus) )
   {
      getBuffer().setWritting(!IS_SET(modus, File::ERead));

      return true;
   }

   return false;
}

void File::close()
{
   virClose();
}

// - Reading & writting

int File::read(void* ptr, int size)
{
   return getBuffer().read(ptr, size);
}

int File::write(void* ptr, int size)
{
   return getBuffer().write(ptr, size);
}

int File::write(const std::string& text)
{
   return getBuffer().write((void*)text.c_str(), text.length());
}

char File::getc()
{
   return getBuffer().getchar();
}

// Search & positioning

void File::seek(int pos, int mode)
{
   return getBuffer().seek(pos, mode);
}

int File::tell() const
{
   return mpBuffer->tell();
}

bool File::eof() const
{
   return mpBuffer != NULL ? mpBuffer->eof() : true;
}

// - Query

int File::size()
{
   return getBuffer().size();
}

bool File::virOpen(const std::string& , int)
{
   PURE_VIRTUAL
   return false;
}

void File::virClose()
{
   delete mpBuffer;
   mpBuffer = NULL;
}
