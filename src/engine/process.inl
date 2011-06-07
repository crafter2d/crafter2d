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
#include "core/defines.h"

/// \fn Process::setInitialized(bool init)
/// \brief Set the initialized flag to signal that the process has started 
/// up correctly.
INLINE void Process::setInitialized(bool init)
{
   initialized = init;
}

/// \fn Process::getConnection()
/// \brief Returns the net connection of this process.
INLINE NetConnection* Process::getConnection()
{ 
   return &conn; 
}

/// \fn Process::getSceneGraph()
/// \brief Returns the scenegraph of this process.
INLINE SceneGraph& Process::getSceneGraph()
{ 
   return graph;
}

INLINE ScriptManager& Process::getScriptManager()
{
   return mScriptManager;
}

/// \fn Process::getActionMap()
/// \brief Returns the current actionmap of this process.
INLINE ActionMap* Process::getActionMap()
{
   return actionMap; 
}

/// \fn Process::isInitialized()
/// \brief Returns the initialize flag.
INLINE bool Process::isInitialized()
{
   return initialized;
}

INLINE bool Process::isActive() const
{
   return mActive;
}

INLINE void Process::setActive(bool active)
{
   mActive = active;
}
