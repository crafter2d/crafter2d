
#include "componentfactory.h"

ComponentFactory::ComponentFactory(ComponentInterface::ComponentType type):
   mComponentType(type)
{
}

ComponentFactory::~ComponentFactory()
{
}

// - Get/set

ComponentInterface::ComponentType ComponentFactory::getComponentType() const
{
   return mComponentType;
}
