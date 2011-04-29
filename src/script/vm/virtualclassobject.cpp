
#include "virtualclassobject.h"

#include "core/defines.h"

#include "script/common/variant.h"

VirtualClassObject::VirtualClassObject():
   VirtualObject(),
   mpMembers(NULL),
   mMemberCount(0)
{
}

VirtualClassObject::~VirtualClassObject()
{
   delete[] mpMembers;
}

Variant& VirtualClassObject::operator[](int index)
{
   ASSERT(index >= 0);
   ASSERT(index < mMemberCount);
   return mpMembers[index];
}

// - Operations

void VirtualClassObject::initialize(int variables)
{
   if ( variables > 0 )
   {
      mpMembers = new Variant[variables];
      mMemberCount = variables;
   }
}

void VirtualClassObject::setMember(int index, const Variant& value)
{
   ASSERT_PTR(mpMembers);
   ASSERT(index < mMemberCount);

   mpMembers[index] = value;
}

// - Downcasting

bool VirtualClassObject::isInstance() const
{
   return true;
}

VirtualClassObject& VirtualClassObject::asInstance()
{
   return *this;
}
