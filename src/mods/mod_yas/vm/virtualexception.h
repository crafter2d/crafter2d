
#ifndef VIRTUAL_EXCEPTION_H_
#define VIRTUAL_EXCEPTION_H_

#include "mod_yas/script_base.h"

class VirtualObject;

class VirtualException
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
