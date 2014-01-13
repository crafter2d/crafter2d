
#ifndef VIRTUAL_EXCEPTION_H_
#define VIRTUAL_EXCEPTION_H_

#include "script/script_base.h"

class VirtualObject;

class SCRIPT_API VirtualException
{
public:
            VirtualException();
   explicit VirtualException(VirtualObject& exception);

 // get/set
   const VirtualObject& getException() const;
         VirtualObject& getException();
   void                 setException(VirtualObject& object);

private:
   VirtualObject* mpException;
};

#endif // VIRTUAL_EXCEPTION_H_
