/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef MOUSE_EVENT_DISPATCHER
#define MOUSE_EVENT_DISPATCHER

#include "core/core_base.h"

class MouseEvent;

/*! 
\brief Event dispatcher from game to user interface.
\author Jeroen Broekhuizen

The mouse event dispatcher is an abstract base class used for routing input gathered
by the GameWindow to e.g. the UI system of JEngine SSE. Note that the game input is
handled differently in the KeyMap.
*/
class CORE_API MouseEventDispatcher
{
public:
   MouseEventDispatcher();
   virtual ~MouseEventDispatcher();

   virtual void dispatch(const MouseEvent& event) = 0;
};

#endif