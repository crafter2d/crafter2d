/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "../defines.h"

INLINE bool RuntimeInfo::hasBaseObject() const
{
   return _pbaseobject != NULL;
}

INLINE RuntimeInfo::CreateObjectFnc RuntimeInfo::getCreateObjectFnc() const
{
   return _createObject;
}

INLINE RuntimeInfo& RuntimeInfo::getBaseObject() const
{
   ASSERT_PTR(_pbaseobject)
   return *_pbaseobject;
}

INLINE const std::string& RuntimeInfo::getName() const
{
   return _name;
}

INLINE const std::string& RuntimeInfo::getBase() const
{
   return _base;
}

INLINE int RuntimeInfo::getId() const
{
   return _id;
}

INLINE void RuntimeInfo::setName(const std::string& name)
{
   _name = name;
}

INLINE void RuntimeInfo::setBase(const std::string& base)
{
   _base = base;
}

INLINE void RuntimeInfo::setBaseObject(RuntimeInfo* pbase)
{
   _pbaseobject = pbase;
}

INLINE void RuntimeInfo::setCreateObjectFnc(CreateObjectFnc fnc)
{
   _createObject = fnc;
}
