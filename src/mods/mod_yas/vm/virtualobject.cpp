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

#include "core/defines.h"

#include "virtualvalue.h"
#include "virtualarray.h"
#include "virtualmachine.h"

VirtualObject::VirtualObject():
   mpClass(NULL),
   mpNativeObject(NULL),
   mpMembers(NULL),
   mMemberCount(0),
   mOwnsNative(false)
{
}

VirtualObject::~VirtualObject()
{
   delete[] mpMembers;
   mpMembers = NULL;
   
   ASSERT(!mOwnsNative || mpNativeObject == NULL);
}

// - Get/set

bool VirtualObject::hasNativeObject() const
{
   return mpNativeObject != NULL;
}

void* VirtualObject::getNativeObjectPtr()
{
   return mpNativeObject;
}

void* VirtualObject::useNativeObjectPtr()
{
   mOwnsNative = false;
   return mpNativeObject;
}

void VirtualObject::setNativeObject(void* pobject)
{
   //ASSERT(mpNativeObject == NULL);
   mpNativeObject = pobject;
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

int VirtualObject::getMemberCount() const
{
   return mMemberCount;
}

// - Operations

void VirtualObject::initialize(int variables)
{
   if ( variables > 0 )
   {
      delete[] mpMembers;
      mpMembers = new VirtualValue[variables];
      mMemberCount = variables;
   }
}

const VirtualValue& VirtualObject::getMember(int index) const
{
   ASSERT(index >= 0);
   ASSERT(index < mMemberCount);

   return mpMembers[index];
}

VirtualValue& VirtualObject::getMember(int index)
{
   ASSERT(index >= 0);
   ASSERT(index < mMemberCount);

   return mpMembers[index];
}

void VirtualObject::setMember(int index, const VirtualValue& value)
{
   ASSERT_PTR(mpMembers);
   ASSERT(index < mMemberCount);

   mpMembers[index] = value;
}

// - Garbage collection

void VirtualObject::finalize(VirtualMachine& vm)
{
   if ( mpNativeObject != NULL )
   {
      vm.unregisterNative(*this);
   }

   vm.release(*this);
}

void VirtualObject::doMark()
{
   for ( int index = 0; index < mMemberCount; index++ )
   {
      if ( mpMembers[index].isObject() )
      {
         mpMembers[index].asObject().mark();
      }
      else if ( mpMembers[index].isArray() )
      {
         mpMembers[index].asArray().mark();
      }
   }
}
