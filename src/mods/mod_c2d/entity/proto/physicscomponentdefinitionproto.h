
#ifndef PHYSICS_COMPONENT_DEFINITION_H
#define PHYSICS_COMPONENT_DEFINITION_H

#include "core/physics/bodydefinition.h"
#include "core/string/string.h"

#include "componentdefinitionproto.h"

class PhysicsComponentDefinitionProto : public ComponentDefinitionProto
{
public:
   PhysicsComponentDefinitionProto();

 // data
   BodyDefinition mDefinition;

protected:
 // overrides
   virtual void virRead(DataStream& stream) override;
   virtual void virWrite(DataStream& stream) const override;
};

#endif // PHYSICS_COMPONENT_DEFINITION_H
