
#ifndef __YasScriptCall_h__
#define __YasScriptCall_h__

#include "core/script/scriptcall.h"

#include "yasscriptobject.h"

class YasScriptManager;
class VirtualCall;

class YasScriptCall : public c2d::ScriptCall
{
public:
   YasScriptCall(YasScriptManager& manager, VirtualCall& call);

   virtual c2d::ScriptObjectHandle newObject(const String& classname) override;

   virtual bool getBoolean(int arg) override;
   virtual int getInt(int arg) override;
   virtual float getReal(int arg) override;
   virtual UChar getChar(int arg) override;
   virtual const String& getString(int arg) override;
   virtual c2d::ScriptObjectHandle getObject(int arg) override;

   virtual void* useInstance(int arg) override;
   virtual void* getInstance(int arg) override;
   virtual void setInstance(int arg, void* pobject, bool owned) override;

   virtual void setResult(bool value) override;
   virtual void setResult(int value) override;
   virtual void setResult(float value) override;
   virtual void setResult(UChar value) override;
   virtual void setResult(const String& value) override;
   virtual void setResult(Variant value) override;
   virtual void setResult(c2d::ScriptObjectHandle& object) override;
   virtual void setResult(c2d::IScriptable& scriptable, bool owned) override;
   virtual void setResult(const String& classname, void* pobject, bool owned = true) override;

private:

 // data
   YasScriptManager& mManager;
   VirtualCall&      mCall;
};

#endif // __YasScriptCall_h__
