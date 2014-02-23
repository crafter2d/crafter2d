
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
   VirtualCall(VirtualMachine& machine, VirtualValue* pargs, int arguments): mMachine(machine), mpArguments(pargs), mSize(arguments), mHasResult(false)
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
      return getArgument(argument).asNumber();
   }

   double getReal(int argument) const {
      return getArgument(argument).asReal();
   }

   const String& getString(int argument) const {
      return getArgument(argument).asString();
   }

   UChar getChar(int argument) const {
      return getArgument(argument).asChar();
   }

   bool getBoolean(int argument) const {
      return getArgument(argument).asBoolean();
   }

   VirtualObject& getObject(int argument) const {
      return getArgument(argument).asObject();
   }

   VirtualArray& getArray(int argument) const {
      return getArgument(argument).asArray();
   }

 // return value
   bool hasResult() {
      return mHasResult;
   }

   VirtualValue& getResult() {
      return mResult;
   }

   void setResult(VirtualObject& object) {
      setResult(VirtualValue(object));
   }

   void setResult(VirtualArray& array) {
      setResult(VirtualValue(array));
   }

   void setResult(int value) {
      setResult(VirtualValue(value));
   }

   void setResult(double value) {
      setResult(VirtualValue(value));
   }

   void setResult(bool value) {
      setResult(VirtualValue(value));
   }

   void setResult(UChar value) {
      setResult(VirtualValue(value));
   }

   void setResult(const String& value) {
      setResult(VirtualValue(mMachine.getContext().mStringCache.lookup(value)));
   }

   void setResult(const VirtualValue& value) {
      mResult = value;
      mHasResult = true;
   }

private:
   VirtualValue& getArgument(int index) const {
      ASSERT(index <= mSize);
      return mpArguments[index];
      // 0 1 2 3 -> ssize = 4; size = 3
      // index 0 -> 4 - 3 = 1
      //return mStack[mStack.size() - mSize + index];
   }

   VirtualMachine&   mMachine;
   VirtualValue*     mpArguments;
   VirtualValue      mResult;
   int               mSize;
   bool              mHasResult;
};

#endif // VIRTUAL_STACK_ACCESSOR_H
