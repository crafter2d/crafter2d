
#include "module.h"

Module::Module(Type type):
   mType(type)
{
}

Module::~Module()
{
}

// - Get/set

Module::Type Module::getType() const
{
   return mType;
}

void Module::setType(Type type)
{
   mType = type;
}
