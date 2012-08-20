
#include "virtualarray.h"

#include <exception>

#include "core/defines.h"

#include "script/common/variant.h"

#include "virtualarrayexception.h"
#include "virtualobject.h"

VirtualArray::VirtualArray():
   Collectable(),
   mpArray(0)
{
}

VirtualArray::~VirtualArray()
{
   if ( mpArray != NULL )
   {
      delete[] mpArray;
      mpArray = NULL;
   }
}

const Variant& VirtualArray::operator[](int index) const
{
   return at(index);
}

Variant& VirtualArray::operator[](int index)
{
   return at(index);
}

// - Query

const Variant& VirtualArray::at(int index) const
{
   return const_cast<VirtualArray&>(*this).at(index);
}

Variant& VirtualArray::at(int index)
{
   if ( index < 0 || index >= mSize )
   {
      throw new VirtualArrayException(VirtualArrayException::eOutOfBounds);
   }
   return mpArray[index];
}

int VirtualArray::size() const
{
   return mSize;
}

// - Operations

void VirtualArray::addLevel(int size)
{
   mpArray = new Variant[size];
   mSize   = size;
}

void VirtualArray::resize(int newsize)
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

// - Marking
   
void VirtualArray::doMark()
{
   for ( int index = 0; index < mSize; index++ )
   {
      Variant& variant = mpArray[index];
      if ( variant.isObject() )
      {
         variant.asObject().mark();
      }
   }
}

void VirtualArray::finalize(VirtualMachine& vm)
{
}
