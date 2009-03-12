/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#include "gameconfigurationmanager.h"
#ifndef JENGINE_INLINE
#  include "gameconfigurationmanager.inl"
#endif

#include "vfs/filesystem.h"

#include "console.h"
#include "gameconfiguration.h"

GameConfigurationManager::GameConfigurationManager():
   mConfigurations(),
   mActiveConfiguration(-1)
{
}

GameConfigurationManager::~GameConfigurationManager()
{
   destroy();
}

//////////////////////////////////////////////////////////////////////////
// - init/destruct
//////////////////////////////////////////////////////////////////////////

void callback(const char* pname, bool isdir, void* pdata)
{
   GameConfigurationManager* pmanager = static_cast<GameConfigurationManager*>(pdata);
   pmanager->registerConfiguration(pname);
}

void GameConfigurationManager::initialize()
{
   FileSystem::getInstance().find("./*.cfg", callback, this);
}

void GameConfigurationManager::destroy()
{
   for ( int index = 0; index < mConfigurations.size(); index++ )
   {
      delete mConfigurations[index];
   }

   mConfigurations.clear();
}

void GameConfigurationManager::registerConfiguration(const std::string& filename)
{
   GameConfiguration* pconfiguration = new GameConfiguration();
   if ( !pconfiguration->load(filename) )
   {
      Console::getInstance().error("Failed to load %s", filename);
   }

   mConfigurations.push_back(pconfiguration);
}
