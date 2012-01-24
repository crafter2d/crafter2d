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
#ifndef INPUT_CONTROLLER_H_
#define INPUT_CONTROLLER_H_

#include <queue>

#include "actionmap.h"
#include "controller.h"

class ActionEvent;
class ActionMap;

class InputController : public Controller
{
   typedef std::queue<ActionEvent> ActionQueue;

public:
   InputController();

 // get/set
   void setActionMap(ActionMap* pactionmap);

 // operations
   void queueAction(const ActionEvent& actionevent);
   void clearActions();

 // overloads
   virtual void requestAction(const ActionEvent& actionevent);
   virtual void performAction(Actor& actor);

private:

 // members
   ActionQueue mActions;
   ActionMap*  mpActionMap;
};

#endif // INPUT_CONTROLLER_H_
