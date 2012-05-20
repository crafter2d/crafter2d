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

#include "file.h"
#include "stdiofile.h"

WinFileSystem::WinFileSystem():
   FileSystem()
{
}

WinFileSystem::~WinFileSystem()
{
}

bool WinFileSystem::recurseDirectory(const std::string& dir, Callback callback, void* pdata)
{
   std::string localdir = dir;
   if ( localdir[localdir.length() - 1] == '/' )
      localdir += '*';
   else
      localdir += "/*";

   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   hFind = FindFirstFile(localdir.c_str(), &ffd);
   if ( hFind == INVALID_HANDLE_VALUE )
      return true;

   do
   {
      bool dir = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;

      callback(ffd.cFileName, dir, pdata);
   }
   while ( FindNextFile(hFind, &ffd) != 0 );

   DWORD dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
      return false;

   return true;
}

bool WinFileSystem::find(const std::string& mask, Callback callback, void* pdata)
{
   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;

   hFind = FindFirstFile(mask.c_str(), &ffd);
   if ( hFind == INVALID_HANDLE_VALUE )
      return true;

   do
   {
      callback(ffd.cFileName, false, pdata);
   }
   while ( FindNextFile(hFind, &ffd) != 0 );

   DWORD dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
      return false;

   return true;
}