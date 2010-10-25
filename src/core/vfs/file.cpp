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

#include <SDL/SDL.h>

#include "../defines.h"

#include "buffer.h"

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

int File::read(void* ptr, int size)
{
   return getBuffer().read(ptr, size);
}

int File::size()
{
   return getBuffer().size();
}

bool File::virOpen(const std::string& filename, int modus)
{
   PURE_VIRTUAL
   return false;
}

void File::virClose()
{
   delete mpBuffer;
   mpBuffer = NULL;
}
