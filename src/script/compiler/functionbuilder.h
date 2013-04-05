
#ifndef FUNCTION_BUILDER_H
#define FUNCTION_BUILDER_H

#include <vector>

#include "cil.h"

class String;

class FunctionBuilder
{
   typedef std::vector<CIL::Instruction> Instructions;

public:
   FunctionBuilder();

 // label operations
   int  allocateLabel();

 // builder operations
   void addPush(int arg);
   void addPop(int arg);
   void addLoadLocal(int arg);
   void addStoreLocal(int arg);
   void addNew(const String& arg);
   void addNewArray(int arg);
   void addLoadElem(int arg);
   void addStoreElem(int arg);
   void addJump(int label);
   void addJumpTrue(int label);
   void addJumpFalse(int label);

private:

   Instructions   mInstructions;
   int            mLabel;
};

#endif // FUNCTION_BUILDER_H
