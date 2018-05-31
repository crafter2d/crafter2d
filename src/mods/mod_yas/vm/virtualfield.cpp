
#include "virtualfield.h"

#include "core/defines.h"

VirtualField::VirtualField():
   mType(),
   mName(),
   mpClass(nullptr),
   mIndex(-1)
{
}

VirtualField::VirtualField(const String& name, const yasc::Type& type):
   mType(type),
   mName(name),
   mpClass(nullptr),
   mIndex(-1)
{
}

VirtualField::~VirtualField()
{
}

// - Get/set

const String& VirtualField::getName() const
{
   return mName;
}

void VirtualField::setName(const String& name)
{
   mName = name;
}

const VirtualClass& VirtualField::getClass() const
{
   ASSERT_PTR(mpClass);
   return *mpClass;
}

void VirtualField::setClass(VirtualClass& klass)
{
   mpClass = &klass;
}

const yasc::Type& VirtualField::getType() const
{
   return mType;
}

void VirtualField::setType(const yasc::Type& type)
{
   mType = type;
}

int VirtualField::getIndex() const
{
   return mIndex;
}
      
void VirtualField::setIndex(int index)
{
   mIndex = index;
}
