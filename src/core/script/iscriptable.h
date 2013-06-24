
#ifndef ISCRIPTABLE_H_
#define ISCRIPTABLE_H_

#include "core/string/string.h"

class IScriptable
{
public:
   virtual const String& getClassName() const = 0;
};

#endif // ISCRIPTABLE_H_
