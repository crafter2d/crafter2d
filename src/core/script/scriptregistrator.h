
#ifndef SCRIPT_OBJECT_H_
#define SCRIPT_OBJECT_H_

#include <vector>

#include "core/string/string.h"
#include "core/defines.h"

namespace c2d
{
   class ScriptCall;
   class ScriptManager;

   class ScriptRegistrator
   {
   public:

      typedef void(*CallbackFnc)(ScriptCall& call);

      // operations
      virtual void addClass(const String& name) = 0;

      virtual void addFunction(const String& prototype, CallbackFnc callback) = 0;

      virtual void registerCallbacks(ScriptManager& manager) = 0;
   };
}

#endif // SCRIPT_OBJECT_H_
