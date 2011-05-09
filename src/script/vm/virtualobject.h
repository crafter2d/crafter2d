
#ifndef VIRTUAL_OBJECT_H_
#define VIRTUAL_OBJECT_H_

#include "script/script_base.h"

class Variant;
class VirtualClass;
class VirtualClassObject;
class VirtualNativeObject;

class SCRIPT_API VirtualObject
{
public:
   VirtualObject();
   virtual ~VirtualObject() = 0;

   Variant& operator[](int index);

 // query
   const VirtualClass& getClass() const;
   void                setClass(const VirtualClass& definition);

 // operation
   void initialize(int variables);
   Variant& getMember(int index);
   void setMember(int index, const Variant& value);

 // downcasting
   virtual bool isNative() const;
   virtual VirtualNativeObject& asNative();

   virtual bool isInstance() const;
   virtual VirtualClassObject& asInstance();

private:

   const VirtualClass*  mpClass;
   Variant*             mpMembers;
   int                  mMemberCount;
};

#endif // VIRTUAL_OBJECT_H_
