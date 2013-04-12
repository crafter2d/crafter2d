
#ifndef CIL_CLASS_H
#define CIL_CLASS_H

#include <vector>

#include "core/string/string.h"

#include "cil.h"

namespace CIL
{
   class Function;

   class Class
   {
      typedef std::vector<String> Interfaces;

   public:
      typedef std::vector<CIL::Type*> Fields;
      typedef std::vector<Function*> Functions;

      Class();

    // get/set
      void setName(const String& name);
      void setBaseName(const String& name);
      void setModifiers(int modifiers);

      const Fields& getFields() const;
      const Fields& getStaticFields() const;
      const Functions& getFunctions() const;

    // operations
      void addField(CIL::Type* pfield);
      void addStaticField(CIL::Type* pfield);
      void addInterface(const String& interfce);
      void addFunction(Function* pfunction);

   private:

      Functions      mFunctions; // owns
      Interfaces     mInterfaces;
      Fields         mFields;
      Fields         mStatics;
      String         mName;
      String         mBaseName;
      int            mModifiers;
   };
}

#endif // CIL_CLASS_H
