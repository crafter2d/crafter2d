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
#include "guieventhandler.h"

#include "guieventhandlers.h"
#ifndef JENGINE_INLINE
#  include "guieventhandlers.inl"
#endif

GuiEventHandlers::GuiEventHandlers():
   _handlers()
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiEventHandlers::add(GuiEventHandler* phandler)
{
   getHandlers().push_back(phandler);
}

GuiEventHandler* GuiEventHandlers::findByEventType(GuiEvent event)
{
   for ( int handler = 0; handler < getHandlers().size(); ++handler )
   {
      GuiEventHandler* phandler = getHandlers()[handler];
      if ( phandler->getType() == event )
      {
         return phandler;
      }
   }

   return NULL;
}
