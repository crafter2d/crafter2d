
#include "core/defines.h"

#include "function.h"

namespace CIL
{

   Function::Function():
       mName(),
       mArgs(),
       mLocals(),
       mModifiers(0)
   {
   }

   // - Query
      
   const Types& Function::getArguments() const
   {
      return mArgs;
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

   void Function::setReturnType(Type* ptype)
   {
      mpReturnType = ptype;
   }

   void Function::addArgument(Type* parg)
   {
      mArgs.push_back(parg);
   }

   void Function::addLocal(Type* plocal)
   {
      mLocals.push_back(plocal);
   }

} // namespace CIL
