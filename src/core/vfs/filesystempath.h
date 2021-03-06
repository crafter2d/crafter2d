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
#ifndef FILESYSTEM_PATH_H_
#define FILESYSTEM_PATH_H_

#include <memory>

#include "core/string/string.h"

class File;
class ZipFile;

class FileSystemPath
{
public:
   explicit FileSystemPath(const String& path);
           ~FileSystemPath();

 // get/set
   const String& getPath() const;

 // query
   bool isZipped() const;
   bool exists(const String& filename) const;

 // operations
   std::unique_ptr<File> open(const String& filename, int modus) const;

 // comparison
   bool operator==(const String& path) const;

private:
 // initialization
   void fillInfo(const String& path);

   String   mPath;
   std::unique_ptr<ZipFile> mZip;
};

#endif // FILESYSTEM_PATH_H_
