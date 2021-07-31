
#include "function.h"

#include "core/defines.h"

#include "../common/type.h"

namespace CIL
{
   Function::Function():
      mpClass(nullptr),
      mArguments(),
      mLocals(),
      mReturnType(),
      mAnnotations(),
      mName(),
      mInstructions(),
      mGuards(),
      mSwitchTables(),
      mModifiers()
   {
   }

   Function::~Function()
   {
   }

   // - Query

   bool Function::isMemberOf(const Class& klass) const
   {
      return mpClass == &klass;
   }

   // - Operations

   void Function::addArgument(const Type& type)
   {
      mArguments.add(type);
   }

   void Function::addArgument(Type&& type)
   {
      mArguments.add(std::move(type));
   }

   void Function::addLocal(const Type& type)
   {
      mLocals.add(type);
   }

   void Function::addLocal(Type&& type)
   {
      mLocals.add(std::move(type));
   }

   void Function::addGuard(Guard* pguard)
   {
      mGuards.add(pguard);
   }

   void Function::addSwitchTable(SwitchTable* ptable)
   {
      mSwitchTables.add(ptable);
   }

   void Function::addAnnotation(const String& annotation)
   {
      mAnnotations.add(annotation);
   }

} // namespace CIL
