
#ifndef CIL_FUNCTION_H
#define CIL_FUNCTION_H

#include "core/defines.h"
#include "core/string/string.h"
#include "core/string/stringlist.h"

#include "../common/types.h"
#include "../common/modifiers.h"

#include "cil.h"
#include "guards.h"
#include "instructions.h"
#include "switchtables.h"

namespace CIL
{
   class Class;

   class Function
   {
   public:
      Function();
      ~Function();

    // get/set
      const Class& getClass() const { 
         ASSERT_PTR(mpClass);
         return *mpClass;
      }
      void         setClass(Class& klass) { mpClass = &klass; }

      const String& getName() const { return mName; }
      void          setName(const String& name) { mName = name; }

      const Instructions& getInstructions() const { return mInstructions; }
      void                setInstructions(const Instructions& instructions) { mInstructions = instructions; }

      const Guards& getGuards() const { return mGuards; }
      void          setGuards(const Guards& guards) { mGuards = guards; }

      const SwitchTables& getSwitchTables() const { return mSwitchTables; }
      void                setSwitchTables(const SwitchTables& tables) { mSwitchTables = tables; }

      const Type& getReturnType() const { return mReturnType; }
      void        setReturnType(Type&& type) { mReturnType = std::move(type); }

      const StringList& getAnnotations() const { return mAnnotations; }

      const Modifiers& getModifiers() const { return mModifiers; }
      void             setModifiers(const Modifiers& modifiers) { mModifiers = modifiers; }

      const Types& getArguments() const { return mArguments; }
      const Types& getLocals() const { return mLocals; }

    // query
      bool isMemberOf(const Class& klass) const;

    // operation
      void addArgument(const Type& type);
      void addArgument(Type&& type);
      void addLocal(const Type& type);
      void addLocal(Type&& type);
      void addGuard(Guard* pguard);
      void addSwitchTable(SwitchTable* ptable);
      void addAnnotation(const String& annotation);

   private:

    // data
      Class*         mpClass;
      Types          mArguments;
      Types          mLocals;
      Type           mReturnType;
      StringList     mAnnotations;
      String         mName;
      Instructions   mInstructions;
      Guards         mGuards;
      SwitchTables   mSwitchTables;
      Modifiers      mModifiers;
   };
}

#endif // CIL_FUNCTION_H
