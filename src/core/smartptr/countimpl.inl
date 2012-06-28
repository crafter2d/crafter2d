
template<class T>
CountImpl<T>::CountImpl(T* pdata)
{
}

// - Overloads

template<class T>
void CountImpl<T>::dispose()
{
   delete mpData;
   mpData = NULL;
}
