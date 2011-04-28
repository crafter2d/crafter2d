
#ifndef VIRTUAL_NATIVE_OBJECT_H_
#define VIRTUAL_NATIVE_OBJECT_H_

#include "virtualobject.h"

class VirtualNativeObject : public VirtualObject
{
public:
            VirtualNativeObject();
   explicit VirtualNativeObject(void* pobject);
   virtual ~VirtualNativeObject();

 // get/set
   void* getObject();
   void  setObject(void* pobject);

 // downcasting
   virtual bool isNative() const;
   virtual VirtualNativeObject& asNative();

private:
   void* mpObject;
};

#endif // VIRTUAL_NATIVE_OBJECT_H_
