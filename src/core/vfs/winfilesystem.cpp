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

UChar WinFileSystem::getSeparator() const
{
   return L'\\';
}

bool WinFileSystem::recurseDirectory(const String& dir, Callback callback, void* pdata)
{
   String localdir = dir;
   if ( localdir[localdir.length() - 1] == '/' )
      localdir += '*';
   else
      localdir += L"/*";

   WIN32_FIND_DATA ffd;

   std::wstring local = localdir.toUtf16();
   HANDLE hFind = FindFirstFileEx(local.c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, NULL, 0);
   if ( hFind == INVALID_HANDLE_VALUE )
      return true;

   do
   {
      bool dir = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;

      String name(ffd.cFileName);
      callback(name, dir, pdata);
   }
   while ( FindNextFile(hFind, &ffd) != 0 );

   DWORD dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
      return false;

   return true;
}

bool WinFileSystem::recursiveFind(const FindInfo& findinfo, std::vector<String>& result)
{
   WIN32_FIND_DATA ffd;

   String newmask = File::concat(findinfo.path, UTEXT("*"));
   HANDLE hFind = FindFirstFileEx(newmask.toUtf16().c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, NULL, 0);
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
   HANDLE hFind = FindFirstFileEx(mask.toUtf16().c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, NULL, 0);
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