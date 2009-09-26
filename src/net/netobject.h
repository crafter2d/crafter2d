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
#ifndef _NETOBJECT_H_
#define _NETOBJECT_H_

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <map>
#include <string>
#include "../defines.h"

#include "netdefines.h"
#include "bitstream.h"
#include "netobjectfactory.h"
#include "runtimeinfo.h"

class RuntimeInfo;

/*! @author Jeroen Broekhuizen

The NetObject class is the base class for objects that should be replicated
across the internet/LAN via a NetConnection.

@section enable_replication Enabling a class for replication
To enable a class for automatic replication you must derive your class from the NetObject
class. Inside the class definition should must add the macro DEFINE_REPLICATABLE with the
the class name. This macro adds some methods to the class which are neccessary for
automatic registering the class. In the implementation file you must include the
IMPLEMENT_REPLICATABLE macro with the same class name. The implementation of the
methods defined by the define macro will be implemented. After fullfiling these requirement
your class will be ready for automatic replication.

For example if you have a class Car, you could define it the following way:
@code
class Car {
public:
   DEFINE_REPLICATABLE(Car)
   Car();
   ...
}
@endcode
At the top of the implementation file you add the following line to add the implementations
of the functions defined by the other macro.
@code
IMPLEMENT_REPLICATABLE(Car)
Car::Car() {
...
@endcode
*/
class NetObject
{	
public:
   DEFINE_REPLICATABLE(NetObject);

                        NetObject();
   virtual              ~NetObject();

   void                 setReplica(bool replica=true);
   void                 setDirty(int flag);
   void                 resetDirty();

   bool                 isDirty() const;
   bool                 isReplica() const;
   bool                 isStatic() const;

   virtual void         pack(BitStream& stream) const;
   virtual void         unpack(BitStream& stream);

protected:
   int dirtyFlag;
   bool replica;
   bool statik;
};

#ifdef JENGINE_INLINE
#  include "netobject.inl"
#endif

#endif
