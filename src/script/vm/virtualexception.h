
#ifndef VIRTUAL_EXCEPTION_H_
#define VIRTUAL_EXCEPTION_H_

#include "virtualobjectreference.h"

class VirtualException
{
public:
   VirtualException();
   explicit VirtualException(const VirtualObjectReference& exception);

 // get/set
   const VirtualObjectReference& getException() const;
   void                          setException(const VirtualObjectReference& object);

private:
   VirtualObjectReference mException;
};

#endif // VIRTUAL_EXCEPTION_H_
