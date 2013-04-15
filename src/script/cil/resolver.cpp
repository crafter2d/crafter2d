
#include "resolver.h"

namespace CIL
{

   Class* Resolver::resolveClass(ClassMap& classes, const String& classname)
   {
      return NULL;
   }

   Function* Resolver::resolveFunction(ClassMap& classes, const String& name)
   {
      int index = name.lastIndexOf('.');
      String classname = name.subStr(0, index);
      String func = name.subStr(index + 1, name.length() - index);

      Function* pfunction = NULL;
      Class* pclass = resolveClass(classes, classname);
      if ( pclass != NULL )
      {

      }

      return pfunction;
   }



} // namespace CIL