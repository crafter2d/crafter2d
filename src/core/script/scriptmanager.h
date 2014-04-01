
#ifndef __ScriptManager_h__
#define __ScriptManager_h__

class String;

namespace c2d
{
   class Script;
   class ScriptObject;
   class ScriptRegistrator;

   class ScriptManager
   {
   public:

      virtual ScriptRegistrator* getRegistrator() = 0;

      virtual ScriptObject* load(const String& classname) = 0;
      virtual ScriptObject* load(const String& classname, void* pobject, bool owned) = 0;

      virtual void addRootObject(ScriptObject& object) = 0;
   };
}

#endif // __ScriptManager_h__
