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
#ifndef UNZIPFILE_H
#define UNZIPFILE_H

#include "core/core_base.h"

#include <string>
#include "unzip.h"

class CORE_API UnzipFile
{
public:
   static bool isZip(const std::string& path);

   explicit UnzipFile(const std::string& path);
            ~UnzipFile();

   bool contains(const std::string& name) const;

   bool readFile(const std::string& name, void*& pdata, int& size, bool casesensitive = true);

private:
   UnzipFile();
   bool open(const std::string& path);

   unzFile _zip;
};

#endif
