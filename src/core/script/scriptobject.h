
#ifndef __ScriptObject_h__
#define __ScriptObject_h__

#include "core/string/char.h"

class String;

namespace c2d
{
   class ScriptObject
   {
   public:
    // arguments
      virtual void arg(int index, bool arg) = 0;
      virtual void arg(int index, int arg) = 0;
      virtual void arg(int index, float arg) = 0;
      virtual void arg(int index, UChar arg) = 0;
      virtual void arg(int index, const String& arg) = 0;

    // instance
      virtual void* getInstance() = 0;
      virtual void* useInstance() = 0;
      virtual void  setInstance(void* pinstance, bool owned = true) = 0;

    // operations
      virtual void prepareCall(int args) = 0;
      virtual void call(const String& function) = 0;
   };
}

#endif // __ScriptObject_h__
