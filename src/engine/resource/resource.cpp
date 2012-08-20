
#include "resource.h"

Resource::Resource():
	mName()
{
}

Resource::Resource(const std::string& name):
   mName(name)
{
}

Resource::~Resource()
{
}

// get/set

const String& Resource::getName() const
{
   return mName;
}

void Resource::setName(const String& name)
{
   mName = name;
}
