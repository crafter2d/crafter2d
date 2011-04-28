
template<class T>
SharedPtr<T>::SharedPtr(T* pointer):
   mCount(),
   mpPointer(pointer)
{
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& that)
{
}

template<class T>
SharedPtr<T>::~SharedPtr()
{
   if ( mCount.isUnique() )
   {
      delete mpPointer;
   }
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

// - Operations

template<class T>
void SharedPtr<T>::reset()
{
   mCount.reset();

   delete mpPointer;
   mpPointer = NULL;
}
