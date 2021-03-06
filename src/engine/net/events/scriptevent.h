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
#ifndef SCRIPT_EVENT_H_
#define SCRIPT_EVENT_H_

#include "netevent.h"

#include "core/streams/bufferedstream.h"

/**
@author Jeroen Broekhuizen
*/
class ScriptEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(ScriptEvent)

            ScriptEvent();
   explicit ScriptEvent(const DataStream& stream);
   virtual ~ScriptEvent();

 // get/set
   const DataStream& getStream() const;

protected:
 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

private:
   BufferedStream mStream;
};

#endif
