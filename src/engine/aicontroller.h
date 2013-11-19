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
#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include "core/entity/controller.h"
#include "core/string/string.h"

class Process;
class Script;
class VirtualObject;

class AIController : public Controller
{
public:
   explicit AIController(Process& process);
   virtual ~AIController();

 // get/set
   void setThis(VirtualObject& self);

 // operations
   virtual void performAction(Entity& actor);

private:

   static const String smFunc;

   Script* mpScript;
};

#endif // AI_CONTROLLER_H
