
#include "virtualstack.h"

#include "core/defines.h"

#include "virtualarray.h"
#include "virtualobject.h"
#include "virtualstring.h"

VirtualStack::VirtualStack(int initialsize):
   mStack(initialsize),
   mSize(0)
{
}

VirtualStack::~VirtualStack()
{
   mStack.clear();
}

const VirtualValue& VirtualStack::operator[](int index) const
{
   ASSERT(index < mSize);
   return mStack[index];
}

VirtualValue& VirtualStack::operator[](int index)
{
   ASSERT(index < mSize);
   return mStack[index];
}

// - Stack operations

void VirtualStack::push(int count, VirtualValue* pvalues)
{
   ensureFits(count);

   for ( int index = 0; index < count; ++index )
   {
      mStack[mSize++] = pvalues[index];
   }
}

void VirtualStack::push(const VirtualValue& value)
{
   top() = value;
}

void VirtualStack::pushInt(int value)
{
   top().setNumber(value);
}

void VirtualStack::pushReal(float value)
{
   top().setReal(value);
}

void VirtualStack::pushBool(bool value)
{
   top().setBoolean(value);
}

void VirtualStack::pushChar(UChar value)
{
   top().setChar(value);
}

void VirtualStack::pushString(VirtualString& value)
{
   top().setString(value);
}

void VirtualStack::pushObject(VirtualObject& object)
{
   top().setObject(object);
}

void VirtualStack::pushArray(VirtualArray& array)
{
   top().setArray(array);
}

VirtualValue VirtualStack::pop()
{
   return mStack[--mSize];
}

void VirtualStack::pop(int count)
{
   mSize -= count;
   ASSERT(mSize >= 0);
}

bool VirtualStack::popBool()
{
   return mStack[--mSize].asBoolean();
}

int VirtualStack::popInt()
{
   return mStack[--mSize].asNumber();
}

float VirtualStack::popReal()
{
   return mStack[--mSize].asReal();
}

UChar VirtualStack::popChar()
{
   return mStack[--mSize].asChar();
}

const String& VirtualStack::popString()
{
   return mStack[--mSize].asString();
}

VirtualObject& VirtualStack::popObject()
{
   return mStack[--mSize].asObject();
}

VirtualArray& VirtualStack::popArray()
{
   return mStack[--mSize].asArray();
}

VirtualValue& VirtualStack::back()
{
   return mStack[mSize-1];
}

void VirtualStack::insert(int index, const VirtualValue& value)
{
   ensureFits(1);

   mStack.insert(mStack.begin() + index, value);
   /*
   for ( int idx = mSize-1; idx >= index; --idx )
   {
      mStack[idx+1] = mStack[idx];
   }

   mStack[index] = value;
   */

   ++mSize;
}

void VirtualStack::setSize(int size)
{
   mSize = size;
}

// - Query

int VirtualStack::size() const
{
   return mSize;
}

// - Operations

VirtualValue& VirtualStack::top()
{
   ensureFits(1);
   return mStack[mSize++];
}

void VirtualStack::ensureFits(int amount)
{
   if ( mSize + amount >= mStack.size() )
   {
      int oldsize = mStack.size();
      mStack.resize(mStack.size() * 2);
   }
}

void VirtualStack::mark()
{
   for ( int index = 0; index < mSize; index++ )
   {
      VirtualValue& VirtualValue = mStack[index];
      if ( VirtualValue.isObject() )
      {
         VirtualValue.asObject().mark();
      }
      else if ( VirtualValue.isArray() )
      {
         VirtualValue.asArray().mark();
      }
   }
}
