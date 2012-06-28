
#ifndef HASHMAP_ITERATOR_H
#define HASHMAP_ITERATOR_H

#include "iterator.h"
#include "hashmap.h"
#include "hashmapbucket.h"

template <typename K, class E>
class HashMapIterator : public Iterator<E>
{
public:
   HashMapIterator<K,E>& operator=(const HashMapIterator& that);

 // overloads
   virtual void  operator++();

   virtual void* key();
   virtual void  reset();
   virtual E&    item();

           void  remove();

   virtual bool  isValid() const;

private:
   friend class HashMap<K,E>;

   HashMapIterator(HashMap<K,E>& hashmap);

 // operations
   void initialize();

   HashMap<K,E>*        mpHashMap;
   HashMapBucket<K,E>*  mpBucket;
   int                  mIndex;
};

#include "hashmapiterator.inl"

#endif // HASHMAP_ITERATOR_H
