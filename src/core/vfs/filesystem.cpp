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

#include "compressedfile.h"
#include "stdiofile.h"

void tokenize(std::vector<std::string>& tokens, const std::string& str, char delimitor)
{
   std::size_t start = 0;
   std::size_t pos = str.find(delimitor);

   while ( pos != std::string::npos )
   {
      std::string token = str.substr(start, pos - start);
      tokens.push_back(token);

      start = pos + 1;
      pos = str.find(delimitor, start);
   }
   

   std::string token = str.substr(start, str.size() - start);
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

FileSystem::FileSystem():
   mPaths()
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::addPath(const std::string& path)
{
   mPaths.add(path);
}

void FileSystem::removePath(const std::string& path)
{
   mPaths.remove(path);
}

bool FileSystem::exists(const std::string& filename) const
{
   for ( int index = 0; index < mPaths.size(); index++ )
   {
      const std::string& path = mPaths[index];
      std::string pathfile = path + '/' + filename;

      if ( StdioFile::exists(pathfile) )
      {
         return true;
      }
   }

   return false;
}

File* FileSystem::open(const std::string& filename, int modus) const
{
   std::string file = filename; // expand(filename);

   for ( int index = 0; index < mPaths.size(); index++ )
   {
      const std::string& path = mPaths[index];

      File* pfile = tryOpen(path, file, modus);
      if ( pfile != NULL )
      {
         return pfile;
      }
   }

   return NULL;
}

File* FileSystem::tryOpen(const std::string& path, const std::string& file, int modus) const
{
   File* pfile = NULL;

   std::string pathfile = path + '/' + file;

   if ( CompressedFile::isCompressedFile(path) )
   {
      if ( CompressedFile::exists(path, file) )
      {
         pfile = new CompressedFile();
      }
   }
   else if ( StdioFile::exists(pathfile) )
   {
      pfile = new StdioFile();
   }

   if ( pfile != NULL )
   {
      pfile->open(pathfile, modus);
   }

   return pfile;
}

typedef std::vector<std::string> Tokens;

static void strip(Tokens& tokens)
{
   int count = 0;
   Tokens::iterator it = tokens.begin();
   while ( it != tokens.end() )
   {
      const std::string& token = *it;
      if ( token.compare("..") == 0 )
      {
         count++;
      }
   }

   tokens.erase(tokens.begin(), tokens.begin() + count);
}

std::string FileSystem::expand(const std::string& path) const
{
   Tokens tokens;
   tokenize(tokens, path, '/');
   strip(tokens);
   
   Tokens::iterator it = tokens.begin();
   while ( it != tokens.end() )
   {
      const std::string& token = (*it);
      Tokens::iterator next = it + 1;

      if ( next != tokens.end() )
      {
         const std::string& s = (*next);
         if ( s.compare("..") == 0 )
         {
            tokens.erase(it,next);
            it = tokens.begin();
         }
      }
   }
   
   return "";
}
