
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
      void setSignature(const Signature& signature);
      void setInstructions(const Instructions& instructions);
      void setLocals(const Types& locals);
      void setVisibility(Visibility visibility);

   private:

      String         mName;
      Signature      mSignature;
      Instructions   mInstructions;
      Types          mArgs;
      Types          mLocals;
      Visibility     mVisibility;
      int            mFlags;
   };
}

#endif // FUNCTION_H
