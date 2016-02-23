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

#include <memory>

#include "core/smartptr/autoptr.h"
#include "core/containers/hashinterface.h"
#include "core/graphics/font.h"
#include "core/graphics/texture.h"

#include "resourcehandle.h"

ResourceManager::ResourceManager():
   mContentManager(),
   mResources()
{
   mResources.setHashFunction(HashInterface::hashString);
}

ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::getInstance()
{
   static ResourceManager manager;
   return manager;
}

// - Operations

static String sPath = UTEXT("../images/");

/// \fn ResourceManager::loadTexture (const std::string& file)
/// \brief Returns a texture from a the given file.
TexturePtr ResourceManager::getTexture(const String& file)
{
   ResourceHandle<Graphics::Texture>* phandle = NULL;

   if ( !mResources.contains(file) )
   {
      String path = sPath + file;
      std::unique_ptr<Graphics::Texture> texture(mContentManager.loadContent<Graphics::Texture>(path));
      if ( !(texture) )
         return TexturePtr();

      phandle = new ResourceHandle<Graphics::Texture>(*this, texture.release());
      phandle->setName(file);

      ResourceHandleBase* pbasehandle = phandle; // needed for *& in container, perhaps time to change that..
		mResources.insert(file, pbasehandle);
	}
   else
   {
      phandle = static_cast<ResourceHandle<Graphics::Texture>*>(*mResources.get(file));
   }

   ASSERT_PTR(phandle);
   return TexturePtr(phandle);
}

// - Notifications

void ResourceManager::notifyResourceDeleted(const ResourceHandleBase& resource)
{
   mResources.remove(resource.getName());
}
