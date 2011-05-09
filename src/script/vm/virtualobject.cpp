
#include "virtualobject.h"

#include <exception>

#include "core/defines.h"

#include "script/common/variant.h"

VirtualObject::VirtualObject():
   mpClass(NULL),
   mpMembers(NULL),
   mMemberCount(0)
{
}

VirtualObject::~VirtualObject()
{
   delete[] mpMembers;
   mpMembers = NULL;
}

Variant& VirtualObject::operator[](int index)
{
   return getMember(index);
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

// - Downcasting

bool VirtualObject::isNative() const
{
   return false;
}

VirtualNativeObject& VirtualObject::asNative()
{
   throw new std::exception();
}

bool VirtualObject::isInstance() const
{
   return false;
}

VirtualClassObject& VirtualObject::asInstance()
{
   throw new std::exception();
}
