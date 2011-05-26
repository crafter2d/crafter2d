
#include "virtualexception.h"

VirtualException::VirtualException():
   mException(NULL)
{
}

VirtualException::VirtualException(const VirtualObjectReference& exception):
   mException(exception)
{
}

// get/set
   
const VirtualObjectReference& VirtualException::getException() const
{
   return mException;
}

void VirtualException::setException(const VirtualObjectReference& object)
{
   mException = object;
}
