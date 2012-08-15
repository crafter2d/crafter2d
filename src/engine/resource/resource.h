
#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "core/string/string.h"

class Resource
{
public:
   Resource();
   Resource(const std::string& name);
   virtual ~Resource();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

private:
   String mName;
};

#endif // RESOURCE_H_
