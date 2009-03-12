/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "../defines.h"

/// \fn NetObject::setReplica(bool rep)
/// \brief Set the replica flag of this network object.
void NetObject::setReplica(bool rep)
{
   replica = rep;
}

/// \fn NetObject::setDirty(bool dirt)
/// \brief Set the dirty flag.
void NetObject::setDirty(bool dirt)
{
   dirty = dirt; 
   if(!dirty)
      dirtyFlag = 0;
}

/// \fn NetObject::isDirty() const
/// \brief Checks if this object is dirty.
bool NetObject::isDirty() const
{ 
   return dirty; 
}

/// \fn NetObject::isReplica() const
/// \brief Checks if this object is a replica.
bool NetObject::isReplica() const
{
   return replica;
}

/// \fn NetObject::isStatic() const
/// \brief Check if this is a static object.
bool NetObject::isStatic() const
{
   return statik;
}
