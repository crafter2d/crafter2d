
#ifndef CIL_FUNCTION_H
#define CIL_FUNCTION_H

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
      const Class& getClass() const;
      void         setClass(Class& klass);

      const String& getName() const;
      void          setName(const String& name);

      const Instructions& getInstructions() const;
      void                setInstructions(const Instructions& instructions);

      const Guards& getGuards() const;
      void          setGuards(const Guards& guards);

      const SwitchTables& getSwitchTables() const;
      void                setSwitchTables(const SwitchTables& tables);

      const yasc::Type& getReturnType() const;
      void              setReturnType(const yasc::Type& type);

      const StringList& getAnnotations() const;

      const Modifiers& getModifiers() const;
      void             setModifiers(const Modifiers& modifiers);

      const yasc::Types& getArguments() const;
      const yasc::Types& getLocals() const;

    // query
      bool isMemberOf(const Class& klass) const;

    // operation
      void addArgument(const yasc::Type& type);
      void addLocal(const yasc::Type& type);
      void addGuard(Guard* pguard);
      void addSwitchTable(SwitchTable* ptable);
      void addAnnotation(const String& annotation);

   private:

    // data
      Class*         mpClass;
      yasc::Types    mArguments;
      yasc::Types    mLocals;
      yasc::Type     mReturnType;
      StringList     mAnnotations;
      String         mName;
      Instructions   mInstructions;
      Guards         mGuards;
      SwitchTables   mSwitchTables;
      Modifiers      mModifiers;
   };
}
#endif // CIL_FUNCTION_H
