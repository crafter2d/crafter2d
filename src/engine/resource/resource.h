
#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>

class Resource
{
public:
   Resource();
   Resource(const std::string& name);

 // get/set
   const std::string& getName() const;
   void               setName(const std::string& name);

private:
   std::string mName;
};

#endif // RESOURCE_H_
