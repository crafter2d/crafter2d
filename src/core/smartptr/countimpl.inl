
template<class T>
CountImpl<T>::CountImpl(T* pdata):
   mpData(pdata)
{
}

// - Overloads

template<class T>
void CountImpl<T>::dispose()
{
   delete mpData;
   mpData = NULL;
}
