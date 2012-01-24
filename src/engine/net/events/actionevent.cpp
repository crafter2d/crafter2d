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
#include "actionevent.h"
#ifndef JENGINE_INLINE
#  include "actionevent.inl"
#endif

IMPLEMENT_REPLICATABLE(ActionEventId, ActionEvent, NetEvent)

ActionEvent::ActionEvent(): 
   NetEvent(actionEvent),
   mAction(0),
   mDown(false)
{
}

ActionEvent::ActionEvent(int action, bool down): 
   NetEvent(actionEvent), 
   mAction(action), 
   mDown(down)
{
}

//--------------------------------------------
// - Streaming
//--------------------------------------------

void ActionEvent::doPack(BitStream& stream) const
{
   NetEvent::doPack(stream);
   stream << mAction << mDown;
}

void ActionEvent::doUnpack(BitStream& stream, int dirtyflag)
{
   NetEvent::doUnpack(stream, dirtyflag);
   stream >> mAction >> mDown;
}
