
#include "cil.h"

#include "core/string/string.h"

namespace CIL
{
   bool Type::match(const Type& that) const
   {
      if ( type == that.type )
      {
         switch ( type )
         {
            case eObject:
               return *name == *that.name;
            case eArray:
               return elem_type->match(*that.elem_type);
            default:
               return true;
         }
      }

      return false;
   }
}
