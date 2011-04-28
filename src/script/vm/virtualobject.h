
#ifndef VIRTUAL_OBJECT_H_
#define VIRTUAL_OBJECT_H_

class Variant;
class VirtualClass;
class VirtualClassObject;
class VirtualNativeObject;

class VirtualObject
{
public:
   VirtualObject();
   virtual ~VirtualObject() = 0;

 // query
   const VirtualClass& getClass() const;
   void                setClass(const VirtualClass& definition);

 // downcasting
   virtual bool isNative() const;
   virtual VirtualNativeObject& asNative();

   virtual bool isInstance() const;
   virtual VirtualClassObject& asInstance();

private:
   const VirtualClass*  mpClass;
};

#endif // VIRTUAL_OBJECT_H_
