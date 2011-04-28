
#include "virtualobject.h"

#include <exception>

VirtualObject::VirtualObject():
   mpClass(NULL)
{
}

VirtualObject::~VirtualObject()
{
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
