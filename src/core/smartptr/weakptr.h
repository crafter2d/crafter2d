
#ifndef WEAK_PTR
#define WEAK_PTR

#include "weakcount.h"

template<class E> class SharedPtr;

template <class T>
class WeakPtr
{
public:
   WeakPtr();
   WeakPtr(const WeakPtr& ptr);
   WeakPtr(SharedPtr<T>& ptr);
   ~WeakPtr();

 // operations
   SharedPtr<T> lock();

private:
   template<class E> friend class SharedPtr;

   CountBase* mpCount;
   T*         mpPointer;
};

#include "weakptr.inl"

#endif // WEAK_PTR
