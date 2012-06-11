
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
      eChar2String,
      eBoolean2String,
      eNot,
      eCmpEqBool,
      eCmpNeqBool,
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
      eCmpEqInt,
      eCmpNeqInt,
      eCmpLtInt,
      eCmpLeInt,
      eCmpGtInt,
      eCmpGeInt,
      eReal0,
      eReal1,
      eReal2,
      eAddReal,
      eSubReal,
      eMulReal,
      eDivReal,
      eRemReal,
      eRealNegate,
      eCmpEqReal,
      eCmpNeqReal,
      eCmpLtReal,
      eCmpLeReal,
      eCmpGtReal,
      eCmpGeReal,
      eAddChar,
      eCmpEqChar,
      eCmpNeqChar,
      eAddStr,
      eCmpEqStr,
      eCmpNeqStr,
      eCmpLtStr,
      eCmpLeStr,
      eCmpGtStr,
      eCmpGeStr,
      eCmpEqObj,
      eCmpNeqObj,
      eCmpEqAr,
      eCmpNeqAr,
      eIsNull,
      eLookup,
      eJump,
      eJumpTrue,
      eJumpFalse,
      ePush,
      ePushTrue,
      ePushFalse,
      ePushThis,
      ePushNull,
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
