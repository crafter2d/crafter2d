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
#include "zipfile.h"

FileSystemPath::FileSystemPath(const String& path):
   mPath(),
   mZip()
{
   fillInfo(path);
}

FileSystemPath::~FileSystemPath()
{
}

void FileSystemPath::fillInfo(const String& path)
{
   int index = path.indexOf(UTEXT(".zip"));
   if ( index > 0 )
   {
      String zipfile = path.subStr(0, index + 4); // include the .zip extension
      if ( path.length() > index + 4 )
      {
         // skip the dir separator after the zip filename
         mPath = path.subStr(index + 5, path.length() - index - 5);
      }

      ASSERT(ZipFile::isZip(zipfile));
      mZip.reset(new ZipFile(zipfile));
   }
   else
   {
      mPath = path;
   }
}

//---------------------------------------
// - Get/set
//---------------------------------------

const String& FileSystemPath::getPath() const
{
   return mPath;
}

// - Query

bool FileSystemPath::isZipped() const
{
   return mZip.get() != nullptr;
}

bool FileSystemPath::exists(const String& filename) const
{
   if ( isZipped() )
   {
      return mZip->contains(File::concat(mPath, filename));
   }
   else
   {
      return StdioFile::exists(File::concat(mPath, filename));
   }
}

// - Operations

File* FileSystemPath::open(const String& filename, int modus) const
{
   File* presult = NULL;
   String file = File::concat(mPath, filename);

   if ( isZipped() )
   {
      ASSERT(IS_SET(modus, File::ERead)); // only read

      if ( mZip->contains(file) )
      {
         presult = new CompressedFile(*mZip);
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
   
bool FileSystemPath::operator==(const String& path) const
{
   return mPath == path;
}
