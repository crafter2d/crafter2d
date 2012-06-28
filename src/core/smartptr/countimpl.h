
#ifndef COUNT_IMPL_H
#define COUNT_IMPL_H

#include "countbase.h"

template<class T>
class CountImpl : public CountBase
{
public:
   CountImpl(T* pdata);

protected:
 // overloads
   virtual void dispose();

private:

   T* mpData;
};

#include "countimpl.inl"

#endif // COUNT_IMPL_H
