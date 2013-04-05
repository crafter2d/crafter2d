
#ifndef COMPILED_CLASS_H
#define COMPILED_CLASS_H

#include <vector>

#include "core/string/string.h"

class CompiledFunction;

class CompiledClass
{
   typedef std::vector<CompiledFunction*> Functions;

public:
   enum Flags {
      eInterface  = 1,
      eNative     = 2
   };

   CompiledClass();

 // operations
   void addFunction(CompiledFunction* pfunction);

private:

   Functions   mFunctions; // owns
   String      mName;
   String      mBaseName;
   int         mFlags;
};

#endif // COMPILED_CLASS_H
