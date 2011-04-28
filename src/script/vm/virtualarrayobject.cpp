
#include "virtualarrayobject.h"

#include <exception>

#include "common/variant.h"

VirtualArrayObject::VirtualArrayObject():
   mpArray(0)
{
}

VirtualArrayObject::~VirtualArrayObject()
{
   delete[] mpArray;
}

const Variant& VirtualArrayObject::operator[](int index) const
{
   return at(index);
}

Variant& VirtualArrayObject::operator[](int index)
{
   return at(index);
}

// - Query

const Variant& VirtualArrayObject::at(int index) const
{
   return const_cast<VirtualArrayObject&>(*this).at(index);
}

Variant& VirtualArrayObject::at(int index)
{
   if ( index < 0 || index >= mSize )
   {
      throw std::exception();
   }
   return mpArray[index];
}

int VirtualArrayObject::size() const
{
   return mSize;
}

// - Operations

void VirtualArrayObject::addLevel(int size)
{
   mpArray = new Variant[size];
   mSize   = size;
}

// - Downcast

VirtualArrayObject& VirtualArrayObject::asArray()
{
   return *this;
}
