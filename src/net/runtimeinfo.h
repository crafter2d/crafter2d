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
#ifndef RUNTIMEINFO_H
#define RUNTIMEINFO_H

#include <string>

class NetObject;

/**
@author Jeroen Broekhuizen
*/
class RuntimeInfo
{
public:
   typedef NetObject* (*CreateObjectFnc)();

   explicit RuntimeInfo(int id);
   ~RuntimeInfo();
   
 // Get/set interface
   bool                 hasBaseObject() const;

   CreateObjectFnc      getCreateObjectFnc() const;
   RuntimeInfo&         getBaseObject() const;
   const std::string&   getName() const;
   const std::string&   getBase() const;
   int                  getId() const;
   
   void setName(const std::string& name);
   void setBase(const std::string& base);
   void setBaseObject(RuntimeInfo* pbase);
   void setCreateObjectFnc(CreateObjectFnc fnc);
   
 // Operations
   bool isDerivedFrom(int baseid);

private:
   CreateObjectFnc   _createObject;
   RuntimeInfo*      _pbaseobject;
   std::string       _name;
   std::string       _base;
   int               _id;
};

#ifdef JENGINE_INCLUDE
#  include "runtimeinfo.inl"
#endif

#endif
