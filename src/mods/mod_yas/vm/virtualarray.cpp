
#include "virtualarray.h"

#include <exception>

#include "core/defines.h"

#include "virtualarrayexception.h"
#include "virtualobject.h"
#include "virtualvalue.h"

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

const VirtualValue& VirtualArray::operator[](int index) const
{
   return at(index);
}

VirtualValue& VirtualArray::operator[](int index)
{
   return at(index);
}

// - Query

const VirtualValue& VirtualArray::at(int index) const
{
   if ( index < 0 || index >= mSize )
   {
      throw new VirtualArrayException(VirtualArrayException::eOutOfBounds);
   }
   return mpArray[index];
}

VirtualValue& VirtualArray::at(int index)
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
   mpArray = new VirtualValue[size];
   mSize   = size;
}

void VirtualArray::resize(int newsize)
{
   if ( mSize != newsize )
   {
      int count = MIN(mSize, newsize);

      VirtualValue* parray = new VirtualValue[newsize];
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
      VirtualValue& variant = mpArray[index];
      if ( variant.isObject() )
      {
         variant.asObject().mark();
      }
   }
}

void VirtualArray::finalize(VirtualMachine& vm)
{
}
