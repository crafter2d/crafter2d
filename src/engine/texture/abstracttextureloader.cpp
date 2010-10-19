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
#include "abstracttextureloader.h"
#ifndef JENGINE_INLINE
#  include "abstracttextureloader.inl"
#endif

#include <SDL/SDL.h>
#include <memory>

#include "engine/vfs/file.h"
#include "engine/vfs/filesystem.h"

#include "engine/defines.h"
#include "engine/autoptr.h"

AbstractTextureLoader::AbstractTextureLoader():
   mTextureInfo()
{
}

AbstractTextureLoader::~AbstractTextureLoader()
{
}

bool AbstractTextureLoader::load(const std::string& filename)
{
   AutoPtr<File> pfile = FileSystem::getInstance().open(filename, File::ERead | File::EBinary);

   bool success = virLoad(*pfile, mTextureInfo);

   return success;
}

bool AbstractTextureLoader::virLoad(File& file, TextureInfo& info)
{
   PURE_VIRTUAL
   return false;
}