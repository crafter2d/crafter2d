
#ifndef CODEGEN_RESOLVER_H
#define CODEGEN_RESOLVER_H

#include "core/string/string.h"

namespace CIL
{
   class Class;
   class ClassMap;
   class Function;

   class Resolver
   {
   public:

      Class*    resolveClass(ClassMap& classes, const String& classname);
      Function* resolveFunction(ClassMap& classes, const String& function);
   };
}

#endif // CODEGEN_RESOLVER_H
