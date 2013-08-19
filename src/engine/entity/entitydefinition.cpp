
#include "entitydefinition.h"

EntityDefinition::EntityDefinition():
   mDefinitions(),
   mClassName()
{
}

// - Get/set

const EntityDefinition::CompDefs& EntityDefinition::getComponentDefinitions() const
{
   return mDefinitions;
}

const String& EntityDefinition::getClassName() const
{
   return mClassName;
}

void EntityDefinition::setClassName(const String& name)
{
   mClassName = name;
}

// - Registration

void EntityDefinition::addComponentDefinition(ComponentDefinition* pdefinition)
{
   mDefinitions.push_back(pdefinition);
}
