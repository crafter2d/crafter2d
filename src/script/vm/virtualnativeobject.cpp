
#include "virtualnativeobject.h"

#include "core/defines.h"

#include "virtualmachine.h"

VirtualNativeObject::VirtualNativeObject(VirtualMachine& machine, void* pobject):
   VirtualObject(),
   mMachine(machine),
   mpObject(pobject),
   mOwned(true)
{
}

VirtualNativeObject::~VirtualNativeObject()
{
   setObject(NULL);
}

// get/set
   
void* VirtualNativeObject::getObject()
{
   return mpObject;
}

void VirtualNativeObject::setObject(void* pobject)
{
   if ( mpObject != pobject )
   {
      mMachine.deleteNative(mpObject);

      if ( mOwned )
      {
         delete mpObject;
         mpObject = NULL;
      }

      mpObject = pobject;
   }
}

void VirtualNativeObject::setOwned(bool owned)
{
   mOwned = owned;
}

// - Downcasting

bool VirtualNativeObject::isNative() const
{
   return true;
}

VirtualNativeObject& VirtualNativeObject::asNative()
{
   return *this;
}
