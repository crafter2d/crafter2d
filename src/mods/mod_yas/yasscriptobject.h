
#ifndef __YasScriptObject_h__
#define __YasScriptObject_h__

#include <vector>

#include "core/script/scriptobject.h"

#include "vm/virtualvalue.h"

class Variant;
class VirtualObject;
class YasScriptManager;

class YasScriptObject : public c2d::ScriptObject
{
public:
   YasScriptObject(YasScriptManager& manager);

 // get/set
   VirtualObject& getThis();
   void           setThis(VirtualObject& object);

 // arguments
   virtual void arg(int index, bool arg) override;
   virtual void arg(int index, int arg) override;
   virtual void arg(int index, float arg) override;
   virtual void arg(int index, UChar arg) override;
   virtual void arg(int index, const String& arg) override;
   virtual void arg(int index, void* pinstance) override;
   virtual void arg(int index, const String& classname, void* pinstance) override;

 // members
  virtual void setMember(int index, const Variant& value) override;

 // overrides
   virtual void* getInstance() override;
   virtual void* useInstance() override;
   virtual void  setInstance(void* pinstance, bool owned = true) override;

 // operations
   virtual void prepareCall(int args) override;
   virtual Variant call(const String& function) override;
   virtual Variant call(const String& function, int argc, Variant* args) override;

private:
   typedef std::vector<VirtualValue> Values;

 // conversion
   Variant toVariant(const Variant& value);

 // data
   YasScriptManager& mManager;
   VirtualObject*    mpObject;
   Values            mArguments;
};

#endif // __YasScriptObject_h__
