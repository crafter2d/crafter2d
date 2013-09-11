
#include "entitydefinition.h"

EntityDefinition::EntityDefinition():
   mChildren(),
   mCompDefs(),
   mLinks(),
   mClassName(),
   mOffset()
{
}

// - Get/set

const EntityDefinition::Children& EntityDefinition::getChildren() const
{
   return mChildren;
}

const EntityDefinition::Links& EntityDefinition::getLinks() const
{
   return mLinks;
}

const EntityDefinition::Components& EntityDefinition::getComponentDefinitions() const
{
   return mCompDefs;
}

const String& EntityDefinition::getClassName() const
{
   return mClassName;
}

void EntityDefinition::setClassName(const String& name)
{
   mClassName = name;
}

const Vector& EntityDefinition::getOffset() const
{
   return mOffset;
}

void EntityDefinition::setOffset(const Vector& offset)
{
   mOffset = offset;
}

// - Registration

void EntityDefinition::addComponentDefinition(ComponentDefinition* pdefinition)
{
   mCompDefs.push_back(pdefinition);
}

void EntityDefinition::addChild(EntityDefinition* pdefinition)
{
   mChildren.push_back(pdefinition);
}

void EntityDefinition::addLink(EntityLinkDefinition* plink)
{
   mLinks.push_back(plink);
}
