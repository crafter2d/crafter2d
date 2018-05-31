
template <class E>
ListConstIterator<E>::ListConstIterator(const List<E>& list, const ListNode<E>* pnode):
   mList(list),
   mpNode(pnode)
{
}

// - Operations

template <class E>
void ListConstIterator<E>::operator++()
{
   if ( mpNode != nullptr )
   {
      mpNode = mpNode->nextptr();
   }
}

template <class E>
void* ListConstIterator<E>::key()
{
   return (void*)mpNode;
}

template <class E>
void ListConstIterator<E>::reset()
{
   mpNode = mList._phead;
}

template <class E>
const E& ListConstIterator<E>::item() const
{
   return mpNode->data();
}

template <class E>
bool ListConstIterator<E>::isValid() const
{
   return mpNode != nullptr;
}
