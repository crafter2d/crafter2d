
#ifndef __YasScriptManager_h__
#define __YasScriptManager_h__

#include "core/script/scriptmanager.h"

#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualmachine.h"

class Variant;
class YasScriptObject;

class YasScriptManager : public c2d::ScriptManager
{
public:
   YasScriptManager();

 // get/set
   VirtualMachine& getVM();

 // overrides
   virtual c2d::ScriptRegistrator* getRegistrator() override;
   virtual c2d::ScriptObject* load(const String& classname) override;
   virtual c2d::ScriptObject* load(const String& classname, void* pobject, bool owned) override;
   virtual void addRootObject(c2d::ScriptObject& object) override;

 // operations
   void registerObject(YasScriptObject& object);

 // execution
   Variant execute(VirtualObject& object, const String& function, int argc, VirtualValue* pargv);

 // conversion
   Variant toVariant(const VirtualValue& value);
   VirtualValue toValue(const Variant& value);
   
private:

 // data
   VirtualContext    mVirtualContext;
   VirtualMachine    mVirtualMachine;
};

#endif // __YasScriptManager_h__
