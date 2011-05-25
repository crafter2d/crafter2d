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
#include "virtualobject.h"

#include <exception>

#include "core/defines.h"

#include "script/common/variant.h"

#include "virtualobjectobserver.h"

VirtualObject::VirtualObject():
   mpObserver(NULL),
   mpClass(NULL),
   mpNativeObject(NULL),
   mOwnsNative(true),
   mpMembers(NULL),
   mMemberCount(0)
{
}

VirtualObject::~VirtualObject()
{
   delete[] mpMembers;
   mpMembers = NULL;

   setNativeObject(NULL);
}

// - Get/set

bool VirtualObject::hasNativeObject() const
{
   return mpNativeObject != NULL;
}

void* VirtualObject::getNativeObject()
{return mpNativeObject;
}

void* VirtualObject::useNativeObject()
{
   ASSERT(mOwnsNative);

   mOwnsNative = false;
   return mpNativeObject;
}

void VirtualObject::setNativeObject(void* pobject)
{
   if ( mpNativeObject != pobject )
   {
      if ( mpNativeObject != NULL && mpObserver != NULL )
      {
         //mpObserver->onDestroyed(*this);
      }

      if ( mOwnsNative )
      {
         delete mpNativeObject;
      }

      mpNativeObject = pobject;

      if ( mpNativeObject != NULL && mpObserver != NULL)
      {
         //mpObserver->onCreated(*this);
      }
   }
}

bool VirtualObject::isOwner() const
{
   return mOwnsNative;
}

void VirtualObject::setOwner(bool owned)
{
   mOwnsNative = owned;
}

// - Query

const VirtualClass& VirtualObject::getClass() const
{
   return *mpClass;
}

void VirtualObject::setClass(const VirtualClass& definition)
{
   mpClass = &definition;
}

// - Operations

void VirtualObject::initialize(int variables)
{
   if ( variables > 0 )
   {
      mpMembers = new Variant[variables];
      mMemberCount = variables;
   }
}

Variant& VirtualObject::getMember(int index)
{
   ASSERT(index >= 0);
   ASSERT(index < mMemberCount);
   return mpMembers[index];
}

void VirtualObject::setMember(int index, const Variant& value)
{
   ASSERT_PTR(mpMembers);
   ASSERT(index < mMemberCount);

   mpMembers[index] = value;
}

// observers
   
void VirtualObject::registerObserver(VirtualObjectObserver& observer)
{
   mpObserver = &observer;
}

void VirtualObject::unregisterObserver(VirtualObjectObserver& observer)
{
   mpObserver = NULL;
}
