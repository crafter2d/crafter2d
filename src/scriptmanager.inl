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
#include "defines.h"

INLINE ScriptManager& ScriptManager::getInstance()
{
   static ScriptManager mgr;
   return mgr;
}

/// \fn ScriptManager::createScript(ScriptManager::Kind k)
/// \brief Creates a new script object. This script has its own stack, but also has access to
/// the world variable (if its set).
/// \returns a pointer to the new script object, or NULL if k was invalid
INLINE Script* ScriptManager::createScript (bool child)
{
   return new Script (luaState, child);
}

/// \fn ScriptManager::getTemporaryScript()
/// \brief Returns a tempory script instance. It should not be destroyed.
INLINE Script& ScriptManager::getTemporaryScript()
{
   return tempScript;
}

INLINE ScriptCollection& ScriptManager::activeCollection()
{
   ASSERT_PTR(MPActiveCollection)
   return *MPActiveCollection;
}

INLINE void ScriptManager::activeCollection(ScriptCollection* pcollection)
{
   MPActiveCollection = pcollection;
}

INLINE ScriptManager::Collections& ScriptManager::collections()
{
   ASSERT_PTR(MPCollections);
   return *MPCollections;
}

INLINE bool ScriptManager::executeScript(const std::string& path)
{
   return executeScript(path.c_str());
}
