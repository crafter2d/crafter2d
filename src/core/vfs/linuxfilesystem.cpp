/***************************************************************************
 *   Copyright (C) 2007 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "linuxfilesystem.h"

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fnmatch.h>

#include "core/string/string.h"

LinuxFileSystem::LinuxFileSystem():
   FileSystem()
{
}

LinuxFileSystem::~LinuxFileSystem()
{
}

bool LinuxFileSystem::doRecurseDirectory(const String& dir, const String& mask, Callback callback, void* pdata)
{
   dirent** pentries = NULL;
   int      count    = scandir(dir.getBuffer(), &pentries, 0, alphasort);

   struct stat s;

   for ( int index = 0; index < count; index++ )
   {
      dirent& entry = *pentries[index];

      if ( fnmatch(mask.getBuffer(), entry.d_name, FNM_PATHNAME) == 0 )
      {
         String file = dir + entry.d_name;
         stat(file.getBuffer(), &s);

         callback(entry.d_name, S_ISDIR(s.st_mode), pdata);
      }
   }

   return true;
}

bool LinuxFileSystem::recurseDirectory(const String& dir, Callback callback, void* pdata)
{
   String localdir = dir;
   if ( localdir[localdir.length() - 1] != '/' )
      localdir += '/';

   return doRecurseDirectory(localdir, "*.*", callback, pdata);
}

bool LinuxFileSystem::find(const String& mask, Callback callback, void* pdata)
{
   String dir;
   String pattern;

   String::size_type index = mask.lastIndexOf('/');
   if ( index != mask.npos )
   {
      dir     = mask.subStr(0, index);
      pattern = mask.subStr(index+1, mask.length() - index - 1);
   }
   else
   {
      dir     = '.';
      pattern = mask;
   }

   return doRecurseDirectory(dir, pattern, callback, pdata);
}
