
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

   virtual bool getBoolean(int arg) override;
   virtual int getInt(int arg) override;
   virtual float getReal(int arg) override;
   virtual UChar getChar(int arg) override;
   virtual const String& getString(int arg) override;
   virtual c2d::ScriptObject& getObject(int arg) override;

   virtual void setResult(bool value) override;
   virtual void setResult(int value) override;
   virtual void setResult(float value) override;
   virtual void setResult(UChar value) override;
   virtual void setResult(const String& classname, void* pobject, bool owned = true) override;

private:

 // data
   VirtualCall&      mCall;
   YasScriptObject   mObject;
};

#endif // __YasScriptCall_h__
