
#ifndef CONST_ITERATOR_H
#define CONST_ITERATOR_H

#include "core/core_base.h" 

template <class E>
class ConstIterator
{
public:

   const E& operator*() const;

 // public interface
   virtual void      operator++() = 0;
   virtual void*     key() = 0;
   virtual void      reset() = 0;
   virtual const E&  item() const = 0;

   virtual bool  isValid() const = 0;

protected:
   ConstIterator();
};

#include "constiterator.inl"

#endif // CONST_ITERATOR_H
