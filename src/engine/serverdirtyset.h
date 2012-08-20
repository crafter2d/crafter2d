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
#ifndef SERVER_DIRTY_SET_H_
#define SERVER_DIRTY_SET_H_

#include <vector>

#include "dirtyset.h"

class AggregateEvent;
class Entity;

class ServerDirtySet : public DirtySet
{
public:
   ServerDirtySet();
   virtual ~ServerDirtySet();
   
 // reporting
           void collect(AggregateEvent& event);
   virtual void reportDirty(Entity& entity);

private:
 // typedefs
   typedef std::vector<Entity*> Objects;

   Objects mObjects;
};

#endif // SERVER_DIRTY_SET_H_
