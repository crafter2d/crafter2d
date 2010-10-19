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

bool FileSystemPath::hasUnzip() const
{
   return mpUnzip != NULL;
}

const UnzipFile& FileSystemPath::getUnzip() const
{
   return *mpUnzip;
}

//---------------------------------------
// - Comparison
//---------------------------------------
   
bool FileSystemPath::operator==(const std::string& path) const
{
   return mPath == path;
}
