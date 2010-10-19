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
#include "resources.h"

ResourceManager::ResourceManager():
   textures()
{
   textures.create(256);
}

ResourceManager& ResourceManager::operator=(const ResourceManager& mgr)
{
   return *this;
}

ResourceManager& ResourceManager::getInstance()
{
   static ResourceManager manager;
   return manager;
}

/// \fn ResourceManager::loadTexture (const std::string& file)
/// \brief Returns a texture from a the given file.
TexturePtr ResourceManager::loadTexture (const std::string& file)
{
	TexturePtr* ptr = static_cast<TexturePtr*>(textures.lookup(file));
	if ( !ptr != NULL )
   {
      Texture* ptexture = new Texture();
      if ( !ptexture->load(file) )
         return TexturePtr();

		ptr = new TexturePtr(ptexture);
		textures.insert(file, static_cast<void*>(ptr));
	}

	return TexturePtr(*ptr);
}

