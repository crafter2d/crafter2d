
#ifndef __YasScriptManager_h__
#define __YasScriptManager_h__

#include "core/script/scriptmanager.h"

#include "script/vm/virtualcontext.h"
#include "script/vm/virtualmachine.h"

class YasScriptObject;

class YasScriptManager : public c2d::ScriptManager
{
public:
   YasScriptManager();

 // get/set
   VirtualMachine& getVM();

 // overrides
   virtual c2d::Script* load(const String& classname, void* pobject, bool owned) override;
   virtual void addRootObject(c2d::ScriptObject& object) override;

 // operations
   void registerObject(YasScriptObject& object);
   
private:

 // data
   VirtualContext    mVirtualContext;
   VirtualMachine    mVirtualMachine;
};

#endif // __YasScriptManager_h__
