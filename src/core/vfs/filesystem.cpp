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

#include <vector>

void tokenize(std::vector<String>& tokens, const String& str, char delimitor)
{
   std::size_t start = 0;
   std::size_t pos = str.indexOf(delimitor);

   while ( pos != String::npos )
   {
      String token = str.subStr(start, pos - start);
      tokens.push_back(token);

      start = pos + 1;
      pos = str.indexOf(delimitor, start);
   }


   String token = str.subStr(start, str.length() - start);
   tokens.push_back(token);
}

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

// static 
UChar FileSystem::getNativeSeparator()
{
   return getInstance().getSeparator();
}

// static
String FileSystem::toNativeSeparator(const String& path)
{
   UChar nativesep = getNativeSeparator();
   UChar search = L'/';
   if ( nativesep == L'/' )
   {
      search = L'\\';
   }

   String fixed(path);
   fixed.replace(search, nativesep);
   return fixed;
}

// - FileSystem implementation

FileSystem::FileSystem():
   mPaths()
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::addPath(const String& path)
{
   String fixedpath = toNativeSeparator(path);
   mPaths.add(fixedpath);
}

void FileSystem::removePath(const String& path)
{
   mPaths.remove(path);
}

void FileSystem::removeAll()
{
   mPaths.removeAll();
}

bool FileSystem::exists(const String& filename) const
{
   for ( int index = 0; index < mPaths.size(); index++ )
   {
      const FileSystemPath& path = mPaths[index];
      if ( path.exists(filename) )
      {
         return true;
      }
   }

   return false;
}

File* FileSystem::open(const String& filename, int modus) const
{
   for ( int index = 0; index < mPaths.size(); index++ )
   {
      const FileSystemPath& path = mPaths[index];
      File* pfile = path.open(filename, modus);
      if ( pfile != nullptr )
      {
         return pfile;
      }
   }

   return nullptr;
}

typedef std::vector<String> Tokens;

static void strip(Tokens& tokens)
{
   int count = 0;
   Tokens::iterator it = tokens.begin();
   while ( it != tokens.end() )
   {
      const String& token = *it;
      if ( token.compare(UTEXT("..")) == 0 )
      {
         count++;
      }
   }

   tokens.erase(tokens.begin(), tokens.begin() + count);
}

String FileSystem::expand(const String& path) const
{
   Tokens tokens;
   tokenize(tokens, path, '/');
   strip(tokens);

   Tokens::iterator it = tokens.begin();
   while ( it != tokens.end() )
   {
      //const String& token = (*it);
      Tokens::iterator next = it + 1;

      if ( next != tokens.end() )
      {
         const String& s = (*next);
         if ( s.compare(UTEXT("..")) == 0 )
         {
            tokens.erase(it,next);
            it = tokens.begin();
         }
      }
   }

   return UTEXT("");
}
