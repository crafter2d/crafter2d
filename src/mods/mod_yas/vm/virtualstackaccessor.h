
#ifndef VIRTUAL_STACK_ACCESSOR_H
#define VIRTUAL_STACK_ACCESSOR_H

#include "core/defines.h"

#include "virtualstack.h"
#include "virtualstring.h"
#include "virtualcontext.h"
#include "virtualmachine.h"

class VirtualCall
{
public:
   VirtualCall(VirtualMachine& machine, Variant* pargs, int arguments): mMachine(machine), mpArguments(pargs), mSize(arguments), mHasResult(false)
   {
   }

 // get/set
   VirtualMachine& getMachine() {
      return mMachine;
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

   const String& getString(int argument) const {
      Variant& value = getArgument(argument);
      return value.asString().getString();
   }

   UChar getChar(int argument) const {
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
      return mHasResult;
   }

   Variant& getResult() {
      return mResult;
   }

   void setResult(VirtualObject& object) {
      setResult(Variant(object));
   }

   void setResult(VirtualArray& array) {
      setResult(Variant(array));
   }

   void setResult(int value) {
      setResult(Variant(value));
   }

   void setResult(double value) {
      setResult(Variant(value));
   }

   void setResult(bool value) {
      setResult(Variant(value));
   }

   void setResult(UChar value) {
      setResult(Variant(value));
   }

   void setResult(const String& value) {
      setResult(Variant(mMachine.getContext().mStringCache.lookup(value)));
   }

   void setResult(const Variant& value) {
      mResult = value;
      mHasResult = true;
   }

private:
   Variant& getArgument(int index) const {
      ASSERT(index <= mSize);
      return mpArguments[index];
      // 0 1 2 3 -> ssize = 4; size = 3
      // index 0 -> 4 - 3 = 1
      //return mStack[mStack.size() - mSize + index];
   }

   VirtualMachine&   mMachine;
   Variant*          mpArguments;
   Variant           mResult;
   int               mSize;
   bool              mHasResult;
};

#endif // VIRTUAL_STACK_ACCESSOR_H
