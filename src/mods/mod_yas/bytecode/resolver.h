
#ifndef FUNCTION_RESOLVER_H
#define FUNCTION_RESOLVER_H

#include <map>

class String;
class VirtualClass;
class VirtualField;
class VirtualFunction;
class VirtualContext;

/**
 * The resolver is used by the bytecode generator to resolve classes
 *
 * ClassResolver --> if class not exist, use generator to build the virtual class
 */

namespace ByteCode
{
   class Resolver
   {
   public:
      explicit Resolver(VirtualContext& context);

    // interface
      VirtualClass& resolveClass(const String& classname);
      VirtualFunction& resolveFunction(const String& prototype);
      VirtualField& resolveField(const String& name);
      VirtualField& resolveStaticField(const String& name);

   private:

    // data
      VirtualContext& mContext;
   };
}

#endif // FUNCTION_RESOLVER_H
