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
#include "contentmanager.h"
#ifndef JENGINE_INLINE
#  include "contentmanager.inl"
#endif

#include <tinyxml.h>

#include "core/content/contentheader.h"
#include "core/content/contentreader.h"
#include "core/modules/contentmodule.h"
#include "core/modules/modulemanager.h"
#include "core/streams/filereaderstream.h"
#include "core/system/uuid.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"

#include "content.h"

namespace c2d
{
   ContentManager::ContentManager() :
      mModules(),
      mBaseDir(),
      mpDevice(NULL),
      mpSimulator(NULL)
   {
   }

   // - Operations

   void ContentManager::initialize(ModuleManager& manager)
   {
      mModules = manager.filter(ModuleKind::eContentModule);
   }

   IContent* ContentManager::load(const String& name)
   {
      IContent* presult = NULL;

      String path = name + UTEXT(".c2d");
      File* pfile = FileSystem::getInstance().open(path, File::ERead | File::EBinary);
      if ( pfile != NULL )
      {
         Uuid uuid;
         FileReaderStream stream(*pfile);
         uuid.read(stream);

         ContentModule* pmodule = findModule(uuid);
         if ( pmodule != NULL )
         {
            ContentReader& reader = pmodule->getReader();
            reader.setContentManager(*this);
            reader.setGraphicsDevice(mpDevice);
            reader.setPhysicsSimulator(mpSimulator);
            reader.setSoundManager(mpSoundManager);

            presult = reader.read(stream);
            if ( presult != NULL )
            {
               presult->setFilename(name);
            }
         }
      }

      return presult;
   }

   // - Search

   ContentModule* ContentManager::findModule(const Uuid& uuid)
   {
      return static_cast<ContentModule*>(mModules[uuid]);
   }
} // namespace c2d
