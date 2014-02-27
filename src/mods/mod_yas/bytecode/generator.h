
#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include "resolver.h"

class String;
class VirtualClass;

namespace ByteCode
{
   /// Byte code generator for the given CPU.

   class Generator
   {
   public:
      Generator();
      ~Generator();

      VirtualClass& generate(VirtualContext& context, const String& classname);

   private:
    // loading
      VirtualClass* loadClass(VirtualContext& context, const String& classname);
   };
}

#endif // BYTECODE_GENERATOR_H
