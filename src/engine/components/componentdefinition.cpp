
#include "componentdefinition.h"

ComponentDefinition::ComponentDefinition(ComponentInterface::ComponentType type):
   mComponentType(type)
{
}

ComponentDefinition::~ComponentDefinition()
{
}

// - Get/set

ComponentInterface::ComponentType ComponentDefinition::getComponentType() const
{
   return mComponentType;
}
