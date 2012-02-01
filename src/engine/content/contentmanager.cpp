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

#include "core/smartptr/autoptr.h"

#include "engine/actor.h"
#include "engine/process.h"

#include "engine/world/world.h"
#include "engine/world/worldreader.h"

#include "objectloader.h"

ContentManager::ContentManager(Process& process):
   mProcess(process)
{
}

//---------------------------------
// - Loading
//---------------------------------

Entity* ContentManager::loadEntity(const std::string& filename)
{
   if ( !mProcess.hasWorld() )
   {
      // need a world first!
      return NULL;
   }

   ActorLoader loader(mProcess);
   loader.setSimulator(mProcess.getWorld().getSimulator());

   return loader.load(filename);
}

World* ContentManager::load(const std::string& filename)
{
   AutoPtr<World> world = new World();
   if ( world.hasPointer() )
   {
      WorldReader reader;
      if ( !reader.read(*world, filename) )
      {
         // failed, throw an exception
         return NULL;
      }
   }

   return world.release();
}
