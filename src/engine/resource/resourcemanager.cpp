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

#include "engine/ui/font.h"

#include "resourcehandle.h"

#include "engine/texture.h"

ResourceManager::ResourceManager():
   mFreeTypeLib(NULL),
   mResources()
{
   mResources.create(256);
   initialize();
}

ResourceManager::~ResourceManager()
{
   destroy();
}

ResourceManager& ResourceManager::getInstance()
{
   static ResourceManager manager;
   return manager;
}

// - Initialization

bool ResourceManager::initialize()
{
   if ( FT_Init_FreeType(&mFreeTypeLib) != 0 )
      return false;

   return true;
}

void ResourceManager::destroy()
{
   FT_Done_FreeType(mFreeTypeLib);
}

/// \fn ResourceManager::loadTexture (const std::string& file)
/// \brief Returns a texture from a the given file.
TexturePtr ResourceManager::getTexture(const std::string& file)
{
	ResourceHandle* phandle = static_cast<ResourceHandle*>(mResources.lookup(file));
	if ( phandle == NULL )
   {
      AutoPtr<Texture> texture = new Texture();
      if ( !texture.hasPointer() || !texture->load(file) )
         return TexturePtr();

      phandle = new ResourceHandle(*this, texture.release());
		mResources.insert(file, static_cast<void*>(phandle));
	}

   return TexturePtr(phandle);
}

FontPtr ResourceManager::getFont(const std::string& name, int size)
{
   ResourceHandle* phandle = static_cast<ResourceHandle*>(mResources.lookup(name));
	if ( phandle == NULL )
   {
      AutoPtr<UIFont> font = new UIFont();
      if ( !font.hasPointer() || !font->load(mFreeTypeLib, name, size) )
      {
         return FontPtr();
      }

      phandle = new ResourceHandle(*this, font.release());
      mResources.insert(name, phandle);
   }
   return FontPtr(phandle);
}

// notifications

void ResourceManager::notifyResourceDeleted(const Resource& resource)
{
   mResources.remove(resource.getName());
}
