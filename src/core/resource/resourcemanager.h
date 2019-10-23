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
#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <string>
#include <unordered_map>

#include "core/content/contentmanager.h"
#include "core/string/string.h"

#include "resourceptr.h"

namespace Graphics
{
   class Texture;
};

typedef ResourcePtr<Graphics::Texture> TexturePtr;

/**
@author Jeroen Broekhuizen
\brief Resource manager.
*/
class CORE_API ResourceManager
{
public:
   static ResourceManager& getInstance();

   ~ResourceManager();

 // resource retreival
   TexturePtr getTexture(const String& file);
   
 // notifications
   void notifyResourceDeleted(const ResourceHandleBase& resource);

private:
   using Resources = std::unordered_map<String, ResourceHandleBase*>;

   explicit ResourceManager();
   ResourceManager& operator=(const ResourceManager& mgr) = delete;
   
 // members
   c2d::ContentManager mContentManager;
	Resources   mResources;
};

#endif // RESOURCE_MANAGER_H_
