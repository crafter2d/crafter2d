
#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include "sharedcount.h"

template<class T>
class SharedPtr
{
public:
   explicit SharedPtr(T* pointer = NULL);
            SharedPtr(const SharedPtr<T>& that);
           ~SharedPtr();

 // pointer access
   const T& operator*() const;
         T& operator*();

   bool     hasPointer() const;
   const T* getPointer() const;
         T* getPointer();

 // operations
   void reset();

private:
   SharedCount mCount;
   T*          mpPointer;
};

#include "sharedptr.inl"

#endif // SHARED_PTR_H_
