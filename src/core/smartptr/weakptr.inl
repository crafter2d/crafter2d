
template <class T>
WeakPtr<T>::WeakPtr():
   mCount()
{
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr& ptr):
   mCount(ptr.mCount)
{
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& ptr):
   mCount(ptr.mCount)
{
}
