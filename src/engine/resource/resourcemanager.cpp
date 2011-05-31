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
#include "resourcemanager.h"

#include "resourcehandle.h"

ResourceManager::ResourceManager():
   mResources()
{
   mResources.create(256);
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
	ResourceHandle* phandle = static_cast<ResourceHandle*>(mResources.lookup(file));
	if ( phandle == NULL )
   {
      Texture* ptexture = new Texture();
      if ( !ptexture->load(file) )
         return TexturePtr();

      phandle = new ResourceHandle(*this, ptexture);
		mResources.insert(file, static_cast<void*>(phandle));
	}

   return TexturePtr(phandle);
}

// notifications

void ResourceManager::notifyResourceDeleted(const Resource& resource)
{
   mResources.remove(resource.getFilename());
}
