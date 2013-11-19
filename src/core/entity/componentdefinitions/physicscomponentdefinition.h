
#ifndef PHYSICS_COMPONENT_DEFINITION_H
#define PHYSICS_COMPONENT_DEFINITION_H

#include "core/physics/bodydefinition.h"

#include "componentdefinition.h"

class PhysicsComponentDefinition : public ComponentDefinition
{
public:
   PhysicsComponentDefinition();

 // get/set
   const BodyDefinition& getBodyDefinition() const;
         BodyDefinition& getBodyDefinition();

private:

 // data
   BodyDefinition mBodyDefinition;
};

#endif // PHYSICS_COMPONENT_DEFINITION_H
