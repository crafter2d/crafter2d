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
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <vector>

#include "core/core_base.h"

#include "filesystempaths.h"

class File;
class String;

enum Errors
{
   NO_ERR,
   ERR_PATH_EXISTS,
   ERR_PATH_NOT_FOUND,
};

/// \brief Abstract base class of the virtual file system.

class CORE_API FileSystem
{
public:
   static FileSystem&   getInstance();
   static UChar         getNativeSeparator();
   static String        toNativeSeparator(const String& path);

   virtual ~FileSystem();

 // query
   bool exists(const String& filename) const;

 // operations
   File* open(const String& filename, int modus) const;

   void addPath(const String& path);
   void removePath(const String& path);
   void removeAll();

   /// Creates any part of the given path
   bool mkpath(const String& path);

   virtual int mkdir(const String& path) = 0;
   virtual bool copyFile(const String& from, const String& to) = 0;

   virtual UChar getSeparator() const = 0;
   virtual bool find(const String& mask, std::vector<String>& result, bool recursive) = 0;

protected:
   FileSystem();

   String expand(const String& path) const;

private:

   FileSystemPaths mPaths;
};

#endif // FILESYSTEM_H_
