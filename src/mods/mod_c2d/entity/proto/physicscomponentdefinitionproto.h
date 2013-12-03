
#ifndef PHYSICS_COMPONENT_DEFINITION_H
#define PHYSICS_COMPONENT_DEFINITION_H

#include "core/physics/bodydefinition.h"
#include "core/string/string.h"

#include "componentdefinitionproto.h"

class PhysicsComponentDefinitionProto : public ComponentDefinitionProto
{
public:
   PhysicsComponentDefinitionProto();

 // overrides
   virtual void read(DataStream& stream) override;
   virtual void write(DataStream& stream) const override;

 // data
   BodyDefinition mDefinition;
};

#endif // PHYSICS_COMPONENT_DEFINITION_H
