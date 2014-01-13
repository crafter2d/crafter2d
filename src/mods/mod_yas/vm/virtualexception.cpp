
#include "virtualexception.h"

#include "core/defines.h"

#include "virtualobject.h"

VirtualException::VirtualException():
   mpException(NULL)
{
}

VirtualException::VirtualException(VirtualObject& exception):
   mpException(&exception)
{
}

// get/set
   
const VirtualObject& VirtualException::getException() const
{
   ASSERT_PTR(mpException);
   return *mpException;
}

VirtualObject& VirtualException::getException()
{
   ASSERT_PTR(mpException);
   return *mpException;
}

void VirtualException::setException(VirtualObject& object)
{
   mpException = &object;
}
