/***************************************************************************
 * Copyright (C) 2006 Jeroen Broekhuizen                                   *
 * jeroen@jeroen.homedns.org                                               *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            * 
 * version 2.1 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the                   *
 * Free Software Foundation, Inc.,                                         *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            *
 ***************************************************************************/
#ifndef GUIEVENT_HANDLERS_H_
#define GUIEVENT_HANDLERS_H_

#include <vector>
#include "guidefines.h"

class GuiEventHandler;

/**
@author Jeroen Broekhuizen
*/
class GuiEventHandlers
{
public:
   typedef std::vector<GuiEventHandler*> EventHandler;

   GuiEventHandlers();

 // Get/set interface
   EventHandler& getHandlers();

 // Operations
   void              add(GuiEventHandler* phandler);

   GuiEventHandler*  findByEventType(GuiEvent event);

private:
   EventHandler    _handlers;
};

#ifdef JENGINE_INLINE
#  include "guieventhandlers.inl"
#endif

#endif
