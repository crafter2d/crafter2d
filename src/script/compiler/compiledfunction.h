
#ifndef COMPILED_FUNCTION_H
#define COMPILED_FUNCTION_H

#include <vector>

#include "core/string/string.h"
#include "cil.h"

class CompiledFunction
{
   typedef std::vector<CIL::Type*> Types;

public:
   enum Flags {
      eStatic     = 1,
      eAbstract   = 2,
      eVirtual    = 4,
      eNative     = 8
   };

   CompiledFunction();

 // operations
   void setName(const String& name);
   void setRetType(CIL::Type* ptype);
   void addArg(CIL::Type* ptype);
   void addLocal(CIL::Type* ptype);

private:

   String      mName;
   Types       mArgs;
   Types       mLocals;
   CIL::Type*  mpRet;
   int         mFlags;
};

#endif // COMPILED_FUNCTION_H
