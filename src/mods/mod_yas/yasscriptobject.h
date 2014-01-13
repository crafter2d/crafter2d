
#ifndef __YasScriptObject_h__
#define __YasScriptObject_h__

#include <vector>

#include "core/script/scriptobject.h"
#include "script/common/variant.h"

class VirtualObject;
class YasScriptManager;

class YasScriptObject : public c2d::ScriptObject
{
public:
   YasScriptObject(YasScriptManager& manager);

 // get/set
   VirtualObject& getObject();
   void           setObject(VirtualObject& object);

 // arguments
   virtual void arg(int index, bool arg) override;
   virtual void arg(int index, int arg) override;
   virtual void arg(int index, float arg) override;
   virtual void arg(int index, UChar arg) override;
   virtual void arg(int index, const String& arg) override;

 // overrides
   virtual void* getInstance() override;
   virtual void* useInstance() override;
   virtual void  setInstance(void* pinstance, bool owned = true) override;

 // operations
   virtual void prepareCall(int args);
   virtual void call(const String& function);

private:
   typedef std::vector<Variant> Variants;

 // data
   YasScriptManager& mManager;
   VirtualObject*    mpObject;
   Variants          mArguments;
};

#endif // __YasScriptObject_h__
