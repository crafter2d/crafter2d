/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "core/modules/modulecollection.h"
#include "core/string/string.h"
#include "core/core_base.h"

#include "content.h"

namespace Graphics
{
   class Device;
}

class IContent;
class ContentModule;
class ModuleManager;
class Uuid;

/**
 * The content manager is responsible for loading content.
 * There are some types of content that we support:
 *   - images
 *   - sounds
 *   - entities
 *   - worlds
 *   - scripts
 *   - effects
 *
 * These should all be derived from a base content class. Such that we
 * can implement a T loadContent(const String& name);
 * Then to load a sound we could say:
 *   Sound* psound = ContentManager<Sound>.loadContent
 *
 */

class CORE_API ContentManager
{
public:
   ContentManager();

 // get/set
   Graphics::Device& getDevice();
   void              setDevice(Graphics::Device& device);

   const String& getBaseDir() const;
   void          setBaseDir(const String& basedir);

 // loading
   template<class T> T* loadContent(const String& name);

 // operations
   void initialize(ModuleManager& manager);
   
private:   
 // loading
   IContent* load(const String& name);

 // search
   ContentModule* findModule(const Uuid& uuid);

 // data
   Graphics::Device* mpDevice; // not owned
   ModuleCollection  mModules;
   String            mBaseDir;
};

#ifdef JENGINE_INLINE
#include "contentmanager.inl"
#endif

template<class T>
T* ContentManager::loadContent(const String& name)
{
   return dynamic_cast<T*>(load(name));
}

#endif // CONTENT_MANAGER_H
