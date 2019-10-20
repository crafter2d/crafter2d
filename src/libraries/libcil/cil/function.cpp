
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

   // - Get/set

   const Class& Function::getClass() const
   {
      ASSERT_PTR(mpClass);
      return *mpClass;
   }
      
   void Function::setClass(Class& klass)
   {
      mpClass = &klass;
   }

   const String& Function::getName() const
   {
      return mName;
   }
   
   void Function::setName(const String& name)
   {
      mName = name;
   }

   const Instructions& Function::getInstructions() const
   {
      return mInstructions;
   }

   void Function::setInstructions(const Instructions& instructions)
   {
      mInstructions = instructions;
   }

   const Guards& Function::getGuards() const
   {
      return mGuards;
   }
      
   void Function::setGuards(const Guards& guards)
   {
      mGuards = guards;
   }

   const SwitchTables& Function::getSwitchTables() const
   {
      return mSwitchTables;
   }

   void Function::setSwitchTables(const SwitchTables& tables)
   {
      mSwitchTables = tables;
   }

   const yasc::Type& Function::getReturnType() const
   {
      return mReturnType;
   }

   void Function::setReturnType(const yasc::Type& type)
   {
      mReturnType = type;
   }

   const StringList& Function::getAnnotations() const
   {
      return mAnnotations;
   }

   const Modifiers& Function::getModifiers() const
   {
      return mModifiers;
   }

   void Function::setModifiers(const Modifiers& modifiers)
   {
      mModifiers = modifiers;
   }

   const yasc::Types& Function::getArguments() const
   {
      return mArguments;
   }

   const yasc::Types& Function::getLocals() const
   {
      return mLocals;
   }

   // - Query

   bool Function::isMemberOf(const Class& klass) const
   {
      return mpClass == &klass;
   }

   // - Operations

   void Function::addArgument(const yasc::Type& type)
   {
      mArguments.add(type);
   }

   void Function::addArgument(yasc::Type&& type)
   {
      mArguments.add(std::move(type));
   }

   void Function::addLocal(const yasc::Type& type)
   {
      mLocals.add(type);
   }

   void Function::addLocal(yasc::Type&& type)
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
