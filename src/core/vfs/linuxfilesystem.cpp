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
#include "core/defines.h"

LinuxFileSystem::LinuxFileSystem():
   FileSystem()
{
}

LinuxFileSystem::~LinuxFileSystem()
{
}

int LinuxFileSystem::mkdir(const String& path)
{
   C2D_UNUSED(path);
   return NO_ERR;
}

bool LinuxFileSystem::copyFile(const String& from, const String& to)
{
   C2D_UNUSED(from);
   C2D_UNUSED(to);
   return true;
}

UChar LinuxFileSystem::getSeparator() const
{
   return L'/';
}

bool LinuxFileSystem::doRecurseDirectory(const String& dir, const String& mask, std::vector<String>& result, bool recursive)
{
   std::string path = dir.toUtf8();
   std::string searchmask = mask.toUtf8();
   
   const char* pdir = path.c_str();
   const char* pmask = searchmask.c_str();

   dirent** pentries = NULL;
   int      count    = scandir(pdir, &pentries, NULL, alphasort);
   
   struct stat s;

   for ( int index = 0; index < count; index++ )
   {
      dirent& entry = *pentries[index];

      String path = dir + L'/' + String::fromUtf8(entry.d_name);
      stat(path.toUtf8().c_str(), &s);
      if ( S_ISDIR(s.st_mode) )
      {
         if ( recursive && (path != UTEXT(".") || path != UTEXT("..")) )
         {
            doRecurseDirectory(path, mask, result, recursive);
         }
      }
      else
      {
         if ( fnmatch(pmask, entry.d_name, FNM_PATHNAME) == 0 )
         {
            result.push_back(path);
         }
      }
   }

   return true;
}

bool LinuxFileSystem::find(const String& mask, std::vector<String>& result, bool recursive)
{
   String dir;
   String pattern;

   int index = mask.lastIndexOf('/');
   if ( index != -1 )
   {
      dir     = mask.subStr(0, index);
      pattern = mask.subStr(index+1, mask.length() - index - 1);
   }
   else
   {
      dir     = '.';
      pattern = mask;
   }
   
   return doRecurseDirectory(dir, pattern, result, recursive);
}
