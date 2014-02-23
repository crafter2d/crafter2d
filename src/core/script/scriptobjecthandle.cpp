
#include "scriptobjecthandle.h"

#include "scriptobject.h"

namespace c2d
{
   ScriptObjectHandle::~ScriptObjectHandle()
   {
      if ( mOwner )
      {
         delete mpObject;
      }
   }
}
