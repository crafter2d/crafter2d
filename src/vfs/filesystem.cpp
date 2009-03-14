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
#include "filesystem.h"

#ifdef _WIN32
#  include "winfilesystem.h"
#else
#  include "linuxfilesystem.h"
#endif

#include "compressedfile.h"
#include "stdiofile.h"

// static 
FileSystem& FileSystem::getInstance()
{
#ifdef _WIN32
   static WinFileSystem       filesystem;
#else
   static LinuxFileSystem  filesystem;
#endif

   return filesystem;
}

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

File* FileSystem::open(const std::string& filename)
{
   bool compressed = false;
   int index = 0;
   if ( filename.compare(0, 2, "..") == 0 )
   {
      index = 2;
   }
   
   index = filename.find('.', index);
   if ( index != filename.npos )
   {
      if ( filename.find('.', index + 1) != filename.npos )
         compressed = true;
   }

   File* pfile = NULL;
   if ( compressed )
      pfile = new CompressedFile();
   else
      pfile = new StdioFile();

   pfile->open(filename);

   return pfile;
}
