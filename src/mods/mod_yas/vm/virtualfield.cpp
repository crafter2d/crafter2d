
#include "virtualfield.h"

#include "../common/type.h"

#include "core/defines.h"

VirtualField::VirtualField():
   mName(),
   mpClass(NULL),
   mpType(NULL),
   mIndex(-1)
{
}

VirtualField::VirtualField(const String& name, const yasc::Type& type):
   mName(name),
   mpClass(NULL),
   mpType(type.clone()),
   mIndex(-1)
{
}

VirtualField::~VirtualField()
{
   setType(NULL);
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
   ASSERT_PTR(mpType);
   return *mpType;
}

void VirtualField::setType(const yasc::Type& type)
{
   mpType = type.clone();
}

void VirtualField::setType(yasc::Type* ptype)
{
   delete mpType;
   mpType = ptype;
}

int VirtualField::getIndex() const
{
   return mIndex;
}
      
void VirtualField::setIndex(int index)
{
   mIndex = index;
}
