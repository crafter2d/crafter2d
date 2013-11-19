
#include "physicscomponentdefinition.h"

PhysicsComponentDefinition::PhysicsComponentDefinition():
   ComponentDefinition(ComponentInterface::ePhysisComponent),
   mBodyDefinition()
{
}

// - Get/set

const BodyDefinition& PhysicsComponentDefinition::getBodyDefinition() const
{
   return mBodyDefinition;
}

BodyDefinition& PhysicsComponentDefinition::getBodyDefinition()
{
   return mBodyDefinition;
}
