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
#include "netobjectfactory.h"
#ifndef JENGINE_INLINE
#  include "netobjectfactory.inl"
#endif

#include "netobject.h"
#include "runtimeinfo.h"

NetObjectFactory::NetObjectFactory():
   _infomap()
{
}


NetObjectFactory::~NetObjectFactory()
{
}

NetObjectFactory& NetObjectFactory::getInstance()
{
   static NetObjectFactory factory;
   return factory;
}

//////////////////////////////////////////////////////////////////////////
// - Initialization
//////////////////////////////////////////////////////////////////////////

void NetObjectFactory::initialize()
{
   buildHierarchy();
}

void NetObjectFactory::buildHierarchy()
{
   RuntimeInfoMap::iterator it = getRuntimeInfoMap().begin();
   for ( ; it != getRuntimeInfoMap().end(); ++it )
   {
      RuntimeInfo* pinfo = (it->second);
      if ( !pinfo->hasBaseObject() )
      {
         RuntimeInfo* pbase = find(pinfo->getBase());
         if ( pbase != nullptr )
            pinfo->setBaseObject(pbase);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// - Registration & creation
//////////////////////////////////////////////////////////////////////////

/// \fn NetObjectFactory::registerObject(int id, RuntimeInfo& info)
/// \brief Registers a NetObject based class for replication.
/// \param obj Reference to some object derived from NetObject
///
/// This method registers a class derived from NetObject so that it can be replicated
/// across a network. You should not call this function.
///
/// Consult @ref enable_replication "'Enabling class replication'" for more information
void NetObjectFactory::registerObject(int id, RuntimeInfo& info)
{
   getRuntimeInfoMap()[id] = &info;
}

NetObject* NetObjectFactory::createObject(const std::string& name)
{
   RuntimeInfo* pinfo = find(name);
   if ( pinfo != nullptr )
      return createObject(pinfo->getId());
   else
      return nullptr;
}

NetObject* NetObjectFactory::createObject(int id)
{
   return (getRuntimeInfoMap()[id]->getCreateObjectFnc())();
}

//////////////////////////////////////////////////////////////////////////
// - Find
//////////////////////////////////////////////////////////////////////////

RuntimeInfo* NetObjectFactory::find(const std::string& name)
{
   RuntimeInfoMap::iterator it = getRuntimeInfoMap().begin();
   for ( ; it != getRuntimeInfoMap().end(); ++it )
   {
      RuntimeInfo* pinfo = (it->second);
      if ( pinfo->getName() == name )
         return pinfo;
   }
   
   return nullptr;
}
