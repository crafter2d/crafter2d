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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class File;

/**
   @author Jeroen Broekhuizen <jeroen@jengine.homedns.org>
*/
class FileSystem
{
public:
   typedef void(*Callback)(const char* name, bool isdir, void* pdata);

   static FileSystem&   getInstance();

   virtual ~FileSystem();

   File* open(const std::string& filename);

   virtual bool recurseDirectory(const std::string& dir, Callback callback, void* pdata = NULL) = 0;
   virtual bool find(const std::string& mask, Callback callback, void* pdata = NULL) = 0;

protected:
   FileSystem();
};

#endif
