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
#ifndef NETOBJECTFACTORY_H_
#define NETOBJECTFACTORY_H_

#include <string>
#include <map>

class RuntimeInfo;
class NetObject;

class NetObjectFactory
{
public:
   static NetObjectFactory& getInstance();

   ~NetObjectFactory();

 // Initialization
   void initialize();

 // Operations
   void         registerObject(int id, RuntimeInfo& info);
   NetObject*   createObject(const std::string& name);
   NetObject*   createObject(int id);

private:
   typedef std::map<int, RuntimeInfo*> RuntimeInfoMap;

   NetObjectFactory();

   void buildHierarchy();

 // Get/set interface
   RuntimeInfoMap& getRuntimeInfoMap();

 // Find
   RuntimeInfo* find(const std::string& name);

   RuntimeInfoMap _infomap;
};

#ifdef JENGINE_INLINE
#  include "netobjectfactory.inl"
#endif

#endif
