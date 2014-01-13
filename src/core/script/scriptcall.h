
#ifndef __ScriptCall_h__
#define __ScriptCall_h__

#include "core/string/char.h"
#include "core/string/string.h"

namespace c2d
{
   class ScriptObject;

   class ScriptCall
   {
   public:
      virtual bool getBoolean(int arg) = 0;
      virtual int getInt(int arg) = 0;
      virtual float getReal(int arg) = 0;
      virtual UChar getChar(int arg) = 0;
      virtual const String& getString(int arg) = 0;
      virtual ScriptObject& getObject(int arg) = 0;

      virtual void setResult(bool value) = 0;
      virtual void setResult(int value) = 0;
      virtual void setResult(float value) = 0;
      virtual void setResult(UChar value) = 0;
      virtual void setResult(const String& classname, void* pobject, bool owned = true) = 0;
   };
}

#endif // __ScriptCall_h__
