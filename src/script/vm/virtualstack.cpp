
#include "virtualstack.h"

#include "core/defines.h"

#include "virtualarrayreference.h"
#include "virtualobject.h"

VirtualStack::VirtualStack(int initialsize):
   mStack(),
   mSize(0)
{
   mStack.resize(initialsize);
}

const Variant& VirtualStack::operator[](int index) const
{
   ASSERT(index < mSize);
   return mStack[index];
}

Variant& VirtualStack::operator[](int index)
{
   ASSERT(index < mSize);
   return mStack[index];
}

// - Stack operations

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

void VirtualStack::pushChar(char value)
{
   top().setChar(value);
}

void VirtualStack::pushString(const std::string& value)
{
   top().setString(value);
}

void VirtualStack::pushObject(VirtualObject& object)
{
   top().setObject(object);
}

void VirtualStack::pushArray(const VirtualArrayReference& array)
{
   top().setArray(array);
}

void VirtualStack::pop(int count)
{
   mSize -= count;
   ASSERT(mSize >= 0);
}

int VirtualStack::popInt()
{
   return mStack[--mSize].asInt();
}

double VirtualStack::popReal()
{
   return mStack[--mSize].asReal();
}

bool VirtualStack::popBool()
{
   return mStack[--mSize].asBool();
}

char VirtualStack::popChar()
{
   return mStack[--mSize].asChar();
}

const std::string& VirtualStack::popString()
{
   return mStack[--mSize].asString();
}

VirtualObject& VirtualStack::popObject()
{
   return mStack[--mSize].asObject();
}

VirtualArrayReference& VirtualStack::popArray()
{
   return mStack[--mSize].asArray();
}

Variant& VirtualStack::back()
{
   return mStack[mSize-1];
}

void VirtualStack::insert(int index, const Variant& value)
{
   mStack.insert(mStack.begin() + index, value);
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
   if ( mSize + 1 >= mStack.size() )
   {
      mStack.resize(mStack.size() * 2);
   }
   return mStack[mSize++];
}

void VirtualStack::mark()
{
   for ( int index = 0; index < mSize; index++ )
   {
      Variant& variant = mStack[index];
      if ( variant.isObject() )
      {
         variant.asObject().mark();
      }
      else if ( variant.isArray() )
      {
         variant.asArray()->mark();
      }
   }
}
