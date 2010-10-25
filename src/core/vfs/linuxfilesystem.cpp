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

LinuxFileSystem::LinuxFileSystem():
   FileSystem()
{
}

LinuxFileSystem::~LinuxFileSystem()
{
}

bool LinuxFileSystem::doRecurseDirectory(const std::string& dir, const std::string& mask, Callback callback, void* pdata)
{
   dirent** pentries = NULL;
   int      count    = scandir(dir.c_str(), &pentries, 0, alphasort);

   struct stat s;

   for ( int index = 0; index < count; index++ )
   {
      dirent& entry = *pentries[index];

      if ( fnmatch(mask.c_str(), entry.d_name, FNM_PATHNAME) == 0 )
      {
         std::string file = dir + entry.d_name;
         stat(file.c_str(), &s);

         callback(entry.d_name, S_ISDIR(s.st_mode), pdata);
      }
   }

   return true;
}

bool LinuxFileSystem::recurseDirectory(const std::string& dir, Callback callback, void* pdata)
{
   std::string localdir = dir;
   if ( localdir[localdir.length() - 1] != '/' )
      localdir += '/';

   return doRecurseDirectory(localdir, "*.*", callback, pdata);
}

bool LinuxFileSystem::find(const std::string& mask, Callback callback, void* pdata)
{
   std::string dir;
   std::string pattern;

   std::string::size_type index = mask.rfind('/');
   if ( index != mask.npos )
   {
      dir     = mask.substr(0, index);
      pattern = mask.substr(index+1, mask.length() - index - 1);
   }
   else
   {
      dir     = '.';
      pattern = mask;
   }

   return doRecurseDirectory(dir, pattern, callback, pdata);
}
