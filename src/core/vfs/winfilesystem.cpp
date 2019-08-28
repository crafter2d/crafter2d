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
#include "winfilesystem.h"

#include <windows.h>

#include "core/defines.h"

#include "file.h"
#include "stdiofile.h"

WinFileSystem::WinFileSystem():
   FileSystem()
{
}

WinFileSystem::~WinFileSystem()
{
}

// - Query

UChar WinFileSystem::getSeparator() const
{
   return L'\\';
}

// - Operations

int WinFileSystem::mkdir(const String& path)
{
   if ( CreateDirectory(path.c_str(), nullptr) == FALSE )
   {
      DWORD error = GetLastError();
      switch ( error )
      {
         case ERROR_ALREADY_EXISTS:
            return ERR_PATH_EXISTS;
         case ERROR_PATH_NOT_FOUND:
            return ERR_PATH_NOT_FOUND;
      }
   }

   return NO_ERR;
}

bool WinFileSystem::copyFile(const String& from, const String& to)
{
  return SUCCEEDED(CopyFile2(from.c_str(), to.c_str(), nullptr));
}

bool WinFileSystem::moveFile(const String& from, const String& to)
{
   return MoveFileEx(from.c_str(), to.c_str(), MOVEFILE_REPLACE_EXISTING) == TRUE;
}

bool WinFileSystem::deleteFile(const String& filename)
{
   return DeleteFile(filename.c_str()) == TRUE;
}

bool WinFileSystem::recursiveFind(const FindInfo& findinfo, std::vector<String>& result)
{
   WIN32_FIND_DATA ffd;

   String mask = File::concat(findinfo.path, UTEXT("*"));
   HANDLE hFind = FindFirstFileEx(mask.c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, nullptr, 0);
   if ( hFind == INVALID_HANDLE_VALUE )
   {
      return true;
   }

   do
   {
      String name(ffd.cFileName);

      bool isdir = IS_SET(ffd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);
      if ( isdir )
      {
         if ( name != UTEXT(".") && name != UTEXT("..") )
         {
            FindInfo rfind;
            rfind.filemask = findinfo.filemask;
            rfind.path = File::concat(findinfo.path, name);

            find(rfind, result, true);
         }
      }
   }
   while ( FindNextFile(hFind, &ffd) != 0 );

   FindClose(hFind);

   return true;
}

bool WinFileSystem::find(const FindInfo& findinfo, std::vector<String>& result, bool recursive)
{
   WIN32_FIND_DATA ffd;

   String mask = File::concat(findinfo.path, findinfo.filemask);
   HANDLE hFind = FindFirstFileEx(mask.c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, nullptr, 0);
   if ( hFind != INVALID_HANDLE_VALUE )
   {
      do
      {
         String name(ffd.cFileName);

         if ( !IS_SET(ffd.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY) )
         {
            result.push_back(File::concat(findinfo.path, name));
         }     
      }
      while ( FindNextFile(hFind, &ffd) != 0 );

      DWORD dwError = GetLastError();

      FindClose(hFind);

      if (dwError != ERROR_NO_MORE_FILES) 
         return false;
   }

   return recursive ? recursiveFind(findinfo, result) : true;
}

bool WinFileSystem::find(const String& mask, std::vector<String>& result, bool recursive)
{
   FindInfo findinfo;
   findinfo.filemask = File::extractFileName(mask);
   findinfo.path = File::extractPath(mask);

   return find(findinfo, result, recursive);
}