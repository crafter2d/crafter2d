
#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

#include "core/string/string.h"
#include "cil.h"

namespace CIL
{
   class Function
   {
   public:
      Function();

    // operations
      void setName(const String& name);
      void setInstructions(const Instructions& instructions);
      void setModifiers(int modifiers);
      void setReturnType(Type* ptype);

      void addArgument(Type* parg);
      void addLocal(Type* ploc);

   private:

      String         mName;
      Instructions   mInstructions;
      Types          mArgs;
      Types          mLocals;
      Type*          mpReturnType;
      int            mModifiers;
   };
}

#endif // FUNCTION_H
