
#include "virtualstack.h"

#include "core/defines.h"

#include "virtualarray.h"
#include "virtualobject.h"
#include "virtualstring.h"

VirtualStack::VirtualStack(int initialsize):
   mStack(),
   mSize(0)
{
   mStack.resize(initialsize);
   fill(0);
}

VirtualStack::~VirtualStack()
{
   for ( Stack::size_type index = 0; index < mStack.size(); ++index )
   {
      delete mStack[index];
   }
   mStack.clear();
}

const Variant& VirtualStack::operator[](int index) const
{
   ASSERT(index < mSize);
   return *mStack[index];
}

Variant& VirtualStack::operator[](int index)
{
   ASSERT(index < mSize);
   return *mStack[index];
}

// - Stack operations

void VirtualStack::push(int count, Variant* pvalues)
{
   ensureFits(count);

   for ( int index = 0; index < count; ++index )
   {
      *mStack[mSize++] = pvalues[index];
   }
}

void VirtualStack::push(const Variant& value)
{
   top() = value;
}

void VirtualStack::pushInt(int value)
{
   top().setInt(value);
}

void VirtualStack::pushReal(double value)
{
   top().setReal(value);
}

void VirtualStack::pushBool(bool value)
{
   top().setBool(value);
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

Variant VirtualStack::pop()
{
   return *mStack[--mSize];
}

void VirtualStack::pop(int count)
{
   mSize -= count;
   ASSERT(mSize >= 0);
}

int VirtualStack::popInt()
{
   return mStack[--mSize]->asInt();
}

double VirtualStack::popReal()
{
   return mStack[--mSize]->asReal();
}

bool VirtualStack::popBool()
{
   return mStack[--mSize]->asBool();
}

UChar VirtualStack::popChar()
{
   return mStack[--mSize]->asChar();
}

const String& VirtualStack::popString()
{
   return mStack[--mSize]->asString().getString();
}

VirtualObject& VirtualStack::popObject()
{
   return mStack[--mSize]->asObject();
}

VirtualArray& VirtualStack::popArray()
{
   return mStack[--mSize]->asArray();
}

Variant& VirtualStack::back()
{
   return *mStack[mSize-1];
}

void VirtualStack::insert(int index, const Variant& value)
{
   ensureFits(1);

   for ( int idx = mSize-1; idx >= index; --idx )
   {
      mStack[idx+1] = mStack[idx];
   }

   mStack[index] = new Variant(value);

   ++mSize;
}

// - Query

int VirtualStack::size() const
{
   return mSize;
}

// - Operations

Variant& VirtualStack::top()
{
   ensureFits(1);
   return *mStack[mSize++];
}

void VirtualStack::ensureFits(int amount)
{
   if ( mSize + amount >= mStack.size() )
   {
      int oldsize = mStack.size();
      mStack.resize(mStack.size() * 2);
      fill(oldsize);
   }
}

void VirtualStack::fill(int from)
{
   for ( std::size_t index = from; index < mStack.size(); index++ )
   {
      mStack[index] = new Variant();
   }
}

void VirtualStack::mark()
{
   for ( int index = 0; index < mSize; index++ )
   {
      Variant& variant = *mStack[index];
      if ( variant.isObject() )
      {
         variant.asObject().mark();
      }
      else if ( variant.isArray() )
      {
         variant.asArray().mark();
      }
   }
}
