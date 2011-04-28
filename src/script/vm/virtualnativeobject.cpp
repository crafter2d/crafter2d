
#include "virtualnativeobject.h"

#include "core/defines.h"

VirtualNativeObject::VirtualNativeObject():
   VirtualObject(),
   mpObject(NULL)
{
}

VirtualNativeObject::VirtualNativeObject(void* pobject):
   VirtualObject(),
   mpObject(pobject)
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
   delete mpObject;
   mpObject = pobject;
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
