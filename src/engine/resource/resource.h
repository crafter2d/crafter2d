
#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>

class Resource
{
public:
   virtual const std::string& getFilename() const = 0;
};

#endif // RESOURCE_H_
