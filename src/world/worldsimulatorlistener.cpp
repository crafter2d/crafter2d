/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#include "worldsimulatorlistener.h"

#include "script.h"
#include "scriptmanager.h"

WorldSimulatorListener::WorldSimulatorListener():
   SimulatorListener()
{
}

WorldSimulatorListener::~WorldSimulatorListener()
{
}

void WorldSimulatorListener::collideObjectWorld(Object& object, Bound& bound, bool overlap)
{
   ScriptManager& mgr = ScriptManager::getInstance();
   Script& script = mgr.getTemporaryScript();
   script.prepareCall("Server_onCollisionObjectWorld");
   script.addParam((void*)&object, "Object");
   script.addParam((void*)&bound, "Bound");
   script.addParam(overlap);
   script.run(3);
}
