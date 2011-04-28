
#ifndef VIRTUAL_CLASS_OBJECT_H_
#define VIRTUAL_CLASS_OBJECT_H_

#include "virtualobject.h"

class VirtualClassObject : public VirtualObject
{
public:
   VirtualClassObject();
   virtual ~VirtualClassObject();

   Variant& operator[](int index);

 // operations
   void initialize(int variables);
   void setMember(int index, const Variant& value);

 // downcasting
   virtual bool isInstance() const;
   virtual VirtualClassObject& asInstance();

private:
   Variant* mpMembers;
   int      mMemberCount;
};

#endif // VIRTUAL_CLASS_OBJECT_H_
