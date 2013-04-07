
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

   public:
      Class();

    // operations
      void addFunction(Function* pfunction);

   private:

      Functions   mFunctions; // owns
      String      mName;
      String      mBaseName;
      Visibility  mVisibility;
      int         mFlags;
   };
}

#endif // CIL_CLASS_H
