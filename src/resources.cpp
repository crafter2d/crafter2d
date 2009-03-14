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

#include "vfs/unzipfile.h"

#include "autoptr.h"
#include "game.h"
#include "gameconfiguration.h"

ResourceManager::ResourceManager():
   textures()
{
	textures.create (256);
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
	const std::string& imagepath = Game::getInstance().getConfiguration().getImagePath();
   std::string path = imagepath + file;

	return doLoadTexture(path);
}

/// \fn ResourceManager::loadSystemTexture (const std::string& file)
/// \brief Returns a system texture.
TexturePtr ResourceManager::loadSystemTexture(const std::string& filename)
{
   const std::string& imagepath = "data.zip/images/";
   std::string path = imagepath + filename;

   return doLoadTexture(path);
}

/// \fn ResourceManager::doLoadTexture (const std::string& path)
/// \brief If texture is already loaded, returns a reference. Otherwise it loads the texture
///        and stores it in the hashtable.
TexturePtr ResourceManager::doLoadTexture(const std::string& path)
{
   TexturePtr* ptr = static_cast<TexturePtr*>(textures.lookup(path));
	if ( !ptr != NULL ) 
   {
      Texture* ptexture = new Texture();
      if ( !ptexture->load(path) )
         return TexturePtr();

		ptr = new TexturePtr(ptexture);
		textures.insert(path, static_cast<void*>(ptr));
	}

	return TexturePtr(*ptr);
}
