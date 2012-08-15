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

#include "filesystempaths.h"

#include <algorithm>

#include "core/string/string.h"

FileSystemPaths::FileSystemPaths():
   mPaths()
{
}

const FileSystemPath& FileSystemPaths::operator[](int index) const
{
   return *mPaths[index];
}

// - Query

class Pred
{
public:
   Pred(const String& path): mPath(path) {}

   bool operator()(FileSystemPath* ppath) {
      return (*ppath) == mPath;
   }

private:
   const String& mPath;
};

bool FileSystemPaths::contains(const String& path) const
{
   return std::find_if(mPaths.begin(), mPaths.end(), Pred(path)) != mPaths.end();
}

int FileSystemPaths::size() const
{
   return mPaths.size();
}

// - Operations

void FileSystemPaths::add(const String& path)
{
   if ( !contains(path) )
   {
      FileSystemPath* ppath = new FileSystemPath(path);
      mPaths.push_back(ppath);
   }
}

void FileSystemPaths::remove(const String& path)
{
   Paths::iterator it = std::find_if(mPaths.begin(), mPaths.end(), Pred(path));
   if ( it != mPaths.end() )
   {
      mPaths.erase(it);
   }
}

void FileSystemPaths::removeAll()
{
   mPaths.clear();
}

