
#ifndef VIRTUAL_NATIVE_OBJECT_H_
#define VIRTUAL_NATIVE_OBJECT_H_

#include "script/script_base.h"

#include "virtualobject.h"

class VirtualMachine;

class SCRIPT_API VirtualNativeObject : public VirtualObject
{
public:
   VirtualNativeObject(VirtualMachine& machine, void* pobject);
   virtual ~VirtualNativeObject();

 // get/set
   void* getObject();
   void  setObject(void* pobject);

   //void setVirtualMachine(VirtualMachine& machine);
   void setOwned(bool owned);

 // downcasting
   virtual bool isNative() const;
   virtual VirtualNativeObject& asNative();

private:
   VirtualMachine& mMachine;
   void* mpObject;
   bool  mOwned;
};

#endif // VIRTUAL_NATIVE_OBJECT_H_
