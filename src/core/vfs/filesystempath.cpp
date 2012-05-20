/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "filesystempath.h"

#include "core/defines.h"

#include "compressedfile.h"
#include "stdiofile.h"
#include "unzipfile.h"
#include "zipfile.h"

FileSystemPath::FileSystemPath(const std::string& path):
   mPath(path),
   mpUnzip(NULL)
{
   fillInfo(path);
}

FileSystemPath::~FileSystemPath()
{
   delete mpUnzip;
   mpUnzip = NULL;
}

void FileSystemPath::fillInfo(const std::string& path)
{
   if ( UnzipFile::isZip(path) )
   {
      mpUnzip = new UnzipFile(path);
   }
}

//---------------------------------------
// - Get/set
//---------------------------------------

const std::string& FileSystemPath::getPath() const
{
   return mPath;
}

// - Query

bool FileSystemPath::isZipped() const
{
   return mpUnzip != NULL;
}

bool FileSystemPath::exists(const std::string& filename) const
{
   if ( isZipped() )
   {
      return mpUnzip->contains(filename);
   }
   else
   {
      return StdioFile::exists(File::concat(mPath, filename));
   }
}

// - Operations

File* FileSystemPath::open(const std::string& filename, int modus) const
{
   File* presult = NULL;
   std::string file = File::concat(mPath, filename);

   if ( isZipped() )
   {
      ASSERT(IS_SET(modus, File::ERead)); // only read

      if ( mpUnzip->contains(filename) )
      {
         presult = new CompressedFile();
      }
   }
   else
   {
      if ( IS_SET(modus, File::ERead) && !StdioFile::exists(file) )
      {
         // for read-mode the file must exist
         return NULL;
      }

      presult = new StdioFile();
   }

   if ( presult != NULL )
   {
      presult->open(file, modus);
   }

   return presult;
}

//---------------------------------------
// - Comparison
//---------------------------------------
   
bool FileSystemPath::operator==(const std::string& path) const
{
   return mPath == path;
}
