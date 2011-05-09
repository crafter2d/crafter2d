
#include "virtualclassobject.h"

#include "core/defines.h"

VirtualClassObject::VirtualClassObject():
   VirtualObject()
{
}

VirtualClassObject::~VirtualClassObject()
{
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
