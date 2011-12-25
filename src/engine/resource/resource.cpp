
#include "resource.h"

Resource::Resource():
	mName()
{
}

Resource::Resource(const std::string& name):
   mName(name)
{
}

// get/set

const std::string& Resource::getName() const
{
   return mName;
}

void Resource::setName(const std::string& name)
{
   mName = name;
}
