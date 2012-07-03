
#include "countimpl.h"

template<class T>
SharedPtr<T>::SharedPtr(T* pointer):
   mpCount(NULL),
   mpPointer(pointer)
{
   if ( pointer != NULL )
   {
      mpCount = new CountImpl<T>(pointer);
   }
}

template<class T>
SharedPtr<T>::SharedPtr(WeakPtr<T>& that):
   mpCount(that.mpCount),
   mpPointer(that.mpPointer)
{
   inc();
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& that):
   mpCount(that.mpCount),
   mpPointer(that.mpPointer)
{
   inc();
}

template<class T>
SharedPtr<T>::~SharedPtr()
{
   dec();
}

template<class T>
const SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& that)
{
   mpCount = that.mpCount;
   mpPointer = that.mpPointer;

   inc();
   
   return *this;
}

// pointer access

template<class T>
const T& SharedPtr<T>::operator*() const
{
   return mpPointer;
}

template<class T>
T& SharedPtr<T>::operator*()
{
   return *mpPointer;
}

template<class T>
bool SharedPtr<T>::hasPointer() const
{
   return mpPointer != NULL;
}

template<class T>
const T* SharedPtr<T>::getPointer() const
{
   ASSERT_PTR(mpPointer);
   return mpPointer;
}

template<class T>
T* SharedPtr<T>::getPointer()
{
   ASSERT_PTR(mpPointer);
   return mpPointer;
}

// query

template<class T>
bool SharedPtr<T>::isUnique() const
{
   return useCount() == 1;
}

template<class T>
int SharedPtr<T>::useCount() const
{
   return mpCount != NULL ? mpCount->useCount() : 0;
}

// - Operations

template<class T>
void SharedPtr<T>::inc()
{
   if ( mpCount != NULL )
   {
      mpCount->addRef();
   }
}

template<class T>
void SharedPtr<T>::dec()
{
   if ( mpCount != NULL )
   {
      mpCount->releaseRef();
   }
}

template<class T>
void SharedPtr<T>::reset()
{
   dec();

   mpCount   = NULL;
   mpPointer = NULL;
}
