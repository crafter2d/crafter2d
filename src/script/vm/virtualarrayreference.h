
#ifndef VIRTUAL_ARRAY_REFERENCE_H_
#define VIRTUAL_ARRAY_REFERENCE_H_

#include "core/smartptr/sharedptr.h"

#include "virtualarrayobject.h"

class VirtualArrayReference
{
public:
   VirtualArrayReference(VirtualArrayObject* parray): mArray(parray) {}
   VirtualArrayReference(const VirtualArrayReference& that): mArray(that.mArray) {}
   ~VirtualArrayReference() {}

   VirtualArrayReference& operator=(const VirtualArrayReference& that) {
      mArray = that.mArray;
      return *this;
   }

   VirtualArrayObject* operator->() { return mArray.getPointer(); }
   VirtualArrayObject& operator*() { return *mArray; }

   VirtualArrayObject* ptr() { return mArray.getPointer(); }

private:
    SharedPtr<VirtualArrayObject> mArray;
};

#endif // VIRTUAL_ARRAY_REFERENCE_H_
