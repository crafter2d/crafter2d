
#include "virtualarrayobject.h"

#include <exception>

#include "core/defines.h"

#include "script/common/variant.h"

#include "virtualarrayexception.h"

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
      throw new VirtualArrayException(VirtualArrayException::eOutOfBounds);
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

void VirtualArrayObject::resize(int newsize)
{
   if ( mSize != newsize )
   {
      int count = MIN(mSize, newsize);

      Variant* parray = new Variant[newsize];
      for ( int index = 0; index < count; index++ )
      {
         parray[index] = mpArray[index];
      }

      delete[] mpArray;
      mpArray = parray;
      mSize   = newsize;
   }
}

// - Downcast

VirtualArrayObject& VirtualArrayObject::asArray()
{
   return *this;
}
