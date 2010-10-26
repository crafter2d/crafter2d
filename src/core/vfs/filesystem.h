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

#include "core/core_base.h"

#include <string>

#include "filesystempaths.h" 

class File;

/// \brief Abstract base class of the virtual file system.

class CORE_API FileSystem
{
public:
   typedef void(*Callback)(const char* name, bool isdir, void* pdata);

   static FileSystem&   getInstance();

   virtual ~FileSystem();

   File* open(const std::string& filename, int modus) const;

   void addPath(const std::string& path);
   void removePath(const std::string& path);

   virtual bool recurseDirectory(const std::string& dir, Callback callback, void* pdata = NULL) = 0;
   virtual bool find(const std::string& mask, Callback callback, void* pdata = NULL) = 0;

protected:
   FileSystem();

   std::string expand(const std::string& path) const;
      
private:
   File* tryOpen(const std::string& path, const std::string& file, int modus) const;

   FileSystemPaths mPaths;
};

#endif // FILESYSTEM_H_
