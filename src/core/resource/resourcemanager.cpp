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

#include "core/smartptr/autoptr.h"
#include "core/containers/hashinterface.h"
#include "core/graphics/font.h"
#include "core/graphics/texture.h"
#include "core/graphics/device.h"

#include "resourcehandle.h"

ResourceManager::ResourceManager():
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
TexturePtr ResourceManager::getTexture(Graphics::Device& device, const String& file)
{
   ResourceHandle<Graphics::Texture>* phandle = NULL;

   if ( !mResources.contains(file) )
   {
      String path = sPath + file;
      DataStream* pstream = NULL;
      AutoPtr<Graphics::Texture> texture = device.createTexture(*pstream); // TODO!
      if ( !texture.hasPointer() )
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

FontPtr ResourceManager::getFont(Graphics::Device& device, const String& name, int size)
{
   ResourceHandle<Graphics::Font>* phandle = NULL;

   if ( !mResources.contains(name) )
   {
      AutoPtr<Graphics::Font> font = device.createFont(name, size);
      if ( !font.hasPointer() )
         return FontPtr();

      phandle = new ResourceHandle<Graphics::Font>(*this, font.release());
      phandle->setName(name);

      ResourceHandleBase* pbasehandle = phandle;
      mResources.insert(name, pbasehandle);
   }
   else
   {
      phandle = static_cast<ResourceHandle<Graphics::Font>*>(*mResources.get(name));
   }

   ASSERT_PTR(phandle);
   return FontPtr(phandle);
}

// - Notifications

void ResourceManager::notifyResourceDeleted(const ResourceHandleBase& resource)
{
   mResources.remove(resource.getName());
}
