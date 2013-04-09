
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
      typedef std::vector<Function*> Functions;
      typedef std::vector<String> Interfaces;

   public:
      Class();

    // get/set
      void setName(const String& name);
      void setBaseName(const String& name);
      void setModifiers(int modifiers);

    // operations
      void addInterface(const String& interfce);
      void addFunction(Function* pfunction);

   private:

      Functions      mFunctions; // owns
      Interfaces     mInterfaces;
      String         mName;
      String         mBaseName;
      int            mModifiers;
   };
}

#endif // CIL_CLASS_H
