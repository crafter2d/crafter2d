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

#include "unzipfile.h"

class FileSystemPath
{
public:
   explicit FileSystemPath(const std::string& path);
           ~FileSystemPath();

 // get/set
   const std::string& getPath() const;

   bool             hasUnzip() const;
   const UnzipFile& getUnzip() const;

 // comparison
   bool operator==(const std::string& path) const;

private:
   void fillInfo(const std::string& path);

   std::string mPath;
   UnzipFile*  mpUnzip;
};

#endif // FILESYSTEM_PATH_H_
