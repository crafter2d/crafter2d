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
#ifndef FILESYSTEM_PATHS_H_
#define FILESYSTEM_PATHS_H_

#include <vector>

#include "filesystempath.h"

class String;

class FileSystemPaths
{
public:
   FileSystemPaths();

   const FileSystemPath& operator[](int index) const;

 // query
   bool contains(const String& path) const;
   int  size() const;

 // operations
   void add(const String& path);
   void remove(const String& path);
   void removeAll();

private:
   typedef std::vector<FileSystemPath*> Paths;

   Paths::const_iterator find(const String& path) const;


   Paths mPaths;
};

#endif // FILESYSTEM_PATHS_H_
