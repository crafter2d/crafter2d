
#ifndef VIRTUAL_INSTRUCTION_H_
#define VIRTUAL_INSTRUCTION_H_

#include <string>

class VirtualInstruction
{
public:
   enum Instruction
   {
      eNop,
      eReserve,
      eNew,
      eNewArray,
      eNewNative,
      eDup,
      eCall,
      eCallSuper,
      eCallNative,
      eCallStatic,
      eCallInterface,
      eRet,
      eInt2Real,
      eInt2String,
      eReal2Int,
      eReal2String,
      eInt0,
      eInt1,
      eInt2,
      eAddInt,
      eSubInt,
      eMulInt,
      eDivInt,
      eRemInt,
      eOrInt,
      eXorInt,
      eAndInt,
      eShiftLeftInt,
      eShiftRightInt,
      eIntNegate,
      eReal0,
      eReal1,
      eReal2,
      eAddReal,
      eSubReal,
      eMulReal,
      eDivReal,
      eRemReal,
      eRealNegate,
      eAddStr,
      eNot,
      eCmpEqual,
      eCmpNotEqual,
      eCmpSEqual,
      eCmpSmaller,
      eCmpGreater,
      eCmpGEqual,
      eJump,
      eJumpTrue,
      eJumpFalse,
      ePush,
      ePushTrue,
      ePushFalse,
      ePushThis,
      ePop,
      eInstanceOf,
      eEnterGuard,
      eEnterGuardF,
      eLeaveGuard,
      eThrow,
      eStore,
      eLoad,
      eStoreLocal,
      eStoreArray,
      eStoreStatic,
      eLoadLocal,
      eLoadArray,
      eLoadStatic,
      eLoadLiteral,
      eLoadClass
   };

   static const std::string& instructionToString(Instruction inst);

   VirtualInstruction();
   VirtualInstruction(Instruction instruction, int argument);

 // get/set
   Instruction getInstruction() const;
   
   int         getArgument() const;
   void        setArgument(int index);

private:
   Instruction mInstruction;
   int         mArgument;
};

#endif // VIRTUAL_INSTRUCTION_H_
