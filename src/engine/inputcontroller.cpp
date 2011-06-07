/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#include "inputcontroller.h"

#include "core/smartptr/autoptr.h"

#include "engine/net/events/actionevent.h"

InputController::InputController():
   Controller(),
   mActions(),
   mpActionMap(NULL)
{
}

// - Get/set

void InputController::setActionMap(ActionMap* pactionmap)
{
   mpActionMap = pactionmap;
}

// - Operations
   
void InputController::queueAction(const ActionEvent& actionevent)
{
   mActions.push(actionevent);
}

// - Overloads

void InputController::requestAction(const ActionEvent& actionevent)
{
   queueAction(actionevent);
}

void InputController::performAction(Object& actor)
{
   ASSERT_PTR(mpActionMap)

   while ( !mActions.empty() )
   {
      const ActionEvent& actionevent = mActions.front();

      mpActionMap->processRemote(actionevent, actor);

      mActions.pop();
   }
}

void InputController::clearActions()
{
   while ( !mActions.empty() )
      mActions.pop();
}
