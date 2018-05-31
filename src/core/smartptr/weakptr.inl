
template <class T>
WeakPtr<T>::WeakPtr():
   mpCount(nullptr),
   mpPointer(nullptr)
{
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr& ptr):
   mpCount(ptr.mpCount),
   mpPointer(ptr.mpPointer)
{
   if ( mpCount != nullptr )
   {
      mpCount->addWeakRef();
   }
}

template <class T>
WeakPtr<T>::WeakPtr(SharedPtr<T>& ptr):
   mpCount(ptr.mpCount),
   mpPointer(ptr.mpPointer)
{
   if ( mpCount != nullptr )
   {
      mpCount->addWeakRef();
   }
}

template <class T>
WeakPtr<T>::~WeakPtr()
{
   if ( mpCount != nullptr )
   {
      mpCount->releaseWeakRef();
   }
}

// operations
   
template <class T>
SharedPtr<T> WeakPtr<T>::lock()
{
   return SharedPtr<T>(*this);
}
