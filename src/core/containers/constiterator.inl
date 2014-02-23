
template <class E>
ConstIterator<E>::ConstIterator()
{
}

// - Operations

template <class E>
const E& ConstIterator<E>::operator*() const
{
   return item();
}
