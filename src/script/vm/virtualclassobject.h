
#ifndef VIRTUAL_CLASS_OBJECT_H_
#define VIRTUAL_CLASS_OBJECT_H_

#include "virtualobject.h"

class VirtualClassObject : public VirtualObject
{
public:
   VirtualClassObject();
   virtual ~VirtualClassObject();

 // downcasting
   virtual bool isInstance() const;
   virtual VirtualClassObject& asInstance();
};

#endif // VIRTUAL_CLASS_OBJECT_H_
