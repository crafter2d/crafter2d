
#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>

#include "core/string/string.h"

#include "cil.h"
#include "signature.h"

namespace CIL
{
   class Function
   {
   public:
      Function();

    // query
      const Signature& getSignature() const;
      const Types& getLocals() const;

    // operations
      void setName(const String& name);

      const Instructions& getInstructions() const;
      void                setInstructions(const Instructions& instructions);

      void setModifiers(int modifiers);

      const Type& getReturnType() const;
      void        setReturnType(Type* ptype);

      void addArgument(Type* parg);
      void addLocal(Type* ploc);

   private:

      String         mName;
      Instructions   mInstructions;
      Signature      mSignature;
      Types          mLocals;
      Type*          mpReturnType;
      int            mModifiers;
   };
}

#endif // FUNCTION_H
