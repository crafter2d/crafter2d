
#include "entitydefinition.h"

EntityDefinition::EntityDefinition():
   mDefinitions()
{
}

// - Registration

void EntityDefinition::addComponentDefinition(ComponentDefinition* pdefinition)
{
   mDefinitions.push_back(pdefinition);
}
