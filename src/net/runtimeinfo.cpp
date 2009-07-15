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
#include "runtimeinfo.h"
#ifndef JENGINE_INCLUDE
#  include "runtimeinfo.inl"
#endif

#include "netobjectfactory.h"

RuntimeInfo::RuntimeInfo(int id, const std::string& classname, const std::string& base, CreateObjectFnc createfunction):
   _name(classname),
   _base(base),
   _createObject(createfunction),
   _pbaseobject(NULL),
   _id(id)
{
   NetObjectFactory::getInstance().registerObject(id, *this);
}

RuntimeInfo::~RuntimeInfo()
{
}

bool RuntimeInfo::isDerivedFrom(int baseid)
{
   if ( baseid == _id )
      return true;
   else if ( hasBaseObject() )
      return getBaseObject().isDerivedFrom(baseid);
   else
      return false;
}
