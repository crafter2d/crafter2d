
#ifndef HASHMAP_BUCKET_H
#define HASHMAP_BUCKET_H

template <typename K, class E>
class HashMapBucket
{
public:
   HashMapBucket(K key, E& element);

 // get/set
   K              getKey();

   E&             getElement();
   void           setElement(E& element);

   bool                hasNext() const;
   HashMapBucket<K,E>* getNext();
   void                setNext(HashMapBucket<K,E>* pnext);

private:
   K              mKey;
   E              mElement;
   HashMapBucket* mpNext;
};

#include "hashmapbucket.inl"

#endif // HASHMAP_BUCKET_H
