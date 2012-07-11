
#ifndef VIRTUAL_STACK_ACCESSOR_H
#define VIRTUAL_STACK_ACCESSOR_H

#include "core/defines.h"

#include "virtualstack.h"

class VirtualStackAccessor
{
public:
   VirtualStackAccessor(VirtualStack& stack): mStack(stack), mSize(stack.back().asInt())
   {
   }

 // query
   VirtualObject& getThis() const
   {
      return getObject(0);
   }

   int getInt(int argument) const {
      Variant& value = getArgument(argument);
      return value.asInt();
   }

   double getReal(int argument) const {
      Variant& value = getArgument(argument);
      return value.asReal();
   }

   const std::string& getString(int argument) const {
      Variant& value = getArgument(argument);
      return value.asString();
   }

   char getChar(int argument) const {
      Variant& value = getArgument(argument);
      return value.asChar();
   }

   bool getBoolean(int argument) const {
      Variant& value = getArgument(argument);
      return value.asBool();
   }

   VirtualObject& getObject(int argument) const {
      Variant& value = getArgument(argument);
      return value.asObject();
   }

   VirtualArray& getArray(int argument) const {
      Variant& value = getArgument(argument);
      return value.asArray();
   }

 // return value
   bool hasResult() {
      return !mResult.isEmpty();
   }

   Variant& getResult() {
      return mResult;
   }

   void setResult(VirtualObject& object) {
      mResult = Variant(object);
   }

   void setResult(VirtualArray& array) {
      mResult = Variant(array);
   }

   void setResult(int value) {
      mResult = Variant(value);
   }

   void setResult(double value) {
      mResult = Variant(value);
   }

   void setResult(bool value) {
      mResult = Variant(value);
   }

   void setResult(char value) {
      mResult = Variant(value);
   }

   void setResult(const std::string& value) {
      mResult = Variant(value);
   }

   void setResult(const Variant& value) {
      mResult = value;
   }

private:
   Variant& getArgument(int index) const {
      ASSERT(index <= mSize);
      return mStack[mStack.size() - (mSize + 1) + index];
      // 0 1 2 3 -> ssize = 4; size = 3
      // index 0 -> 4 - 3 = 1
   }

   VirtualStack&  mStack;
   Variant        mResult;
   int            mSize;
};

#endif // VIRTUAL_STACK_ACCESSOR_H
