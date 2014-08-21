
#ifndef LIST_CONST_ITERATOR_H
#define LIST_CONST_ITERATOR_H

#include "core/core_base.h"

#include "constiterator.h"
#include "list.h"

template <class E>
class ListConstIterator : public ConstIterator<E>
{
public:

 // public interface
   virtual void      operator++();
   virtual void*     key();
   virtual void      reset();
   virtual const E&  item() const;

   virtual bool  isValid() const;

private:
   friend class List<E>;

   ListConstIterator();
   ListConstIterator(const List<E>& list, const ListNode<E>* pnode);

 // data
   const List<E>& mList;
   const ListNode<E>* mpNode;
};

#include "listconstiterator.inl"

#endif // LIST_CONST_ITERATOR_H
