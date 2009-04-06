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
#include "net/netevent.h"

#include "actionmap.h"
#include "scenegraph.h"

#include "process.h"
#ifndef JENGINE_INLINE
#  include "process.inl"
#endif

Process::Process(void): 
   graph(),
   actionMap(0),
   initialized(false)
{
   conn.attachProcess(this);
}

Process::~Process(void)
{
}

bool Process::create()
{
   return true;
}

bool Process::destroy()
{
   conn.disconnect();

   graph.setNotify(false);
   graph.removeAll();

   return true;
}

void Process::update(float tick)
{
   if ( conn.isConnected() )
      conn.update();
}

void Process::sendScriptEvent(const BitStream* stream, Uint32 client)
{
   if ( conn.isConnected() )
   {
      BitStream s;
      NetEvent event(scriptEvent);
      s << &event << stream;
      if (client != INVALID_CLIENTID)
         conn.setClientId(client);
      conn.send(&s);
   }
}
