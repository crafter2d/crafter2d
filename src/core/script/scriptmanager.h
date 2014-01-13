
#ifndef __ScriptManager_h__
#define __ScriptManager_h__

class String;

namespace c2d
{
   class Script;
   class ScriptObject;

   class ScriptManager
   {
   public:

      virtual Script* load(const String& classname, void* pobject, bool owned) = 0;

      virtual void addRootObject(ScriptObject& object) = 0;
   };
}

#endif // __ScriptManager_h__
