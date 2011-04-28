
#ifndef VIRTUAL_OBJECT_REFERENCE_H_
#define VIRTUAL_OBJECT_REFERENCE_H_

#include "core/smartptr/sharedptr.h"

#include "virtualobject.h"

class VirtualObjectReference
{
public:
   VirtualObjectReference(VirtualObject* pobject): mObject(pobject) {}
   VirtualObjectReference(const VirtualObjectReference& that): mObject(that.mObject) {}
   ~VirtualObjectReference() {}

   VirtualObjectReference& operator=(const VirtualObjectReference& that) {
      mObject = that.mObject;
      return *this;
   }

   bool isNull() { return !mObject.hasPointer(); }
   VirtualObject* ptr() { return mObject.getPointer(); }
   void clear() { mObject.reset(); }

   const VirtualObject* operator->() const { return mObject.getPointer(); }
   VirtualObject* operator->() { return mObject.getPointer(); }
   VirtualObject& operator*() { return *mObject; }

private:
   SharedPtr<VirtualObject> mObject;
};

#endif // VIRTUAL_OBJECT_REFERENCE_H_
