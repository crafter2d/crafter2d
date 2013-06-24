
#include "virtualinstruction.h"

static std::string sReserve       = "reserve";
static std::string sNew           = "new";
static std::string sNewArray      = "newarray";
static std::string sCall          = "call";
static std::string sCallSuper     = "callsuper";
static std::string sCallNative    = "callnative";
static std::string sCallStatic    = "callstatic";
static std::string sRet           = "ret";
static std::string sInt2Real      = "int2real";
static std::string sInt2String    = "int2str";
static std::string sReal2String   = "real2str";
static std::string sInt0          = "iconst0";
static std::string sInt1          = "iconst1";
static std::string sReal0         = "rconst0";
static std::string sReal1         = "rconst1";
static std::string sAddInt        = "addi";
static std::string sSubInt        = "subi";
static std::string sMulInt        = "muli";
static std::string sDivInt        = "divi";
static std::string sCmpEqInt      = "cmpeqi";
static std::string sCmpNeqInt     = "cmpneqi";
static std::string sCmpLtInt      = "cmplti";
static std::string sCmpLeInt      = "cmplei";
static std::string sCmpGtInt      = "cmpgti";
static std::string sCmpGeInt      = "cmpgei";
static std::string sAddReal       = "addr";
static std::string sSubReal       = "subr";
static std::string sMulReal       = "mulr";
static std::string sDivReal       = "divr";
static std::string sCmpEqReal     = "cmpeqr";
static std::string sCmpNeqReal    = "cmpneqr";
static std::string sCmpLtReal     = "cmpltr";
static std::string sCmpLeReal     = "cmpler";
static std::string sCmpGtReal     = "cmpgtr";
static std::string sCmpGeReal     = "cmpger";
static std::string sAddString     = "adds";
static std::string sCmpEqString   = "cmpeqs";
static std::string sCmpNeqString  = "cmpneqs";
static std::string sCmpEqBool     = "cmpeqb";
static std::string sCmpNeqBool    = "cmpneqb";
static std::string sNot           = "not";
static std::string sCmpEqObj      = "cmpeqo";
static std::string sCmpNeqObj     = "cmpneqo";
static std::string sCmpEqArray    = "cmpeqa";
static std::string sCmpNeqArray   = "cmpneqa";
static std::string sJump          = "jump";
static std::string sJumpTrue      = "jz";
static std::string sJumpFalse     = "jnz";
static std::string sPush          = "push";
static std::string sPushZero      = "pushzero";
static std::string sPushOne       = "pushone";
static std::string sPushThis      = "pushthis";
static std::string sDup           = "dup";
static std::string sPop           = "pop";
static std::string sInstanceOf    = "instanceof";
static std::string sEnterGuard    = "guard";
static std::string sEnterGuardF   = "guardf";
static std::string sLeaveGuard    = "leaveguard";
static std::string sThrow         = "throw";
static std::string sStore         = "store";
static std::string sLoad          = "load";
static std::string sStoreLocal    = "storelocal";
static std::string sLoadLocal     = "loadlocal";
static std::string sStoreArray    = "storearray";
static std::string sLoadArray     = "loadarray";
static std::string sStoreStatic   = "storestatic";
static std::string sLoadStatic    = "loadstatic";
static std::string sLoadLiteral   = "loadliteral";
static std::string sInvalid       = "stfu";

//static
const std::string& VirtualInstruction::opcodeToString(Opcode opcode)
{
   switch ( opcode )
   {
      case eReserve:       return sReserve;
      case eNew:           return sNew;
      case eNewArray:      return sNewArray;
      case eCall:          return sCall;
      case eCallSuper:     return sCallSuper;
      case eCallNative:    return sCallNative;
      case eCallStatic:    return sCallStatic;
      case eRet:           return sRet;
      case eInt2Real:      return sInt2Real;
      case eInt2String:    return sInt2String;
      case eReal2String:   return sReal2String;
      case eNot:           return sNot;
      case eCmpEqBool:     return sCmpEqBool;
      case eCmpNeqBool:    return sCmpNeqBool;
      case eInt0:          return sInt0;
      case eInt1:          return sInt1;
      case eAddInt:        return sAddInt;
      case eSubInt:        return sSubInt;
      case eMulInt:        return sMulInt;
      case eDivInt:        return sDivInt;
      case eCmpEqInt:      return sCmpEqInt;
      case eCmpNeqInt:     return sCmpNeqInt;
      case eCmpLeInt:      return sCmpLeInt;
      case eCmpLtInt:      return sCmpLtInt;
      case eCmpGeInt:      return sCmpGeInt;
      case eCmpGtInt:      return sCmpGtInt;
      case eReal0:         return sReal0;
      case eReal1:         return sReal1;
      case eAddReal:       return sAddReal;
      case eSubReal:       return sSubReal;
      case eMulReal:       return sMulReal;
      case eDivReal:       return sDivReal;
      case eCmpEqReal:     return sCmpEqReal;
      case eCmpNeqReal:    return sCmpNeqReal;
      case eCmpLeReal:     return sCmpLeReal;
      case eCmpLtReal:     return sCmpLtReal;
      case eCmpGeReal:     return sCmpGeReal;
      case eCmpGtReal:     return sCmpGtReal;
      case eAddStr:        return sAddString;
      case eCmpEqStr:      return sCmpEqString;
      case eCmpNeqStr:     return sCmpNeqString;
      case eCmpEqObj:      return sCmpEqObj;
      case eCmpNeqObj:     return sCmpNeqObj;
      case eCmpEqAr:       return sCmpEqArray;
      case eCmpNeqAr:      return sCmpNeqObj;
      case eJump:          return sJump;
      case eJumpTrue:      return sJumpTrue;
      case eJumpFalse:     return sJumpFalse;
      case ePush:          return sPush;
      case ePushThis:      return sPushThis;
      case eDup:           return sDup;
      case ePop:           return sPop;
      case eInstanceOf:    return sInstanceOf;
      case eEnterGuard:    return sEnterGuard;
      case eEnterGuardF:   return sEnterGuardF;
      case eLeaveGuard:    return sLeaveGuard;
      case eThrow:         return sThrow;
      case eStore:         return sStore;
      case eLoad:          return sLoad;
      case eStoreLocal:    return sStoreLocal;
      case eLoadLocal:     return sLoadLocal;
      case eStoreArray:    return sStoreArray;
      case eLoadArray:     return sLoadArray;
      case eStoreStatic:   return sStoreStatic;
      case eLoadStatic:    return sLoadStatic;
      case eLoadLiteral:   return sLoadLiteral;
      default:             return sInvalid;
   }
}

VirtualInstruction::VirtualInstruction():
   mOpcode(eNop),
   mArgument(-1)
{
}

VirtualInstruction::VirtualInstruction(Opcode opcode, int argument):
   mOpcode(opcode),
   mArgument(argument)
{
}

VirtualInstruction::Opcode VirtualInstruction::getOpcode() const
{
   return mOpcode;
}

void VirtualInstruction::setOpcode(Opcode inst)
{
   mOpcode = inst;
}

int VirtualInstruction::getArgument() const
{
   return mArgument;
}

void VirtualInstruction::setArgument(int index)
{
   mArgument = index;
}

// - Query

bool VirtualInstruction::isNop() const
{
   return mOpcode == VirtualInstruction::eNop;
}

bool VirtualInstruction::isLabel(int label) const
{
   return mOpcode == eLabel && mArgument == label;
}
