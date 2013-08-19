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
#include "aicontroller.h"

#include "core/defines.h"
#include "core/smartptr/autoptr.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "actor.h"
#include "process.h"

// static
const String AIController::smFunc("updateAI");

AIController::AIController(Process& process):
   Controller(),
   mpScript(NULL)
{
   mpScript = new Script(process.getScriptManager());
}

AIController::~AIController()
{
   delete mpScript;
   mpScript = NULL;
}

// - Get/set

void AIController::setThis(VirtualObject& self)
{
   mpScript->setThis(self);
}

// - Operations

void AIController::performAction(Actor& actor)
{
   Variant arg(mpScript->resolve(&actor));
   mpScript->run(smFunc, 1, &arg);
}
