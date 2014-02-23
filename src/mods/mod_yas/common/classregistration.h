
#ifndef CLASS_REGISTRATION_H
#define CLASS_REGISTRATION_H

#include <vector>

#include "core/string/string.h"

class FunctionRegistration;

class ClassRegistration
{
public:
   typedef std::vector<FunctionRegistration*> Functions;

   String name;
   Functions functions;
   int start;
   int end;
};

#endif // CLASS_REGISTRATION_H
