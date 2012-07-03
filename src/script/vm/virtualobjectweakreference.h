
#ifndef VIRTUAL_OBJECT_WEAK_REFERENCE_H
#define VIRTUAL_OBJECT_WEAK_REFERENCE_H

#include "core/smartptr/weakptr.h"

#include "virtualobject.h"

class VirtualObjectWeakReference
{
public:
   VirtualObjectWeakReference(): mObject() {}
   VirtualObjectWeakReference(VirtualObjectReference& ref): mObject(ref.mObject) {}

   VirtualObjectReference lock() {
      return VirtualObjectReference(*this);
   }

private:
   friend class VirtualObjectReference;

   WeakPtr<VirtualObject> mObject;
};

#endif // VIRTUAL_OBJECT_WEAK_REFERENCE_H
