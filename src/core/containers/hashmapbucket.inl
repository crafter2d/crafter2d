
#include "core/defines.h"

template <typename K, class E>
HashMapBucket<K,E>::HashMapBucket(K key, E& element):
   mKey(key),
   mElement(element),
   mpNext(NULL)
{
}

// - Get/set

template <typename K, class E>
K HashMapBucket<K,E>::getKey()
{
   return mKey;
}
   
template <typename K, class E>
E& HashMapBucket<K,E>::getElement()
{
   return mElement;
}

template <typename K, class E>
void HashMapBucket<K,E>::setElement(E& element)
{
   mElement = element;
}

template <typename K, class E>
bool HashMapBucket<K,E>::hasNext() const
{
   return mpNext != NULL;
}

template <typename K, class E>
HashMapBucket<K,E>* HashMapBucket<K,E>::getNext()
{
   return mpNext;
}

template <typename K, class E>
void HashMapBucket<K,E>::setNext(HashMapBucket<K,E>* pnext)
{
   mpNext = pnext;
}
