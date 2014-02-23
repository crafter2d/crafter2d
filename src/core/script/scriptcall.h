
#ifndef __ScriptCall_h__
#define __ScriptCall_h__

#include "core/string/char.h"
#include "core/string/string.h"
#include "core/script/scriptobjecthandle.h"

class Variant;

namespace c2d
{
   class IScriptable;
   class ScriptObject;

   class ScriptCall
   {
   public:
      virtual ScriptObjectHandle newObject(const String& classname) = 0;

      virtual bool getBoolean(int arg) = 0;
      virtual int getInt(int arg) = 0;
      virtual float getReal(int arg) = 0;
      virtual UChar getChar(int arg) = 0;
      virtual const String& getString(int arg) = 0;
      virtual ScriptObjectHandle getObject(int arg) = 0;

      virtual void setResult(bool value) = 0;
      virtual void setResult(int value) = 0;
      virtual void setResult(float value) = 0;
      virtual void setResult(UChar value) = 0;
      virtual void setResult(const String& value) = 0;
      virtual void setResult(Variant value) = 0;
      virtual void setResult(ScriptObjectHandle& object) = 0;
      virtual void setResult(IScriptable& scriptable, bool owned) = 0;
      virtual void setResult(const String& classname, void* pobject, bool owned = true) = 0;
   };
}

#endif // __ScriptCall_h__
