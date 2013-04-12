
#ifndef VIRTUAL_INSTRUCTION_H_
#define VIRTUAL_INSTRUCTION_H_

#include <string>

class VirtualInstruction
{
public:
   enum Opcode
   {
      eNop,
      eReserve,
      eLabel,
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
      eString2Int,
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
      eShiftLeftInt,
      eShiftRightInt,
      eXorInt,
      eOrInt,
      eAndInt,
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

   static const std::string& opcodeToString(Opcode inst);

   VirtualInstruction();
   VirtualInstruction(Opcode instruction, int argument);

 // get/set
   Opcode getOpcode() const;
   void   setOpcode(Opcode inst);
   
   int         getArgument() const;
   void        setArgument(int index);

 // query
   bool isNop() const;
   bool isLabel(int label) const;

private:
   Opcode   mOpcode;
   int      mArgument;
};

#endif // VIRTUAL_INSTRUCTION_H_
