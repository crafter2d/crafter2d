/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#ifndef RESOURCE_HANDLE_H_
#define RESOURCE_HANDLE_H_

#include "core/defines.h"
#include "core/smartptr/refcount.h"

class Resource;
class ResourceManager;

class ResourceHandle : public RefCount
{
public:
   ResourceHandle(ResourceManager& manager, Resource* presource = NULL);
   virtual ~ResourceHandle();

 // get/set
   bool            hasResource() const;
   const Resource& getResource() const;
   Resource&       getResource();
   void            setResource(Resource* presource);

 // operations
   void release();

private:

 // members
   ResourceManager& mManager;
   Resource*        mpResource;
};

#ifdef JENGINE_INLINE
#  include "resourcehandle.inl"
#endif

#endif // RESOURCE_HANDLE_H_
