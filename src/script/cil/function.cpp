
#include "core/defines.h"

#include "function.h"

namespace CIL
{

   Function::Function():
       mName(),
       mSignature(),
       mLocals(),
       mModifiers(0)
   {
   }

   // - Query
      
   const Signature& Function::getSignature() const
   {
      return mSignature;
   }

   const Types& Function::getLocals() const
   {
      return mLocals;
   }

   // - Operations

   void Function::setName(const String& name)
   {
      mName = name;
   }

   const Instructions& Function::getInstructions() const
   {
      return mInstructions;
   }

   void Function::setInstructions(const CIL::Instructions& instructions)
   {
      mInstructions = instructions;
   }

   void Function::setModifiers(int modifiers)
   {
      mModifiers = modifiers;
   }

   const Type& Function::getReturnType() const
   {
      ASSERT_PTR(mpReturnType);
      return *mpReturnType;
   }

   void Function::setReturnType(Type* ptype)
   {
      mpReturnType = ptype;
   }

   void Function::addArgument(Type* parg)
   {
      mSignature.addArgument(parg);
   }

   void Function::addLocal(Type* plocal)
   {
      mLocals.push_back(plocal);
   }

} // namespace CIL
